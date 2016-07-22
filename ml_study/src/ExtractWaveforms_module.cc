
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>
#include "tbb/parallel_reduce.h"
#include "tbb/parallel_sort.h"

// Framework includes
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h" 
#include "art/Framework/Principal/Event.h" 
#include "fhiclcpp/ParameterSet.h" 
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "art/Framework/Principal/Handle.h" 
#include "art/Persistency/Common/Ptr.h" 
#include "art/Persistency/Common/PtrVector.h" 
#include "art/Framework/Services/Registry/ServiceHandle.h" 
#include "art/Framework/Services/Optional/TFileService.h" 
#include "art/Framework/Services/Optional/TFileDirectory.h"
#include "art/Framework/Core/FindOne.h" 
#include "art/Framework/Core/FindMany.h" 
#include "messagefacility/MessageLogger/MessageLogger.h" 
#include "cetlib/maybe_ref.h"

// LArSoft includes
#include "larcore/SimpleTypesAndConstants/geo_types.h"
#include "larcore/SimpleTypesAndConstants/RawTypes.h" // raw::ChannelID_t
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "larcore/Geometry/Geometry.h"
// #include "larsim/Filters/ChannelFilter.h"
#pragma GCC diagnostic pop
#include "larcore/Geometry/CryostatGeo.h"
#include "larcore/Geometry/TPCGeo.h"
#include "larcore/Geometry/PlaneGeo.h"
#include "larcore/Geometry/WireGeo.h"
#include "lardata/RecoBase/Wire.h"
#include "lardata/RecoBase/Hit.h"
#include "lardata/RecoBase/Cluster.h"
#include "lardata/RecoBase/Track.h"
#include "lardata/RecoBase/Vertex.h"
#include "lardata/RecoBase/SpacePoint.h"
// #include "larsim/Utilities/LArProperties.h"
// #include "larsim/Utilities/DetectorProperties.h"
// #include "Utilities/AssociationUtil.h"
#include "lardata/RawData/ExternalTrigger.h"
#include "lardata/RawData/RawDigit.h"
#include "lardata/RawData/OpDetPulse.h"
#include "lardata/RawData/raw.h"
// #include "RawDataUtilities/TriggerDigitUtility.h" 
// #include "MCCheater/BackTracker.h"
#include "larsim/Simulation/SimChannel.h"

#include "SimulationBase/MCTruth.h"
#include "SimulationBase/MCParticle.h"
#include "SimulationBase/MCTrajectory.h"


// ROOT includes
#include "TComplex.h"
#include "TFile.h"
#include "TH2D.h"
#include "TF1.h"
#include "TTree.h"
#include "TTimeStamp.h"

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <iterator>
#include <algorithm>

using namespace std;

struct Waveform;

namespace study {
  class ExtractWaveforms;
  short calc_median(Waveform& wfs, size_t dim_S);
  short calc_median(Waveform** wfsp, size_t dim_S, size_t wire);
}


struct Waveform {
  
  Waveform() :
    type("NONE"), channel(0), plane(0), wireindex(0), wiresz(0), eid(0), ped(0.0),
    adcs(),median(0)
  {
    adcs.reserve(10000);
  }
  
  Waveform(string const& t, int c, int pl, int wi, int sz, int e, float p) :
    type(t), channel(c), plane(pl), wireindex(wi), wiresz(sz), eid(e), ped(p),
    adcs(),median(0)
  {
    adcs.reserve(10000);
  }
  
  Waveform(istream& ist) :
    type("NONE"), channel(0), plane(0), wireindex(0), wiresz(0), eid(0), ped(0.0),
    adcs(),median(0)
  {
    float sigma;
    int samples;
    adcs.reserve(10000);
    ist >> eid >> type >> channel >> plane
	>> wireindex >> wiresz >> samples >> ped >> sigma;
    
    short val;
    while (!(ist >> val).eof()) adcs.push_back(val);
    study::calc_median(*this,adcs.size());
  }
  
  void fill(raw::RawDigit::ADCvector_t& uncom, size_t total)
  {
    for (size_t i = 0; i < total; ++i) 
      {
	adcs.push_back(uncom[i]);
      }
    median=study::calc_median(*this, total);
  }
  
  string type;
  int channel;
  int plane;
  int wireindex;
  int wiresz;
  int eid;
  float ped;
  vector<short> adcs;
  short median;
};

typedef vector<Waveform> Waveforms;
typedef std::vector<std::string> Strings;

class study::ExtractWaveforms : public art::EDAnalyzer
{
public:
  explicit ExtractWaveforms(fhicl::ParameterSet const & p);
  virtual ~ExtractWaveforms();

