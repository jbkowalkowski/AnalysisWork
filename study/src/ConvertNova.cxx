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
    x(0),y(0),z(0),x_err(0.0),y_err(0.0),z_err(0.0),eid(0),type(0),view(0),energy(0.0),score(0.0),class_()
    {}
  Waveform(istream& ist):
    x(0),y(0),z(0),x_err(0.0),y_err(0.0),z_err(0.0),eid(0),type(0),view(0),energy(0.0),score(0.0),class_()
  {
    ist >> x >> y >> z >> x_err >> y_err >> z_err >> eid >> type >> view >> energy >> score >> class_;
  }

  // X/I,Y/I,Z/I,X_err/F,Y_err/F,Z_err/F,EID/L,Type/I,View/I,Energies/F,Score/F,Class/S

  int x,y,z;
  float x_err,y_err,z_err;
  int eid;
  int type;
  int view;
  float energy;
  float score;
  string class_;
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
  string filename_out_y = filename_out + "_Y.vti";

  // read the raw digits in
  std::ifstream fin(filename_in.c_str());
  if(!fin)
    {
      std::cerr << "no input file " << filename_in << "\n";
      return -1;
    }

  Waveforms wfs;
  wfs.reserve(1000);
  std::string header,line;
  getline(fin, header);

  while(!getline(fin,line).eof())
    {
      for(auto& c:line) { if(c==',') c=' '; }
      // split the line up
      istringstream iline(line);
      Waveform wf(iline);
      // short val;
      // while(!(iline>>val).eof()) wf.adcs.push_back(val);
      wfs.push_back(wf);
    }

  // dim_planes_axis
  size_t dim_xyz = 256;

  vtkSmartPointer<vtkImageData> imageData_y =
    vtkSmartPointer<vtkImageData>::New();

  // only two dimensions: (whick sample, which_wire)
  imageData_y->SetDimensions(dim_xyz,dim_xyz,dim_xyz);
  imageData_y->AllocateScalars(VTK_FLOAT, 1);

  int* dims = imageData_y->GetDimensions();
  for(int x=0;x<dims[0];++x)
    for(int y=0;y<dims[1];++y)
      for(int z=0;z<dims[2];++z)
      {
	  float* pixel_y=
	    static_cast<float*>(imageData_y->GetScalarPointer(x,y,z));
	  pixel_y[0] = 0.0;
	  // pixel_y[1] = 0.0;
      }

  auto func = [&](int x,int y,int z, float e) 
  { 
    float* pixel_y=
      static_cast<float*>(imageData_y->GetScalarPointer(x,y,z));
    pixel_y[0]+=e;
  };

  for(auto& it:wfs)
  {
    auto x_max = std::min(it.x+it.x_err,255.0f);
    auto y_max = std::min(it.y+it.y_err,255.0f);
    auto z_max = std::min(it.z+5.5f,255.0f);
    for(size_t ex=it.x-it.x_err;ex<x_max;++ex)
     for(size_t ey=it.y-it.y_err;ey<y_max;++ey)
      for(size_t ez=it.z-5.5f;ez<z_max;++ez) 
        func(ex,ey,ez,it.energy+1.0);
  }

#if 0
  // auto cell_data = imageData_y->GetCellData();

  auto p_cells = imageData_y->GetCellData()->GetScalars("ImageScalars");
  auto p_scalers = imageData_y->GetPointData()->GetScalars("ImageScalars");

  if(!p_scalers)
  {
    cerr << "bad p_scalers, bad\n";
    return -1;
  }
 
  vtkShortArray* sa = vtkShortArray::SafeDownCast(p_scalers);
  // cout << sa->Capacity() << "\n";
  for(auto it=sa->Begin(); it!=(sa->Begin()+20); ++it)
  {
    cout << *it << "\n";
  }
#endif
  
  vtkSmartPointer<vtkXMLImageDataWriter> writer =
    vtkSmartPointer<vtkXMLImageDataWriter>::New();

  writer->SetFileName(filename_out_y.c_str());
  writer->SetInputData(imageData_y);
  writer->Write();
  return 0;
}
