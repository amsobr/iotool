cmake_minimum_required( VERSION 3.8 )

project( iotool )

list( APPEND  CMAKE_PREFIX_PATH  ${CMAKE_CURRENT_SOURCE_DIR}/cmake )


set( BOARD_NAME agp01 CACHE STRING "Target board. Possible values are \"agp01\" (default) , emulator" )

message( STATUS "Configuring iotool for board: ${BOARD_NAME}" )


set( VER_MAJOR 0 )
set( VER_MINOR 3 )
set( VER_PATCH 1 )
set( VER_EXTRA m0 )

set( USE_MCHECK FALSE )
set( USE_MTRACE FALSE )

set( CMAKE_CXX_STANDARD 20 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
#enable_testing()

#set(CMAKE_BUILD_TYPE Release)


add_compile_options( -Wall -Wextra -Wno-psabi -Wno-unused-parameter )

add_definitions(
    -DVER_MAJOR="${VER_MAJOR}"
    -DVER_MINOR="${VER_MINOR}"
    -DVER_PATCH="${VER_PATCH}"
    -DVER_EXTRA="${VER_EXTRA}"
    )

if ( USE_MCHECK )
    add_definitions( -DUSE_MCHECK )
    set( INSTRUMENTATION_LIBS -lmcheck )
endif()

if ( USE_MTRACE )
    add_definitions( -DUSE_MTRACE )
endif()


find_package( Poco REQUIRED Foundation Util Net XML Crypto JSON )

add_subdirectory( lib/libcbsl )
add_subdirectory( lib/librpn )

add_subdirectory( src )
