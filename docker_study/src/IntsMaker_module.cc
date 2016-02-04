//--------------------------------------------------------------------
//
// Produces an IntProduct instance.
//
//--------------------------------------------------------------------

#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
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
    Atom<int> ivalue { Name("start_value") };
    Atom<size_t> total { Name("total") };
  };

}

namespace study {
  class IntsMaker;
}

using study::IntsMaker;

class study::IntsMaker : public art::EDProducer
{
public:
  using Parameters = EDProducer::Table<Config>;

  explicit IntsMaker( EDProducer::Table<Config> const& p )
    : value_( p().ivalue() )
    , total_( p().total() )
  {
    produces<IntProducts>();
  }

  explicit IntsMaker( int i ) : value_(i), total_(1)
  {
    produces<IntProducts>();
  }

  virtual ~IntsMaker() { }
  virtual void produce( art::Event& e );

private:
  int value_;
  size_t total_;
};

void IntsMaker::produce( art::Event& e )
{
  auto p = std::make_unique<IntProducts>( total_ );

  for(size_t i=0; i<total_; ++i) (*p)[i].value=i+value_;
  e.put(std::move(p));
  ++value_;
}

DEFINE_ART_MODULE(IntsMaker)
