#include "art/Persistency/Common/Assns.h"
#include "art/Persistency/Common/Ptr.h"
#include "art/Persistency/Common/PtrVector.h"
#include "art/Persistency/Common/Wrapper.h"

#include "SimpleProducts.h"

#include <vector>

template class art::Wrapper<study::IntProduct>;
template class art::Wrapper<std::vector<study::IntProduct> >;
template class art::Wrapper<std::vector<study::DoubleProduct> >;
template class art::Wrapper<std::vector<study::StringProduct> >;