  void analyze(art::Event const & e) override;
  void beginJob();
private:
  void processTracks(art::Event const& e, 
		     std::string const& label, 
		     int const& id);
  
  void processSP(art::Event const& e, std::string const& label);
  void processMC(art::Event const& e, std::string const& label);
  void processMCPart(art::Event const& e, std::string const& label);
  void processParticle(simb::MCParticle const& mcp);

  void processDigits(art::Event const& e, std::string const& label);
  short calc_median(Waveform** wfsp, 
		    size_t dim_S, size_t dim, 
		    double band, short& out_low, short& out_high);
  //short calc_median(Waveform** wfsp, 
  //		    size_t dim_S, size_t wire); 
  //short calc_median(Waveform& wfsp, 
  //		    size_t dim_S); 
  void gen_points(std::string const& filename,
		  Waveform** wfsp, size_t dim_S, size_t dim);
    
  bool as_mask_;
  std::string prefix_;
  Strings write_digits_;
  bool find_sigs_;
  size_t zeros_;
  bool use_delta_;
  std::string filename_out_u;
  std::string filename_out_v;
  std::string filename_out_y;
  geo::Geometry* geom_;
};

std::string makeName(std::string const& pre, std::string const& rest)
{
  std::stringstream ss;
  ss << pre << rest << ".csv";
  return ss.str();
}

void checkFile(ofstream& s, std::string const& n)
{
  if (!s)
    {
      std::ostringstream ss;
      ss << "Failed to open output file " << n << "\n";
      std::cerr << ss.str();
      throw std::runtime_error(ss.str().c_str());
    }
}

study::ExtractWaveforms::ExtractWaveforms(fhicl::ParameterSet const & pset) :
  EDAnalyzer(pset),
  as_mask_(pset.get< bool >("as_mask",false)),
  prefix_(pset.get< std::string >("file_name", "output_")),
  write_digits_(pset.get<Strings>("write_digits", Strings())),
  find_sigs_(pset.get< bool >("find_sigs",false)),
  zeros_(pset.get<size_t>("zeros",3)),
  use_delta_(pset.get< bool >("use_delta",false)),
  filename_out_u(prefix_ + "U.csv"),
  filename_out_v(prefix_ + "V.csv"),
  filename_out_y(prefix_ + "Y.csv"),
  geom_()
{
  std::cerr << "In module ctor\n";
  art::ServiceHandle<geo::Geometry> geom;
  geom_ = &(*geom);
}

study::ExtractWaveforms::~ExtractWaveforms() { }

//short study::ExtractWaveforms::calc_median(Waveform** wfsp, size_t dim_S, size_t wire)
short study::calc_median(Waveform** wfsp, size_t dim_S, size_t wire)
{
  Waveform& wfa = *(wfsp[wire]);
  return calc_median(wfa,dim_S);
}

//short study::ExtractWaveforms::calc_median(Waveform& wfs, size_t dim_S)
short study::calc_median(Waveform& wfa, size_t dim_S)
{
  // const double get_rid = .49;
  std::vector<short> all_plane;
  all_plane.reserve(3500 * 10000);
  //cout << "generating median" << std::endl;
  
  for (size_t sample = 0; sample < dim_S; ++sample)
    all_plane.push_back(wfa.adcs[sample]);
  
  sort(all_plane.begin(), all_plane.end());
  auto all_half_u = all_plane.size() / 2;
  auto all_median_u = all_plane[all_half_u];
  return all_median_u;
}

short study::ExtractWaveforms::calc_median(Waveform** wfsp, size_t dim_S, size_t dim, double band, short& out_low, short& out_high) 
{
  // const double get_rid = .49;
  std::vector<short> all_plane;
  all_plane.reserve(3500 * 10000);
  cout << "generating median" << std::endl;
  
  for (size_t wire = 0; wire < dim; ++wire) 
    {
      Waveform& wfa = *(wfsp[wire]);
      for (size_t sample = 0; sample < dim_S; ++sample)
	all_plane.push_back(wfa.adcs[sample]);
    }
  
  sort(all_plane.begin(), all_plane.end());
  auto all_half_u = all_plane.size() / 2;
  auto all_pos = all_half_u * band;
  auto all_median_u = all_plane[all_half_u];
  out_low = all_plane[all_half_u - all_pos];
  out_high = all_plane[all_half_u + all_pos];
  cout << out_low << " " << all_median_u << " " << out_high << "\n";
  return all_median_u;
}

