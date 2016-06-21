
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

using namespace std;

namespace study {
    class ExtractWaveforms;
}

struct Waveform {

    Waveform() :
    type("NONE"), channel(0), plane(0), wireindex(0), wiresz(0), eid(0), ped(0.0) {
        adcs.reserve(10000);
    }

    Waveform(string const& t, int c, int pl, int wi, int sz, int e, float p) :
    type(t), channel(c), plane(pl), wireindex(wi), wiresz(sz), eid(e), ped(p) {
        adcs.reserve(10000);
    }

    Waveform(istream& ist) :
    type("NONE"), channel(0), plane(0), wireindex(0), wiresz(0), eid(0), ped(0.0) {
        float sigma;
        int samples;
        adcs.reserve(10000);
        ist >> eid >> type >> channel >> plane >> wireindex >> wiresz >> samples >> ped >> sigma;

        short val;
        while (!(ist >> val).eof()) adcs.push_back(val);

    }

    string type;
    int channel;
    int plane;
    int wireindex;
    int wiresz;
    int eid;
    float ped;
    vector<short> adcs;
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
  short calc_median(Waveform** wfsp, 
		    size_t dim_S, size_t wire); 
  void gen_points(std::string const& filename,
		  Waveform** wfsp, size_t dim_S, size_t dim);
    
