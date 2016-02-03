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

cet_set_compiler_flags(DIAGS VIGILANT
 WERROR
 NO_UNDEFINED
 EXTRA_FLAGS -pedantic
 )
 cet_have_qual(sse2 SSE2)
 if ( SSE2 )
  cet_add_compiler_flags(CXX -msse2 -ftree-vectorizer-verbose=2)
 endif()
 
cet_report_compiler_flags()


# these are minimum required versions, not the actual product versions
find_ups_product( art v1_18_03 )
find_ups_product( cetbuildtools v4_17_02 )
find_ups_product( cetpkgsupport v0_00_00 )
find_ups_product( fhiclcpp v0_00_00 )
find_ups_product( cetlib v0_00_00 )
find_ups_product( messagefacility v0_00_00 )
find_ups_root( v6_04_06 )

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

# simple_plugin(ParaviewExtract       "module" NO_INSTALL 
#   art_Framework_Services_Registry Geometry UBooNEGeometryHelper_service)

simple_plugin(ParaviewExtract "module" NO_INSTALL art_Framework_Services_Registry )
simple_plugin(IntProducer "module" NO_INSTALL)

# cet_make_exec(fhicl_test
#  SOURCE fhicl_test.cc
#  LIBRARIES
#  fhiclcpp
#  cetlib ${Boost_PROGRAM_OPTIONS_LIBRARY}
#  )

# art_make_library( LIBRARY_NAME study_src
#                   SOURCE ${src_sources}
#                   LIBRARIES ${src_libraries}
#   	)

# art_dictionary(DICTIONARY_LIBRARIES study_src art_Persistency_Common_dict NO_INSTALL)
# art_dictionary(DICTIONARY_LIBRARIES art_Persistency_Common NO_INSTALL)
# build_dictionary(study DICTIONARY_LIBRARIES art_Persistency_Common_dict NO_INSTALL)
build_dictionary(study DICT_NAME_VAR study DICTIONARY_LIBRARIES art_Persistency_Common art_Persistency_Common_dict art_Utilities cetlib NO_INSTALL)
# art_dictionary(DICT_NAME_VAR study USE_PRODUCT_NAME NO_CHECK_CLASS_VERSION)
# art_dictionary(USE_PRODUCT_NAME)

# packaging utility
include(UseCPack)

