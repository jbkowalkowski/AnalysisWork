#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkProperty.h>
#include <vtkXMLImageDataWriter.h>
#include <vtkXMLImageDataReader.h>
#include <vtkImageData.h>
#include <vtkPolyData.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkImageDataGeometryFilter.h>
#include <vtkRenderWindow.h>
#include <vtkActor.h>
#include <vtkPolyDataWriter.h>
#include <vtkShortArray.h>
#include <vtkPointData.h>
#include <vtkCellData.h>

#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

struct Waveform
{
  Waveform():
    type("NONE"),channel(0),plane(0),wireindex(0),wiresz(0),eid(0),ped(0.0) { adcs.reserve(10000); }
  Waveform(string const& t,int c, int pl, int wi, int sz, int e, float p):
    type(t),channel(c),plane(pl),wireindex(wi),wiresz(sz),eid(e),ped(p) { adcs.reserve(10000); }
  Waveform(istream& ist):
    type("NONE"),channel(0),plane(0),wireindex(0),wiresz(0),eid(0),ped(0.0) 
  {
    float sigma;
    int samples;
    adcs.reserve(10000);
    ist >> eid >> type >> channel >> plane >> wireindex >> wiresz >> samples >> ped >> sigma;

    short val;
    while(!(ist>>val).eof()) adcs.push_back(val);

    vector<short> samp(adcs);
    size_t half=samp.size()/2;
    //cerr << "half=" << half << "\n";
    //std::nth_element(samp.begin(),samp.begin()+half, samp.end());
    std::sort(samp.begin(),samp.end());
    median=samp[half];

    const size_t dist = samp.size()*.49;
    low=  samp[half-dist] - median;
    high= samp[half+dist] - median;
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
  short high,low;
};
typedef vector<Waveform> Waveforms;

void gen_points(vtkSmartPointer<vtkXMLPolyDataWriter>& writer,
		std::string const& filename,
		Waveform* wfsp, size_t dim_S, size_t dim)
{
  
  // Positions and cells
  vtkSmartPointer<vtkPoints> pts =
    vtkSmartPointer<vtkPoints>::New();
  vtkSmartPointer<vtkCellArray> verts =
    vtkSmartPointer<vtkCellArray>::New();
  vtkSmartPointer<vtkShortArray> energy =
    vtkSmartPointer<vtkShortArray>::New();

  energy->SetName("energy");
  energy->Resize(dim*dim_S);
  energy->SetNumberOfValues(dim*dim_S);

  for (size_t wire = 0; wire < dim;++wire)
    {
      Waveform& wf_u = *(wfsp[wire]);

      for (size_t sample = 0; sample < dim_S;++sample)
	{
	  short au = wf_u.adcs[sample];
	  pts->InsertNextPoint(wire,sample,0);
	  vtkIdType pid[1];
	  verts->InsertNextCell(1,pid);
	  energy->InsertNextValue(au);	    
	}
    }

  // Create a polydata
  vtkSmartPointer<vtkPolyData> polydata = 
    vtkSmartPointer<vtkPolyData>::New();
  polydata->SetPoints ( pts );
  polydata->SetVerts ( verts );
  polydata->GetPointData()->AddArray(energy);

  writer->SetFileName(filename.c_str());
  writer->SetInputData(polydata);
  writer->Write();

}


int main(int argc, char *argv[])
{
  // Parse command line arguments
  if(argc < 3)
    {
    std::cerr << "Required arguments: infile.csv outfile" << std::endl;
    return -1;
    }

  std::string filename_in = argv[1];
  std::string filename_out = argv[2];
  string filename_out_u = filename_out + "_U.vti";
  string filename_out_v = filename_out + "_V.vti";
  string filename_out_y = filename_out + "_Y.vti";

  // read the raw digits in
  std::ifstream fin(filename_in.c_str());
  if(!fin)
    {
      std::cerr << "no input file " << filename_in << "\n";
      return -1;
    }

  /*
The wires in the “Y” plane are oriented vertically, while wires in the
“U” and “V” planes are oriented ±60 degrees, respectively, with
respect to vertical. Ionization electrons drift from beam-left to
beam-right, reaching in turn the U, V, and Y planes. The drifting
electrons induce charge on the U and V induction plane sense wires as
they drift by and deposit their negative charge on the Y collection
plane sense wires.
   */

  /*
Three readout planes, spaced by 3 mm, form the beam-right side of the
detector, with the 3456 Y wires arrayed vertically and the 2400 U and
2400 V wires oriented at ± 60 degrees with respect to the Y wires. All
planes have a wire separation of 3 mm within the plane. Bias voltages
placed on the three wire planes allow all electrons pass through the U
and V planes to be collected by the Y plane.
   */

  Waveforms wfs;
  wfs.reserve(10000);
  std::string header,line;
  // eid/I,type/S,channel/I,plane/I,wireindex/I,wiresz/I,samples/I,ped/F,sigma/F
  getline(fin, header);
  while(!getline(fin,line).eof())
    {
      for(auto& c:line) { if(c==',') c=' '; }
      // split the line up
      istringstream iline(line);
      Waveform wf(iline);
      wfs.push_back(wf);
    }

  vector<Waveform*> wfsp;
  for(size_t i=0;i<wfs.size();++i) { wfsp.push_back(&wfs[i]); }
  sort(wfsp.begin(), wfsp.end(),
       [](Waveform* a, Waveform* b)
        {return (a->plane < b->plane)?true:
           (a->plane > b->plane)?false:
             (a->wireindex < b->wireindex)?true:
               false;}
       );

  // dim_planes_axis
  size_t dim_UV = 2400, dim_Y = 3456;
  size_t dim_S = wfs[0].adcs.size(); // /4;

  vtkSmartPointer<vtkXMLPolyDataWriter> writer =
    vtkSmartPointer<vtkXMLPolyDataWriter>::New();
  
  // first u, then v, then y
  gen_points(writer, filename_out_u, &wfsp[0], dim_S, dim_UV);
  gen_points(writer, filename_out_v, &wfsp[dim_UV], dim_S, dim_UV);
  gen_points(writer, filename_out_y, &wfsp[dim_UV*2], dim_S, dim_Y);

  return 0;
}
