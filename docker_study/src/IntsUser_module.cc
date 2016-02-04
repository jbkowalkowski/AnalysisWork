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
  class IntsUser;
}

using study::IntsUser;

class study::IntsUser : public art::EDAnalyzer
{
public:
  using Parameters = EDAnalyzer::Table<Config>;

  explicit IntsUser( EDAnalyzer::Table<Config> const& p )
    : EDAnalyzer(p), label_( p().label() ), sum_(0)
  { 
  }

  virtual ~IntsUser() { }
  virtual void analyze( art::Event const& e );

private:
  std::string label_;
  size_t sum_;
};

void IntsUser::analyze( art::Event const& e )
{
  auto p = e.getValidHandle< IntProducts >(label_);
  for(auto const& it:*p) sum_+=it.value;
}

DEFINE_ART_MODULE(IntsUser)
