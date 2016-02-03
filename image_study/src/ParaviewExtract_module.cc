
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

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
#include "art/Framework/Core/FindOneP.h" 
#include "messagefacility/MessageLogger/MessageLogger.h" 
#include "cetlib/maybe_ref.h"

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <stdexcept>

using namespace std;

class Extract : public art::EDAnalyzer 
{
public:
  explicit Extract(fhicl::ParameterSet const & p);
  virtual ~Extract();
  
  void analyze(art::Event const & e) override;
  void beginJob();
private:
};

std::string makeName(std::string const& pre, std::string const& rest)
{
   std::stringstream ss;
   ss << pre << rest << ".csv";;
   return ss.str();
}

void checkFile(ofstream& s, std::string const& n)
{
  if(!s)
    {
      std::ostringstream ss;
      ss << "Failed to open output file " << n << "\n";
      std::cerr << ss.str();
      throw std::runtime_error(ss.str().c_str());
    }
}

Extract::Extract(fhicl::ParameterSet const & pset) :
  EDAnalyzer(pset)
{
  std::cerr << "In module ctor\n";
}

Extract::~Extract()
{
}

void Extract::analyze(art::Event const & )
{
  std::cerr << "In module analyze\n";
}

void Extract::beginJob()
{
}

DEFINE_ART_MODULE(Extract)
