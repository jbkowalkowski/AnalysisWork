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
    std::nth_element(samp.begin(),samp.begin()+half, samp.end());
    median=samp[half];
    low= (half==0)?median:samp[half-1] - median;
    high= (half==samp.size()-1)?median:samp[half+1] - median;
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
  size_t dim_UV = 2400, dim_Y = 3456;
  size_t dim_S = wfs[0].adcs.size()/4;

  vtkSmartPointer<vtkImageData> imageData_u =
    vtkSmartPointer<vtkImageData>::New();
  vtkSmartPointer<vtkImageData> imageData_v =
    vtkSmartPointer<vtkImageData>::New();
  vtkSmartPointer<vtkImageData> imageData_y =
    vtkSmartPointer<vtkImageData>::New();

  // only two dimensions: (whick sample, which_wire)
  imageData_u->SetDimensions(dim_UV,dim_S,1);
  imageData_v->SetDimensions(dim_UV,dim_S,1);
  imageData_y->SetDimensions(dim_Y,dim_S,1);
  //imageData_u->SetDimensions(dim_UV+1,dim_S+1,2);
  //imageData_v->SetDimensions(dim_UV+1,dim_S+1,2);
  //imageData_y->SetDimensions(dim_Y+1,dim_S+1,2);

  vtkSmartPointer<vtkShortArray> energy_u =
    vtkSmartPointer<vtkShortArray>::New();
  energy_u->SetName("energy");
  energy_u->Resize(dim_UV*dim_S);
  energy_u->SetNumberOfValues(dim_UV*dim_S);

  vtkSmartPointer<vtkShortArray> energy_v = 
    vtkSmartPointer<vtkShortArray>::New();
  energy_v->SetName("energy");
  energy_v->Resize(dim_UV*dim_S);
  energy_v->SetNumberOfValues(dim_UV*dim_S);

  vtkSmartPointer<vtkShortArray> energy_y = 
    vtkSmartPointer<vtkShortArray>::New();
  energy_y->SetName("energy");
  energy_y->Resize(dim_Y*dim_S);
  energy_y->SetNumberOfValues(dim_Y*dim_S);

  // (i,j,k) idx_flat=k*(nptsx*nptsy)+j*nptrx+i

  auto flat_index_uv = [&](int i,int j, int k)
    {
      auto a = k*(dim_UV*dim_S)+j*dim_UV+i;
      //cout << "y index " << a << "\n";
      return a;
    };
  auto flat_index_y = [&](int i,int j, int k)
    {
      auto a = k*(dim_Y*dim_S)+j*dim_Y+i;
      //cout << "x index " << a << "\n";
      return a;
    };
  
  int* dims = imageData_u->GetDimensions();
  cout << "dim uv = " << dims[0] << " " << dims[1] << " " << dims[2] << "\n";

  for (size_t wire = 0; wire < dim_UV;++wire)
    {
      Waveform& wf_u = *(wfsp[wire]);
      Waveform& wf_v = *(wfsp[wire+dim_UV]);

      for (size_t sample = 0; sample < dim_S;++sample)
	{
	  short au = wf_u.adcs[sample] - wf_u.median;
	  short av = wf_v.adcs[sample] - wf_v.median;
	  au=(au<wf_u.high && au>wf_u.low)?0:au;
	  av=(av<wf_v.high && av>wf_v.low)?0:av;
	  
	  auto u = flat_index_uv(wire,sample,0);
	  energy_u->SetTupleValue(u,&au);
	  auto v = flat_index_uv(wire,sample,0);
	  energy_v->SetTupleValue(v,&av);
      }
    }

  dims = imageData_y->GetDimensions();
  cout << "dim y = " << dims[0] << " " << dims[1] << " " << dims[2] << "\n";

  for(size_t wire=0;wire<dim_Y;++wire)
    {
      Waveform& wf_y = *(wfsp[wire+dim_UV*2]);

      for (size_t sample = 0; sample < dim_S;++sample)
	{
	  short ay = wf_y.adcs[sample] - wf_y.median;
	  ay=(ay<wf_y.high && ay>wf_y.low)?0:ay;

	  auto y = flat_index_y(wire,sample,0);
	  energy_y->SetTupleValue(y,&ay);
      }
    }

  auto u_cells = imageData_u->GetPointData();
  auto v_cells = imageData_v->GetPointData();
  auto y_cells = imageData_y->GetPointData();

  u_cells->AddArray(energy_u);
  v_cells->AddArray(energy_v);
  y_cells->AddArray(energy_y);

  std::cout << "u Points: " << imageData_u->GetNumberOfPoints() << "\n";
  std::cout << "u Cells: " << imageData_u->GetNumberOfCells() << "\n";
  std::cout << "y Points: " << imageData_y->GetNumberOfPoints() << "\n";
  std::cout << "y Cells: " << imageData_y->GetNumberOfCells() << "\n";
  
  vtkSmartPointer<vtkXMLImageDataWriter> writer =
    vtkSmartPointer<vtkXMLImageDataWriter>::New();

  writer->SetFileName(filename_out_u.c_str());
  writer->SetInputData(imageData_u);
  writer->Write();

  writer->SetFileName(filename_out_v.c_str());
  writer->SetInputData(imageData_v);
  writer->Write();

  writer->SetFileName(filename_out_y.c_str());
  writer->SetInputData(imageData_y);
  writer->Write();
  return 0;
}