void study::ExtractWaveforms::processDigits(art::Event const& e, 
					    std::string const& label)
{
  auto digits = e.getValidHandle< std::vector < raw::RawDigit >> (label);
  auto eid = e.event();

  Waveforms wfs;
  wfs.reserve(10000);

#if 1
  auto digi = *digits;
  wfs.resize(digi.size());
  tbb::blocked_range<size_t> range(0, digi.size());
  tbb::parallel_for(range, [&](const tbb::blocked_range<size_t>& r) {
      for (size_t i = r.begin(); i < r.end(); ++i)
	{
	  raw::RawDigit::ADCvector_t uncom(digi[i].Samples());
	  raw::Uncompress(digi[i].ADCs(), uncom, digi[i].Compression());

	  std::vector<geo::WireID> wireids = geom_->ChannelToWire(digi[i].Channel());
	  auto plane_id = wireids[0].planeID();
	  auto wire_idx = wireids[0].Wire;
	  
	  Waveform wf("eid,type,channel,plane,wireindex,wiresz,samples,ped,sigma\n", 
		      digi[i].Channel(), plane_id.Plane, 
		      wire_idx, wireids.size(), 
		      eid, digi[i].GetPedestal());

	  wf.fill(uncom, digi[i].Samples());
	  wfs[i]=wf;
	}
    }
    );
#else
  for (auto const& it : (*digits)) 
    {
      std::vector<geo::WireID> wireids = geom_->ChannelToWire(it.Channel());
      auto plane_id = wireids[0].planeID();
      auto wire_idx = wireids[0].Wire;
      Waveform wf("eid,type,channel,plane,wireindex,wiresz,samples,ped,sigma\n", 
		  it.Channel(), plane_id.Plane, 
		  wire_idx, wireids.size(), 
		  eid, it.GetPedestal());
      raw::RawDigit::ADCvector_t uncom(it.Samples());
      raw::Uncompress(it.ADCs(), uncom, it.Compression());

      for (size_t i = 0; i < it.Samples(); ++i) 
	{
	  wf.adcs.push_back(uncom[i]);
        }
      wfs.push_back(wf);
    }
  std::cout << "finished reading points\n";
#endif

  vector<Waveform*> wfsp;
  wfsp.reserve(wfs.size());
#if 1
  wfsp.resize(wfs.size());
  tbb::blocked_range<size_t> wrange(0, wfsp.size());
  tbb::parallel_for(wrange, [&](const tbb::blocked_range<size_t>& r) {
      for (size_t i = r.begin(); i < r.end(); ++i)
	{
	  wfsp[i] = &wfs[i];
	}
    }
    );

  tbb::parallel_sort(wfsp.begin(), wfsp.end(),
		     [](Waveform* a, Waveform * b) {
		       return (a->plane < b->plane) ? true :
			 (a->plane > b->plane) ? false :
			 (a->wireindex < b->wireindex) ? true :
			 false;
		     }
		     );
#else  
  for (size_t i = 0; i < wfs.size(); ++i) 
    {
      wfsp.push_back(&wfs[i]);
    }
  sort(wfsp.begin(), wfsp.end(),
       [](Waveform* a, Waveform * b) {
	 return (a->plane < b->plane) ? true :
	   (a->plane > b->plane) ? false :
	   (a->wireindex < b->wireindex) ? true :
	   false;
       }
       );
#endif
  std::cout << "finished sorting points\n";
  size_t dim_UV = 2400, dim_Y = 3456;
  size_t dim_S = wfs[0].adcs.size(); // /4;
    
  // first u, then v, then y
  {
  ostringstream ofn;
  ofn << eid << "_" << filename_out_u;
  gen_points( ofn.str(), &wfsp[0], dim_S, dim_UV);
  }
  {
  ostringstream ofn;
  ofn << eid << "_" << filename_out_v;
  gen_points( ofn.str(), &wfsp[dim_UV], dim_S, dim_UV);
  }
  {
  ostringstream ofn;
  ofn << eid << "_" << filename_out_y;
  gen_points( ofn.str(), &wfsp[dim_UV * 2], dim_S, dim_Y);
  }

}

struct SigRange
{
  size_t wire;
  size_t start;
  size_t end;
  size_t size;
  int sum;
  short median;
};
std::ostream& operator<<(std::ostream& ost, SigRange const& sr)
{
  ost << sr.wire << " " << sr.start << " "
      << sr.end << " " << sr.size << " " << sr.sum << " " << sr.median;
  return ost;
}

struct SigBody
{
  SigBody(Waveform** wfs,size_t ds,size_t d,int ns, bool ud):
    dim_S(ds),dim(d),no_sig(ns),wfsp(wfs),use_delta(ud) { }
  
