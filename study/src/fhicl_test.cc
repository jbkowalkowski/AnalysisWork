
#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/intermediate_table.h"
#include "fhiclcpp/make_ParameterSet.h"
#include "fhiclcpp/parse.h"

#include <array>
#include <iomanip>
#include <iostream>
#include <cstdlib>
#include <string>
#include <tuple>
#include <typeinfo>
#include <vector>
#include <utility>

using namespace fhicl;

struct Thing
{
 Thing() { }
 
};

int main(int argc, char* argv[])
{
  if(argc < 2)
  {
    std::cerr << "usage: " << argv[0] << " fcl_file\n";
    return -1;
  }

  std::string fname = argv[1];

  cet::filepath_lookup policy("FHICL_FILE_PATH");
  fhicl::intermediate_table tbl1;
  fhicl::parse_document(fname, policy, tbl1);
  fhicl::ParameterSet pset;
  fhicl::make_ParameterSet(tbl1, pset);

  auto const vofp = pset.get<std::vector<std::pair<std::string,unsigned> > >("pair2");

  for(auto const& it : vofp)
  {
    std::cout << it.first << ',' << it.second << "\n";
  }
  return 0;
}
