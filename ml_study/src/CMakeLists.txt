# ======================================================================
#  art main build file
#
#  cd .../path/to/build/directory
#  source .../path/to/art/ups/setup_for_development
#  cmake [-DCMAKE_INSTALL_PREFIX=/install/path]
#        -DCMAKE_BUILD_TYPE=$CETPKG_TYPE
#        $CETPKG_SOURCE
#  make
#  make test
#  make install
#  make package (builds distribution tarfile)
# ======================================================================


# use cmake 2.8 or later
cmake_minimum_required (VERSION 2.8)

project(study)

# cetbuildtools contains our cmake modules
SET ( CETBUILDTOOLS_VERSION $ENV{CETBUILDTOOLS_VERSION} )
IF (NOT CETBUILDTOOLS_VERSION)
    MESSAGE (FATAL_ERROR "ERROR: setup cetbuildtools to get the cmake modules")
ENDIF()

# set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Modules 
#                      $ENV{CETBUILDTOOLS_DIR}/Modules 
#		      ${CMAKE_MODULE_PATH})

set(CMAKE_MODULE_PATH $ENV{ART_DIR}/Modules $ENV{CETBUILDTOOLS_DIR}/Modules ${CMAKE_MODULE_PATH})

include(CetCMakeEnv)
cet_cmake_env()

cet_set_compiler_flags(DIAGS VIGILANT WERROR NO_UNDEFINED EXTRA_FLAGS -pedantic)
cet_have_qual(sse2 SSE2)

if ( SSE2 )
 cet_add_compiler_flags(CXX -msse2 -ftree-vectorizer-verbose=2)
endif()
 
cet_report_compiler_flags()

# these are minimum required versions, not the actual product versions
find_ups_product( uboonecode v0_00_00 )
find_ups_product( nutools v0_00_00 )
find_ups_product( art v0_00_00 )
find_ups_product( cetbuildtools v0_00_00 )
find_ups_product( fhiclcpp v0_00_00 )
find_ups_product( cetlib v0_00_00 )
find_ups_product( messagefacility v0_00_00 )
find_ups_product( larcore v0_00_00 )
find_ups_product( larsoft v0_00_00 )
find_ups_product( tbb v0_0 )

# find_ups_product( cetpkgsupport v0_00_00 )
# find_ups_product( cppunit v0_00_00 )
# find_ups_product( gccxml v0_0_000000000 )
# find_ups_product( clhep v0_0_0_0 )
# find_ups_product( sqlite v0_00_00_00 )
# find_ups_root( v0_00_00 )
# find_ups_boost(v0_00_0)


# define CLHEP
# cet_find_library( CLHEP  NAMES CLHEP   PATHS ENV CLHEP_LIB_DIR NO_DEFAULT_PATH )

# define cppunit
# cet_find_library( CPPUNIT NAMES cppunit   PATHS ENV CPPUNIT_LIB   NO_DEFAULT_PATH )

# macros for art_dictionary and simple_plugin
include(ArtDictionary)
include(BuildPlugins)

# Plugin skeleton generators for cetskelgen.
# add_subdirectory(perllib)

# tools
# add_subdirectory(tools)

# source
# add_subdirectory (art)

# testing
# add_subdirectory(test)

# ups - table and config files
# add_subdirectory(ups)

# CMake modules
# add_subdirectory(Modules)

# packaging utility
include(UseCPack)

simple_plugin(ExtractWaveforms "module" NO_INSTALL 
  art_Framework_Services_Registry SimulationBase larcore_Geometry lardata_RawData UBooNEGeometryHelper_service tbb)

# cet_make_exec(fhicl_test
#   SOURCE fhicl_test.cc
#   LIBRARIES
#   fhiclcpp
#   cetlib ${Boost_PROGRAM_OPTIONS_LIBRARY}
#   )


