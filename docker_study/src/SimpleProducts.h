#ifndef test_TestObjects_ToyProducts_h
#define test_TestObjects_ToyProducts_h

// ======================================================================
//
// EDProducts for testing purposes
//
// ======================================================================

#include "art/Persistency/Common/traits.h"
#include "cetlib/container_algorithms.h"

#ifndef __GCCXML__
#  include <cstdint>
#else
#  include "boost/cstdint.hpp"
#endif

#include <stdexcept>
#include <string>
#include <vector>

namespace study
{

  struct IntProduct
  {
    explicit IntProduct(int i=0) : value(i) { }
    ~IntProduct() { }

    IntProduct &operator+= (IntProduct const &other)
    { value += other.value; return *this; }

    int value;
  };
  typedef std::vector<IntProduct> IntProducts;

  struct DoubleProduct
  {
    explicit DoubleProduct(double d=2.2) : value(d) { }
    ~DoubleProduct() { }

    double value;
  };
  typedef std::vector<DoubleProduct> DoubleProducts;

  struct StringProduct
  {
    StringProduct() : name_() {}
    explicit StringProduct(const std::string& s) : name_(s){}
    std::string name_;
  };
  typedef std::vector<StringProduct> StringProducts;
}
#endif /* test_TestObjects_ToyProducts_h */

// Local Variables:
// mode: c++
// End:
