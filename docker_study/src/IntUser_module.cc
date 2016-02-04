//--------------------------------------------------------------------
//
// Produces an IntProduct instance.
//
//--------------------------------------------------------------------

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "art/Framework/Principal/Handle.h"
#include "fhiclcpp/types/Atom.h"

#include "SimpleProducts.h"

#include <iostream>
#include <memory>

namespace {

  using namespace fhicl;
  struct Config {
    Atom<std::string> label { Name("label") };
  };

}

namespace study {
  class IntUser;
}

using study::IntUser;

class study::IntUser : public art::EDAnalyzer
{
public:
  using Parameters = EDAnalyzer::Table<Config>;

  explicit IntUser( EDAnalyzer::Table<Config> const& p ):
       EDAnalyzer(p)
     , label_( p().label() )
     ,  sum_(0)
  { 
  }

  virtual ~IntUser() { }
  virtual void analyze( art::Event const& e );

private:
  std::string label_;
  size_t sum_;
};

void IntUser::analyze( art::Event const& e )
{
  auto p = e.getValidHandle< IntProduct >(label_);
  sum_ += p->value;
}

DEFINE_ART_MODULE(IntUser)