  bool as_mask_;
  std::string prefix_;
  Strings write_tracks_;
  Strings write_digits_;
  Strings write_hits_;
  Strings write_spacepoints_;
  std::string filename_out_u;
  std::string filename_out_v;
  std::string filename_out_y;
  std::string filename_out_tr;
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
  write_tracks_(pset.get<Strings>("write_tracks", Strings())),
  write_digits_(pset.get<Strings>("write_digits", Strings())),
  write_hits_(pset.get<Strings>("write_hits", Strings())),
  write_spacepoints_(pset.get<Strings>("write_spacepoints", Strings())),
  filename_out_u(prefix_ + "U.csv"),
  filename_out_v(prefix_ + "V.csv"),
  filename_out_y(prefix_ + "Y.csv"),
  filename_out_tr(prefix_ + "tracks.csv"),
  geom_()
{
  std::cerr << "In module ctor\n";
  art::ServiceHandle<geo::Geometry> geom;
  geom_ = &(*geom);
}

study::ExtractWaveforms::~ExtractWaveforms() { }

short study::ExtractWaveforms::calc_median(Waveform** wfsp, size_t dim_S, size_t wire)
{
  // const double get_rid = .49;
  std::vector<short> all_plane;
  all_plane.reserve(3500 * 10000);
  cout << "generating median" << std::endl;
  
  Waveform& wfa = *(wfsp[wire]);
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

void study::ExtractWaveforms::processTracks(art::Event const& e, 
					    std::string const& label, 
					    int const& /*id*/)
{
  auto tr = e.getValidHandle< std::vector<recob::Track> >(label); //* to array of tracks
  art::FindMany<recob::Hit> hits_p(tr, e, label); // "pmtrack");
  //auto eid = e.event();

  std::cout << "Processing tracks for algorithm " << label << "\n";
  
  //Attributes
  //do we need the evend id too?
  //all the points in this file will be from the same event anyways
  //set the expected size for these attribute arrays?
  //don't know what to call to find that.
  
  for (size_t i = 0; i < tr->size(); ++i)
    { //for each track
      // auto tid = (*tr)[i].ID(); //track ID
      
      if (!write_hits_.empty())
	{
	  std::vector<recob::Hit const*> hits;
	  hits_p.get(i, hits);
	  for (size_t j = 0; j < hits.size(); ++j)
	    { 
	      // for each hit algorithm
	      // hit data output
	    }
	}
      
      size_t numTrajPoints = (*tr)[i].NumberTrajectoryPoints();
      std::cerr << "at track processing position " << i << "\n";
      
      for (size_t j = 0; j < numTrajPoints; ++j) 
	{ 
	  // for each point on track
	  // std::cerr << "at traj processing position " << j << "\n";
	  double pos[3];
	  double dir[3];
	  (*tr)[i].LocationAtPoint(j).GetXYZ(pos);
	  (*tr)[i].DirectionAtPoint(j).GetXYZ(dir);
	  // double p = (*tr)[i].MomentumAtPoint(j);
          
	  //add the point to "points" and the id to the PolyLine
	  // variable list
	  // label, id, tid, j, dir, p            
	}
    }       
}

void study::ExtractWaveforms::processMC(art::Event const& e, std::string const& label)
{
  auto mct = e.getValidHandle< std::vector<simb::MCTruth> >(label);
  auto eid = e.event();

  cout << "In processMC, " << eid << " " << mct->size() << std::endl;

  for(auto it=mct->begin();it!=mct->end();++it)
    {
      cout << "number of particles = " << it->NParticles() << std::endl;
      for(auto i = 0;i<it->NParticles();++i)
	{
	  simb::MCParticle const& mcp = it->GetParticle(i);
	  processParticle(mcp);
	}
    }
}

void study::ExtractWaveforms::processMCPart(art::Event const& e, std::string const& label)
{
  auto mct = e.getValidHandle< std::vector<simb::MCParticle> >(label);
  auto eid = e.event();

  cout << "In processMCPart, " << eid << " " << mct->size() << std::endl;

  for(auto it=mct->begin();it!=mct->end();++it)
    {
      processParticle(*it);
    }
}

void study::ExtractWaveforms::processParticle(simb::MCParticle const& mcp)
{
  simb::MCTrajectory const& mctraj = mcp.Trajectory();
  int pdg = mcp.PdgCode();
  auto trackid = mcp.TrackId();
  cout << "PART " << pdg <<  " " << trackid << " " 
       << mcp.NumberTrajectoryPoints() << " " 
       << mctraj.TotalLength() << " " << mctraj.size() << std::endl;
  
  for(auto j=0UL;j<mctraj.size();++j)
    {
      cout << "TRAJ " <<" "<< pdg <<" "<< trackid <<" "<< j <<" "
	   << mctraj.X(j) <<" "<< mctraj.Y(j) <<" "<< mctraj.Z(j) <<" "<< mctraj.T(j) <<" "
	   << mctraj.Px(j) <<" "<< mctraj.Py(j) <<" "<< mctraj.Pz(j) <<" "<< mctraj.E(j) <<" "
	   << "\n";
    }
}

void study::ExtractWaveforms::processSP(art::Event const& e, 
					std::string const& label) 
{
  auto sp = e.getValidHandle< std::vector<recob::SpacePoint> >(label);
  auto eid = e.event();
  
  for (auto const& it : (*sp))
    {
      const double* pos = it.XYZ();
      const double* err = it.ErrXYZ();
      
#if 1
      std::cout << eid << ',' << label << ',' << it.ID()
               << ',' << pos[0] << ',' << pos[1] << ',' << pos[2]
               << ',' << err[0] << ',' << err[1] << ',' << err[2]
               << "\n";
#endif
    }
}

void study::ExtractWaveforms::processDigits(art::Event const& e, 
					    std::string const& label)
{
  auto digits = e.getValidHandle< std::vector < raw::RawDigit >> (label);
  auto eid = e.event();

  Waveforms wfs;
  wfs.reserve(10000);
    
  for (auto const& it : (*digits)) 
    {
      std::vector<geo::WireID> wireids = geom_->ChannelToWire(it.Channel());
      auto plane_id = wireids[0].planeID();
      auto wire_idx = wireids[0].Wire;
      Waveform wf("eid/I,type/S,channel/I,plane/I,wireindex/I,wiresz/I,samples/I,ped/F,sigma/F\n", 
		  it.Channel(),
		  plane_id.Plane, 
		  wire_idx, 
		  wireids.size(), 
		  eid, 
		  it.GetPedestal());
      raw::RawDigit::ADCvector_t uncom(it.Samples());
      raw::Uncompress(it.ADCs(), uncom, it.Compression());

      for (size_t i = 0; i < it.Samples(); ++i) 
	{
	  wf.adcs.push_back(uncom[i]);
        }
      wfs.push_back(wf);
    }
  std::cout << "finished reading points\n";
    
  vector<Waveform*> wfsp;
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

#if 0
    std::vector<geo::WireID> wireids = geo->ChannelToWire(channel);
    for (auto const& wid : wireids)
      {
        // check that the plane and tpc are the correct ones to draw
        if (wid.planeID() != pid) continue;
#endif

#if 0
        auto pulses = e.getValidHandle<std::vector< raw::OpDetPulse >> (label);
        for (auto const& it : (*pulses))
	  {
            pu_file_ << eid << ',' << label << ',' << it.OpChannel()
		     << ',' << it.Samples() << ',' << it.PMTFrame() 
		     << ',' << it.FirstSample()
	      ;

            // cannot gain access to the waveform (const)
            std::vector<short> wf = it.Waveform();
            for (auto const& it : (*wf)) 
	      {
                pu_file_ << ',' << wf;
	      }
            pu_file_ << "\n";
	  }
      }
#endif
}

void study::ExtractWaveforms::gen_points(
					 std::string const& filename,
					 Waveform** wfsp, 
					 size_t dim_S, size_t dim) 
{
#if 0
  auto flat_index = [&](int i, int j, int k) 
    {
      auto a = k * (dim * dim_S) + j * dim_S + i;
      return a;
    };
#endif

  std::cout << "Gen_points called\n";
  // Positions and cells
  // std::cout << "NumberOfValues: " << dim*dim_S << "\n";

  ofstream outf(filename.c_str());

  for (size_t wire = 0; wire < dim; ++wire) 
    {
      short median = as_mask_?calc_median(wfsp, dim_S, wire):0;
      Waveform& wf_u = *(wfsp[wire]);
      // outf << wire << " " ;
      
      for (size_t sample = 0; sample < dim_S; ++sample) 
	{
	  short au = wf_u.adcs[sample] - median;
	  // auto u = flat_index(sample, wire, 0);
	  // energy->SetTupleValue(u, &au);
          outf << au << " ";
        }

      outf << "\n";
    }

  // auto cells = pts->GetPointData();
  //cells->AddArray(energy);  
}

void study::ExtractWaveforms::analyze(art::Event const & e) 
{
#if 0
  rdu::TriggerDigitUtility tdu(evt, fTriggerUtility);
  art::ServiceHandle<util::LArProperties> larprop;
  art::ServiceHandle<util::DetectorProperties> detprop;
  art::ServiceHandle<cheat::BackTracker> bt;
#endif
  
  std::cerr << "In module analyze\n";

  if (!write_tracks_.empty()) 
    {
      int id=0;
      for (auto const& lab : write_tracks_)
	{
	  processTracks(e, lab, id);
	  id++;
	}
      
      //processTracks(e,"pmtrack");
      //processTracks(e,"cctrack");
      //processTracks(e,"costrk");
    }

  std::cerr << "after module analyze processTracks\n";

  if (!write_spacepoints_.empty()) 
    {
      // for (auto const& lab : write_spacepoints_)
      //     processSP(e, lab);

      // processSP(e,"pmtrack");
      // processSP(e,"cctrack"); // no such thing
      // processSP(e,"costrk");
    }
  
  if (!write_digits_.empty()) 
    {
      for (auto const& lab : write_digits_)
	processDigits(e, lab);
      // processMC(e,"generator");
      // processMCPart(e,"largeant");
      // processDigits(e,"daq");
      // processDigits(e,"SlicerInput");
    }
}

void study::ExtractWaveforms::beginJob() { }

DEFINE_ART_MODULE(study::ExtractWaveforms)
