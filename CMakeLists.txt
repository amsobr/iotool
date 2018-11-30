cmake_minimum_required( VERSION 2.8.6 )

project( ads1262-util )

#set( KERNEL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../buildroot/output/build/linux-master" )
#set( ARCH       "arm" )

set( VER_MAJOR 0 )
set( VER_MINOR 0 )
set( VER_PATCH 1 )
set( VER_EXTRA proto )

set( USE_MCHECK FALSE )
set( USE_MTRACE FALSE )

#enable_testing()

#set(CMAKE_BUILD_TYPE Release)

#configure_file( "tools/mk_pkg.sh.cmake" ${CMAKE_BINARY_DIR}/tools/mk_pkg.sh @ONLY )

if ( USE_MCHECK )
    add_definitions( -DUSE_MCHECK )
    set( INSTRUMENTATION_LIBS -lmcheck )
endif()

if ( USE_MTRACE )
    add_definitions( -DUSE_MTRACE )
endif()

list( APPEND CMAKE_C_FLAGS
  -std=gnu99
)

#include(CheckCXXCompilerFlag)
#CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
#CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
#if(COMPILER_SUPPORTS_CXX11)
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#elseif(COMPILER_SUPPORTS_CXX0X)
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
#else()
#  message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
#endif()

#include("cmake/GTest.cmake")
#include_directories("${CMAKE_BINARY_DIR}/third-party/gmock-src/gtest/include")

#include_directories(
#  lib
#  lib/boost_install/include
#  source
#  source/lib
#  source/hal/plc
#)

#link_directories( lib )

add_subdirectory( lib/libcbsl )


add_subdirectory( src )



#add_custom_target( edpredy-upd
#        COMMAND ${CMAKE_BINARY_DIR}/tools/mk_pkg.sh ""
#        DEPENDS edpredy qcaspi gwio_chrdrv procstat qca_reset bindog
#)

#add_custom_target( edpredy-installer
#        COMMAND ${CMAKE_BINARY_DIR}/tools/mk_pkg.sh "installdir"
#        DEPENDS edpredy qcaspi gwio_chrdrv procstat qca_reset bindog cc2530FWUpdater
#)
        
