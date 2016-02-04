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
  };

}

namespace study {
  class IntMaker;
}

using study::IntMaker;

class study::IntMaker : public art::EDProducer
{
public:
  using Parameters = EDProducer::Table<Config>;

  explicit IntMaker( EDProducer::Table<Config> const& p )
    : value_( p().ivalue() )
  { 
    produces<IntProduct>();
  }

  explicit IntMaker( int i ) : value_(i)
  {
    produces<IntProduct>();
  }

  virtual ~IntMaker() { }
  virtual void produce( art::Event& e );

private:
  int value_;
};

void IntMaker::produce( art::Event& e )
{
  e.put(std::make_unique<IntProduct>(value_++));
}

DEFINE_ART_MODULE(IntMaker)
