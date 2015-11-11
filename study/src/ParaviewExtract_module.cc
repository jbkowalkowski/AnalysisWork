
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
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "Geometry/Geometry.h"
#include "Filters/ChannelFilter.h"
#pragma GCC diagnostic pop
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
// #include "Utilities/DetectorProperties.h"
#include "Utilities/AssociationUtil.h"
#include "RawData/ExternalTrigger.h"
#include "RawData/RawDigit.h"
#include "RawData/OpDetPulse.h"
#include "RawData/raw.h"
// #include "RawDataUtilities/TriggerDigitUtility.h" 
// #include "MCCheater/BackTracker.h"
#include "Simulation/SimChannel.h"

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

typedef std::vector<std::string> Strings;

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
  void processDigits(art::Event const& e, std::string const& label);

  std::string prefix_;
  Strings write_tracks_;
  Strings write_digits_;
  Strings write_hits_;
  Strings write_spacepoints_;
  std::string po_name_;
  std::string tr_name_;
  std::string sp_name_;
  std::string ht_name_;
  std::string di_name_;
  std::string pu_name_;
  std::ofstream po_file_;
  std::ofstream tr_file_;
  std::ofstream sp_file_;
  std::ofstream ht_file_;
  std::ofstream di_file_;
  std::ofstream pu_file_;
  geo::Geometry* geom_;
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
  write_tracks_(pset.get<Strings>("write_tracks", Strings())),
  write_digits_(pset.get<Strings>("write_digits", Strings())),
  write_hits_(pset.get<Strings>("write_hits", Strings())),
  write_spacepoints_(pset.get<Strings>("write_spacepoints", Strings())),
  po_name_(makeName(prefix_,"po")),
  tr_name_(makeName(prefix_,"tr")),
  sp_name_(makeName(prefix_,"sp")),
  ht_name_(makeName(prefix_,"ht")),
  di_name_(makeName(prefix_,"di")),
  pu_name_(makeName(prefix_,"pu")),
  po_file_(po_name_.c_str()),
  tr_file_(tr_name_.c_str()),
  sp_file_(sp_name_.c_str()),
  ht_file_(ht_name_.c_str()),
  di_file_(di_name_.c_str()),
  pu_file_(pu_name_.c_str()),
  geom_()
{
  std::cerr << "In module ctor\n";
  checkFile(po_file_, po_name_);
  checkFile(tr_file_, tr_name_);
  checkFile(sp_file_, sp_name_);
  checkFile(ht_file_, ht_name_);
  checkFile(di_file_, di_name_);
  checkFile(pu_file_, pu_name_);

  // headers
  // po = eid type id index x y z dirx diry dirz p
  // tr = eid type id total x y z dirx diry dirz theta phi
  // sp = eid type id x y z errx erry errz
  // hi = eid type tid peaktime chan peakamp sumadc view wire
  po_file_ << "eid/I,type/S,id/I,index/I,x/F,y/F,z/F,dirx/F,diry/F,dirz/F,p/F\n";
  tr_file_ << "eid/I,type/S,id/I,total/I\n";
  sp_file_ << "eid/I,type/S,id/I,x/F,y/F,z/F,errx/F,erry/F,errz/F\n";
  ht_file_ << "eid/I,type/S,id/I,peaktime/F,chan/F,peakamp/F,sumadc/F,view/I,wire/S\n";
  // add di and pu
  di_file_ << "eid/I,type/S,channel/I,plane/I,wireindex/I,wiresz/I,samples/I,ped/F,sigma/F\n";
  pu_file_ << "eid/I,type/S,channel/I,samples/I,pmtframe/I\n";

  art::ServiceHandle<geo::Geometry> geom;
  geom_ = &(*geom);
}

lariat::ParaviewExtract::~ParaviewExtract()
{
}

