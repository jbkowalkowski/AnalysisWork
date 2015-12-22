
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <thread>
#include <cmath>
#include <stdexcept>
#include <unordered_map>
#include <time.h>
#include <mutex>

#include <unistd.h>
#include <sys/stat.h>

#include "mpi.h"

using namespace std;

// ----------------


class Mpi
{
public:
  Mpi(int argc, char* argv[]);
  ~Mpi();

  int rank() const { return rank_; }
  int np() const { return np_; }
  int hostid() const { return hostid_; }
  std::string name() const { return name_; }

  int rank_group() const { return rank_group_; }
  int np_group() const { return np_group_; }

  void barrier();

private:
  int np_;
  int rank_;
  int rank_group_;
  int np_group_;
  int hostid_;
  std::string name_;
  MPI_Comm comm_;

};

Mpi::Mpi(int argc, char* argv[])
{
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD,&np_);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank_);
  hostid_ = (unsigned int)(gethostid());

  int name_sz = 0;
  char name_buf[MPI_MAX_PROCESSOR_NAME];
  MPI_Get_processor_name(name_buf, &name_sz);
  name_ = name_buf;

  MPI_Comm_split(MPI_COMM_WORLD, hostid_, rank_, &comm_);
  MPI_Comm_rank(comm_, &rank_group_);
  MPI_Comm_size(comm_, &np_group_);

}

Mpi::~Mpi()
{
  MPI_Finalize();
}

void Mpi::barrier()
{
  MPI_Barrier(MPI_COMM_WORLD);
}


// ----------------- 
// this is the thread function there actual work is done

struct Params
{
  size_t id;
  string common;
  string pdf;
  size_t seed;
  string& output;
  std::mutex& m;
};

void go(Params p)
{
  ostringstream ost;
  ost << p.id << "+" << p.seed;
  p.output = ost.str();

  {
    // protects access to cout / cerr from all threads
    std::lock_guard<std::mutex> l(p.m);
    cout << p.id << " working with " << p.seed << "\n";
  }
}


// ----------------

int main(int argc, char* argv[])
{
  Mpi mpi(argc,argv);

  ostringstream ost;
  ost << "qout" << mpi.rank();
  ofstream out(ost.str().c_str());

  cerr << "process " << mpi.rank() << " started\n";

  mpi.barrier();
  double st = MPI_Wtime();

  // ----------------- 
  // broadcast out the three configurations

  string common;
  string pdf;
  int specific;

  common.resize(10000);
  pdf.resize(10000);

  // read the common config here
  if (mpi.rank()==0) 
    {
      common = "this is common junk";
      common.resize(10000);
    }
  
  MPI_Bcast(&common[0],common.size(),MPI_BYTE,0,MPI_COMM_WORLD);
  out << "common config = " << common << " " << common.size() << "\n";
  out.flush();

  // set up the PDF stuff here
  if (mpi.rank()==0) 
    {
      pdf = "this is PDF junk";
      pdf.resize(10000);
    }
  
  MPI_Bcast(&pdf[0],pdf.size(),MPI_BYTE,0,MPI_COMM_WORLD);
  out << "pdf = " << pdf << "\n";
  out.flush();
  
  // prepare the specific configurations
  vector<int> vs;
  if (mpi.rank()==0) 
    {
      vs.resize(mpi.np());
      for(size_t i=0;i<mpi.np();++i)
	{
	  vs[i]=i*10000;
	}
    }
  
  MPI_Scatter(&vs[0],1,MPI_INT,&specific,1,MPI_INT, 0,MPI_COMM_WORLD);  
  out << "specific = " << specific << "\n";
  out.flush();

  cerr << "after init\n";

#if 1
  // -----------------
  // start 32 threads on each node

  std::mutex mu;
  const size_t tot_threads=32;
  vector<std::thread> tvec;
  vector<string> outputs(tot_threads);
  for(size_t i=0;i<tot_threads;++i)
    {
      Params p = {i,common,pdf,specific+i,outputs[i],mu};
      tvec.push_back(std::thread (std::bind(go,p)));
    }

  for(auto& t:tvec) t.join();

  // at this point outputs has the string output from all the threads
#endif

  mpi.barrier();

  // ----------------- 
  // let's gather a result (10 strings from each rank)
  // 
  const char sep = 0x09;

  size_t tot_str=10;
  int total;
  ostringstream ovals;
  for(size_t i=0;i<outputs.size();++i)
    {
      ovals << outputs[i] << sep;
    }
  string cvals = ovals.str();
  total = cvals.size();

  if(mpi.rank()==0)
    {
      // first get lengths of each string from each rank
      std::vector<int> sizes(mpi.np());
      MPI_Gather(&total,1, MPI_INT, &sizes[0],1, MPI_INT, 
		 0, MPI_COMM_WORLD);

      // set the total size of the return string across all ranks
      string recv;
      auto sum = accumulate(sizes.begin(),sizes.end(),0);
      recv.resize(sum);
      cerr << "total size = " << sum << "\n";

      // set the offsets required by gatherv
      vector<int> offsets(sizes.size());
      offsets[0]=0;
      for(size_t i=1;i<sizes.size();++i) 
	{
	  offsets[i]=offsets[i-1]+sizes[i-1];
	}

      MPI_Gatherv(&cvals[0],total, MPI_BYTE, &recv[0],&sizes[0],&offsets[0],
		  MPI_BYTE,0,MPI_COMM_WORLD);

      cerr << "completed gatherv at root\n";
      cerr << recv << "\n";

      // extract the strings at the root and print them
      istringstream valstr(recv);
      string ext;
      while(!getline(valstr,ext,sep).eof())
	{
	  cout << ext << "\n";
	}
    }
  else
    {
      cerr << "start gather (output\n";
      int* dummy = 0;
      MPI_Gather(&total, 1, MPI_INT, dummy,1,MPI_INT,
		 0,MPI_COMM_WORLD);
      cerr << "completing gather (output)\n";
      MPI_Gatherv(&cvals[0], total, MPI_BYTE, dummy,dummy,dummy,
		  MPI_BYTE,0,MPI_COMM_WORLD);
      cerr << "completing gatherv\n";
    }

  // -----------------

  mpi.barrier();

  if (mpi.rank()==0)
    {
      double end = MPI_Wtime() - st;
      cout << "time = " << end << "\n";
    }

  return 0;
}
