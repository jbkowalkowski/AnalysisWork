
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

// LArSoft includes
#include "SimpleTypesAndConstants/geo_types.h"
#include "SimpleTypesAndConstants/RawTypes.h" // raw::ChannelID_t
#include "Geometry/Geometry.h"
#include "Geometry/CryostatGeo.h"
#include "Geometry/TPCGeo.h"
#include "Geometry/PlaneGeo.h"
#include "Geometry/WireGeo.h"
#include "RecoBase/Wire.h"
#include "RecoBase/Hit.h"
#include "RecoBase/Cluster.h"
#include "RecoBase/Track.h"
#include "RecoBase/Vertex.h"
#include "RecoBase/SpacePoint.h"
#include "Utilities/LArProperties.h"
#include "Utilities/DetectorProperties.h"
#include "Utilities/AssociationUtil.h"
#include "RawData/ExternalTrigger.h"
#include "RawData/RawDigit.h"
#include "RawData/raw.h"
#include "RawDataUtilities/TriggerDigitUtility.h" 
#include "MCCheater/BackTracker.h"
#include "Simulation/SimChannel.h"
#include "Filters/ChannelFilter.h"

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

namespace lariat 
{
   class ParaviewExtract;
}

class lariat::ParaviewExtract : public art::EDAnalyzer 
{
public:
  explicit ParaviewExtract(fhicl::ParameterSet const & p);
  virtual ~ParaviewExtract();
  
  void analyze(art::Event const & e) override;
  void beginJob();
private:
  void processTracks(art::Event const& e, std::string const& label);
  void processSP(art::Event const& e, std::string const& label);

  std::string prefix_;
  std::string po_name_;
  std::string tr_name_;
  std::string sp_name_;
  std::ofstream po_file_;
  std::ofstream tr_file_;
  std::ofstream sp_file_;
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

lariat::ParaviewExtract::ParaviewExtract(fhicl::ParameterSet const & pset) :
  EDAnalyzer(pset),
  prefix_(pset.get< std::string >("file_name", "output_")),
  po_name_(makeName(prefix_,"po")),
  tr_name_(makeName(prefix_,"tr")),
  sp_name_(makeName(prefix_,"sp")),
  ht_name_(makeName(prefix_,"ht")),
  po_file_(po_name_.c_str()),
  tr_file_(tr_name_.c_str()),
  sp_file_(sp_name_.c_str()),
  ht_file_(ht_name_.c_str())
{
  checkFile(po_file_, po_name_);
  checkFile(tr_file_, tr_name_);
  checkFile(sp_file_, sp_name_);

  // headers
  // po = eid type id index x y z dirx diry dirz p
  // tr = eid type id total x y z dirx diry dirz theta phi
  // sp = eid type id x y z errx erry errz
  // hi = eid type tid peaktime chan peakamp sumadc view wire
  tr_file_ << "eid/I,type/I,id/I,index/I,x/F,y/F,z/F,dirx/F,diry/F,dirz/F\n";
}

lariat::ParaviewExtract::~ParaviewExtract()
{
}

void ParaviewExtract::processTracks(art::Event const& e, std::string const& label)
{
  auto tr = e.getValidHandle< std::vector<recob::Track> >(label);
  art::FindMany<recob::Hit> hits_p(tr, e, "pmtrack");
  auto eid = e.event();
  
  for(size_t i=0;i<tr->size();++i)
    {
      auto tid = (*tr)[i].ID();
      
      tr_file_ << eid << ',' << label << ',' << tid
	       <<','<< (*tr)[i].NumberTrajectoryPoints()
	;
      
      std::vector<recob::Hit const*> hits;
      hits_p.get(i,hits);
      
      for(size_t j=0;j<hits.size();++j)
	{
	  ht_file_ << eid <<','<< label <<','<< tid
		   <<','<< hits[j]->PeakTime()
		   <<','<< hits[j]->Channel()
		   <<','<< hits[j]->PeakAmplitude
		   <<','<< hits[j]->SummedADC()
		   <<','<< hits[j]->View()
		   <<','<< hits[j]->WireID()
	    ;
	}

      for(size_t j=0;j<(*tr)[i].NumberTrajectoryPoints();++j)
	{
	  double pos[3];
	  double dir[3];
	  (*tr)[i].LocationAtPoint(j).getXYZ(pos);
	  (*tr)[i].DirectionAtPoint(j).getXYZ(dir);
	  double p = (*tr)[i].MomentumAtPoint(j);
	  
	  po_file_ << eid <<','<< label <<','<< tid <<','<< j
		   <<','<< pos[0] <<','<< pos[1] <<','<< pos[2]
		   <<','<< dir[0] <<','<< dir[1] <<','<< dir[2]
		   <<','<< p
	    ;
	}
    }
}

void ParaviewExtract::processSP(art::Event const& e, std::string const& label)
{
  auto sp = e.getValidHandle< std::vector<recob::SpacePointk> >(label);
  auto eid = e.event();

  for( auto const& it:sp )
    {
      const double* pos = sp.XYZ();
      const double* err = sp.ErrXYZ();
      
      sp_file_ << eid <<','<< label <<','<< sp.ID()
	       <<','<< pos[0] <<','<< pos[1] <<','<< pos[2]
	       <<','<< err[0] <<','<< err[1] <<','<< err[2]
	;
    }
}

void lariat::ParaviewExtract::analyze(art::Event const & e)
{
#if 0
   rdu::TriggerDigitUtility tdu(evt, fTriggerUtility);
   art::ServiceHandle<geo::Geometry> geom;
   art::ServiceHandle<util::LArProperties> larprop;
   art::ServiceHandle<util::DetectorProperties> detprop;
   art::ServiceHandle<cheat::BackTracker> bt;
#endif

   processTracks(e,"pmtrack");
   processTracks(e,"cctrack");
   processTracks(e,"costrk");

   processSP(e,"pmtrack");
   processSP(e,"cctrack");
   processSP(e,"costrk");
}

void lariat::ParaviewExtract::beginJob()
{
}

DEFINE_ART_MODULE(lariat::ParaviewExtract)
