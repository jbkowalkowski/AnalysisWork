#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkProperty.h>
#include <vtkXMLImageDataWriter.h>
#include <vtkXMLImageDataReader.h>
#include <vtkRectilinearGridWriter.h>
#include <vtkRectilinearGridReader.h>
#include <vtkImageData.h>
#include <vtkRectilinearGrid.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkImageDataGeometryFilter.h>
#include <vtkRenderWindow.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkShortArray.h>
#include <vtkFloatArray.h>
#include <vtkDoubleArray.h>
#include <vtkPointData.h>
#include <vtkCellData.h>

#include <fstream>
#include <sstream>
#include <algorithm>
#include <typeinfo>

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
  string filename_out_x = filename_out + "_X.vti";
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
  size_t dim_xyz = 257;

  vtkSmartPointer<vtkImageData> grid_x =
    vtkSmartPointer<vtkImageData>::New();

  vtkSmartPointer<vtkImageData> grid_y =
    vtkSmartPointer<vtkImageData>::New();

  // only two dimensions: (whick sample, which_wire)
  grid_x->SetDimensions(dim_xyz,2,dim_xyz);
  grid_y->SetDimensions(2,dim_xyz,dim_xyz);
  
  // (i,j,k) idx_flat=k*(nptsx*nptsy)+j*nptrx+i

  vtkSmartPointer<vtkFloatArray> energy_x =
    vtkSmartPointer<vtkFloatArray>::New();
  energy_x->SetName("energy");
  energy_x->Resize(256*256);
  energy_x->SetNumberOfValues(256*256);

  vtkSmartPointer<vtkFloatArray> energy_y = 
    vtkSmartPointer<vtkFloatArray>::New();
  energy_y->SetName("energy");
  energy_y->Resize(256*256);
  energy_y->SetNumberOfValues(256*256);

#if 1
  auto flat_index_y = [&](int i,int j, int k)
    {
      auto a = k*(1*256)+j*1+i;
      cout << "y index " << a << "\n";
      return a;
    };
  auto flat_index_x = [&](int i,int j, int k)
    {
      auto a = k*(256*1)+j*256+i;
      cout << "x index " << a << "\n";
      return a;
    };

  for(size_t i=0;i<wfs.size();++i)
    {
      float tmp = wfs[i].energy+1.0;
      if(i%2==1)
	{
	  auto u=flat_index_x(wfs[i].x,0,wfs[i].z);
	  energy_x->SetTupleValue(u,&tmp);
	  auto v=flat_index_x(wfs[i].x,1,wfs[i].z);
	  energy_x->SetTupleValue(v,&tmp);
	}
      else
	{
	  auto u=flat_index_y(0,wfs[i].y,wfs[i].z);
	  energy_y->SetTupleValue(u,&tmp);
	  auto v=flat_index_y(1,wfs[i].y,wfs[i].z);
	  energy_y->SetTupleValue(v,&tmp);
	}
    }

  cout << "y cell type = " << grid_x->GetCellType(flat_index_y(1,4,5)) << "\n";
  cout << "x cell type = " << grid_y->GetCellType(flat_index_x(1,4,5)) << "\n";
#endif

  auto x_cells = grid_x->GetCellData();
  auto y_cells = grid_y->GetCellData();

  x_cells->AddArray(energy_x);
  y_cells->AddArray(energy_y);

  std::cout << "Points: " << grid_x->GetNumberOfPoints() << "\n";
  std::cout << "Cells: " << grid_x->GetNumberOfCells() << "\n";
  
  vtkSmartPointer<vtkXMLImageDataWriter> writer =
    vtkSmartPointer<vtkXMLImageDataWriter>::New();

  writer->SetFileName(filename_out_x.c_str());
  writer->SetInputData(grid_x);
  writer->Write();

  writer->SetFileName(filename_out_y.c_str());
  writer->SetInputData(grid_y);
  writer->Write();
  return 0;
}