  SigBody(SigBody& s,tbb::split):
    dim_S(s.dim_S),dim(s.dim),no_sig(s.no_sig),wfsp(s.wfsp),use_delta(s.use_delta) { }
  // ~SigBody() { }

  size_t dim_S,dim;
  int no_sig;
  Waveform** wfsp;
  bool use_delta;
  
  void operator()(const tbb::blocked_range<size_t>& r)
  {
    for (size_t i = r.begin(); i < r.end(); ++i)
      {
	Waveform& wf_u = *(wfsp[i]);
	int sum=0;
	size_t start=0, end=0;
	int state=no_sig;
	auto curr = wf_u.adcs[0];
	
	for (size_t sample = 0; sample < dim_S; ++sample) 
	  {
	    short au = wf_u.adcs[sample];
	    auto d = au - (use_delta?curr:wf_u.median);
	    curr=au;
	    
	    if(state==0)
	      { if(d==0) { ++state; end=sample; }}
	    else if(state==no_sig)
	      { if(d!=0) { state=0; start=sample; }}
	    else if(state==(no_sig-1))
	      {
		if(d==0)
		  {
		    // save currrent SigRange object
		    sigs.emplace_back( SigRange({ i, start,end, (end-start), sum, wf_u.median }) );
		    start=0;
		    end=0;
		    ++state;
		  }
		else state=0;
	      }
	    else
	      {
		if(d==0) ++state; else state=0;
	      }
	    sum+=d;
	  }
	
	if(start!=0 || end!=0)
	  {
	    if(end<start) end=dim_S;
	    // save current SigRange object
	    sigs.emplace_back( SigRange({ i, start,end, (end-start), sum, wf_u.median }) );
	  }
      }
  }

  void join(SigBody& rhs)
  {
    sigs.insert(sigs.end(), rhs.sigs.begin(), rhs.sigs.end());
  }

  vector<SigRange> sigs;
  
};

void study::ExtractWaveforms::gen_points(
					 std::string const& filename,
					 Waveform** wfsp, 
					 size_t dim_S, size_t dim) 
{
  std::cout << "Gen_points called\n";
  ofstream outf(filename.c_str());

  for (size_t wire = 0; wire < dim; ++wire) 
    {
      Waveform& wf_u = *(wfsp[wire]);
      short median = as_mask_?wf_u.median:0;
      
      for (size_t sample = 0; sample < dim_S; ++sample) 
	{
	  short au = wf_u.adcs[sample] - median;
          outf << au << " ";
	}
      outf << "\n";
    }

  if(!find_sigs_) return;

  int no_sig = (int)zeros_;
  tbb::blocked_range<size_t> range(0, dim);
#if 1
  SigBody b(wfsp, dim_S, dim, no_sig,use_delta_);
  parallel_reduce(range, b);
  ofstream outfs((string("sig_") + filename).c_str());
  copy(b.sigs.begin(),b.sigs.end(),std::ostream_iterator<SigRange>(outfs,"\n"));
#else
  tbb::parallel_for(range, [&](const tbb::blocked_range<size_t>& r) {
      vector<SigRange> sigs;
      for (size_t i = r.begin(); i < r.end(); ++i)
	{
	  Waveform& wf_u = *(wfsp[i]);
	  int sum=0;
	  size_t start=0, end=0;
	  int state=no_sig;
	  auto curr = wf_u.adcs[0];
	  
	  for (size_t sample = 0; sample < dim_S; ++sample) 
	    {
	      short au = wf_u.adcs[sample];
	      auto d = au-curr;
	      curr=au;

	      if(state==0)
		{ if(d==0) { ++state; end=sample; }}
	      else if(state==no_sig)
		{ if(d!=0) { state=0; start=i; }}
	      else if(state==(no_sig-1))
		{
		  if(d==0)
		    {
		      // save currrent SigRange object
		      sigs.emplace_back( SigRange({ start,end, (end-start), sum }) );
		      start=0;
		      end=0;
		      ++state;
		    }
		  else state=0;
		}
	      else
		{
		  if(d==0) ++state; else state=0;
		}
	      sum+=d;
	    }
	  
	  if(start!=0 || end!=0)
	    {
	      if(end<start) end=dim_S;
	      // save current SigRange object
	      sigs.emplace_back( SigRange({ start,end, (end-start), sum }) );
	    }
	}
    }
    );
#endif
}

void study::ExtractWaveforms::analyze(art::Event const & e) 
{
  std::cerr << "In module analyze\n";

  if (!write_digits_.empty()) 
    {
      for (auto const& lab : write_digits_)
	processDigits(e, lab);
    }
}

void study::ExtractWaveforms::beginJob() { }

DEFINE_ART_MODULE(study::ExtractWaveforms)
