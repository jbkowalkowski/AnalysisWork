#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkProperty.h>
#include <vtkXMLImageDataWriter.h>
#include <vtkXMLImageDataReader.h>
#include <vtkImageData.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkImageDataGeometryFilter.h>
#include <vtkRenderWindow.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>

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
  string filename_out_uv = filename_out + "_UV.vti";
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
      short val;
      while(!(iline>>val).eof()) wf.adcs.push_back(val);
      wfs.push_back(wf);
    }

  vector<Waveform*> wfsp;
  for(size_t i=0;i<wfs.size();++i) { wfsp.push_back(&wfs[i]); }
#if 0
  sort(&wfsp[0],&wfsp[2400],
       [](Waveform* a, Waveform* b){return a->channel<b->channel;});
  sort(&wfsp[2400],&wfsp[2400+2400],
       [](Waveform* a, Waveform* b){return a->channel<b->channel;});
  sort(&wfsp[2400+2400],&wfsp[wfsp.size()],
       [](Waveform* a, Waveform* b){return a->channel<b->channel;});
#else
  sort(wfsp.begin(), wfsp.end(),
       [](Waveform* a, Waveform* b)
        {return (a->plane < b->plane)?true:
           (a->plane > b->plane)?false:
             (a->wireindex < b->wireindex)?true:
               false;}
       );
#endif

  // dim_planes_axis
  size_t dim_UV_y = 2400, dim_Y_y = 3456;
  size_t dim_UVY_x = wfs[0].adcs.size();

  vtkSmartPointer<vtkImageData> imageData_uv =
    vtkSmartPointer<vtkImageData>::New();
  vtkSmartPointer<vtkImageData> imageData_y =
    vtkSmartPointer<vtkImageData>::New();

  // only two dimensions: (whick sample, which_wire)
  imageData_uv->SetDimensions(dim_UVY_x,dim_UV_y,2);
  imageData_uv->AllocateScalars(VTK_SHORT, 1);
  imageData_y->SetDimensions(dim_UVY_x,dim_Y_y,1);
  imageData_y->AllocateScalars(VTK_SHORT, 1);

  int* dims = imageData_uv->GetDimensions();

  for (int wire = 0; wire < dims[1];++wire)
    {
      Waveform& wf_u = *(wfsp[wire]);
      Waveform& wf_v = *(wfsp[wire+dim_UV_y]);
      for (int sample = 0; sample < dims[0];++sample)
	{
	  short* pixel_u=
	    static_cast<short*>(imageData_uv->GetScalarPointer(sample,wire,0));
	  short* pixel_v=
	    static_cast<short*>(imageData_uv->GetScalarPointer(sample,wire,1));
	  *pixel_u = wf_u.adcs[sample];
	  *pixel_v = wf_v.adcs[sample];
      }
    }

  dims = imageData_y->GetDimensions();
  for(int wire=0;wire<dims[1];++wire)
    {
      Waveform& wf_y = *(wfsp[wire+dim_UV_y*2]);
      for (int sample = 0; sample < dims[0];++sample)
	{
	  short* pixel_y=
	    static_cast<short*>(imageData_y->GetScalarPointer(sample,wire,0));
	  *pixel_y = wf_y.adcs[sample];
      }
    }
  
  vtkSmartPointer<vtkXMLImageDataWriter> writer =
    vtkSmartPointer<vtkXMLImageDataWriter>::New();

  writer->SetFileName(filename_out_uv.c_str());
  writer->SetInputData(imageData_uv);
  writer->Write();

  writer->SetFileName(filename_out_y.c_str());
  writer->SetInputData(imageData_y);
  writer->Write();
  return 0;
}