void lariat::ParaviewExtract::processTracks(art::Event const& e, std::string const& label)
{
  auto tr = e.getValidHandle< std::vector<recob::Track> >(label);
  art::FindMany<recob::Hit> hits_p(tr, e, label); // "pmtrack");
  auto eid = e.event();
  
  for(size_t i=0;i<tr->size();++i)
    {
      auto tid = (*tr)[i].ID();
      
      tr_file_ << eid << ',' << label << ',' << tid
	       <<','<< (*tr)[i].NumberTrajectoryPoints()
	       <<"\n";
      
      if(!write_hits_.empty())
	{
      std::vector<recob::Hit const*> hits;
      hits_p.get(i,hits);
      
      for(size_t j=0;j<hits.size();++j)
	{
	  ht_file_ << eid <<','<< label <<','<< tid
		   <<','<< hits[j]->PeakTime()
		   <<','<< hits[j]->Channel()
		   <<','<< hits[j]->PeakAmplitude()
		   <<','<< hits[j]->SummedADC()
		   <<','<< hits[j]->View()
		   <<','<< hits[j]->WireID()
	    <<"\n";
	}
	}

    std::cerr << "at track processing position " << i << "\n";
      for(size_t j=0;j<(*tr)[i].NumberTrajectoryPoints();++j)
	{
      std::cerr << "at traj processing position " << j << "\n";
	  double pos[3];
	  double dir[3];
	  (*tr)[i].LocationAtPoint(j).GetXYZ(pos);
	  (*tr)[i].DirectionAtPoint(j).GetXYZ(dir);
	  double p = 0.0; // (*tr)[i].MomentumAtPoint(j);
	  
	  po_file_ << eid <<','<< label <<','<< tid <<','<< j
		   <<','<< pos[0] <<','<< pos[1] <<','<< pos[2]
		   <<','<< dir[0] <<','<< dir[1] <<','<< dir[2]
		   <<','<< p
	    <<"\n";
	}
    }
}

void lariat::ParaviewExtract::processSP(art::Event const& e, std::string const& label)
{
  auto sp = e.getValidHandle< std::vector<recob::SpacePoint> >(label);
  auto eid = e.event();

  for( auto const& it:(*sp) )
    {
      const double* pos = it.XYZ();
      const double* err = it.ErrXYZ();
      
      sp_file_ << eid <<','<< label <<','<< it.ID()
	       <<','<< pos[0] <<','<< pos[1] <<','<< pos[2]
	       <<','<< err[0] <<','<< err[1] <<','<< err[2]
	<<"\n";
    }
}

void lariat::ParaviewExtract::processDigits(art::Event const& e, std::string const& label)
{
  auto digits = e.getValidHandle< std::vector<raw::RawDigit>>(label);
  auto eid = e.event();

  for( auto const& it:(*digits) )
    {
      std::vector<geo::WireID> wireids = geom_->ChannelToWire(it.Channel());
      auto plane_id = wireids[0].planeID();
      auto wire_idx = wireids[0].Wire;

      di_file_ << eid <<','<< label <<','<< it.Channel() 
               <<','<< plane_id.Plane <<','<< wire_idx <<','<< wireids.size()
	       <<','<< it.Samples() <<','<< it.GetPedestal() <<','<< it.GetSigma()
	;

      for(size_t i=0;i<it.Samples();++i)
	{
	  di_file_ <<','<< it.ADC(i);
	}
      di_file_ << "\n";
    }

#if 0
       std::vector<geo::WireID> wireids = geo->ChannelToWire(channel);
       for(auto const& wid : wireids){
         // check that the plane and tpc are the correct ones to draw
         if (wid.planeID() != pid) continue;
#endif

#if 0
  auto pulses = e.getValidHandle< std::vector<raw::OpDetPulse>>(label);
  for( auto const& it:(*pulses) )
    {
      pu_file_ << eid <<','<< label <<','<< it.OpChannel()
	       <<','<< it.Samples() <<','<< it.PMTFrame() <<','<< it.FirstSample()
      ;

  // cannot gain access to the waveform (const)
      std::vector<short> wf = it.Waveform(); 
      for( auto const& it:(*wf) )
	{
	  pu_file_ <<','<< wf;
	}
      pu_file_ << "\n";
    }
#endif
}

void lariat::ParaviewExtract::analyze(art::Event const & e)
{
#if 0
   rdu::TriggerDigitUtility tdu(evt, fTriggerUtility);
   art::ServiceHandle<util::LArProperties> larprop;
   art::ServiceHandle<util::DetectorProperties> detprop;
   art::ServiceHandle<cheat::BackTracker> bt;
#endif

  std::cerr << "In module analyze\n";
  
  if(!write_tracks_.empty())
    {
      for(auto const& lab : write_tracks_)
	processTracks(e,lab);

      //processTracks(e,"pmtrack");
      //processTracks(e,"cctrack");
      //processTracks(e,"costrk");
    }
  
  std::cerr << "after module analyze processTracks\n";
  
  if(!write_spacepoints_.empty())
    {
      for(auto const& lab : write_spacepoints_)
	processSP(e,lab);
      
      //processSP(e,"pmtrack");
      // processSP(e,"cctrack"); // no such thing
      //processSP(e,"costrk");
    }

  if(!write_digits_.empty())
    {
      for(auto const& lab : write_digits_)
	processDigits(e,lab);
      //processDigits(e,"daq");
      // processDigits(e,"SlicerInput");
    }
}

void lariat::ParaviewExtract::beginJob()
{
}

DEFINE_ART_MODULE(lariat::ParaviewExtract)
