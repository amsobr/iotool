cmake_minimum_required( VERSION 3.8 )

project( iotool )

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
#set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
list( APPEND  CMAKE_PREFIX_PATH  ${CMAKE_CURRENT_SOURCE_DIR}/cmake )


set( BOARD_NAME agp01 CACHE STRING "Target board. Possible values are \"agp01\" (default) , emulator" )

message( STATUS "Configuring iotool for board: ${BOARD_NAME}" )


set( VER_MAJOR 0 )
set( VER_MINOR 3 )
set( VER_PATCH 1 )
set( VER_EXTRA m0 )

set( USE_MCHECK FALSE )
set( USE_MTRACE FALSE )

#enable_testing()

#set(CMAKE_BUILD_TYPE Release)


add_compile_options( -Wall -Wextra -Wno-psabi -Wno-unused-parameter )

if ( USE_MCHECK )
    add_definitions( -DUSE_MCHECK )
    set( INSTRUMENTATION_LIBS -lmcheck )
endif()

if ( USE_MTRACE )
    add_definitions( -DUSE_MTRACE )
endif()





find_package( Poco REQUIRED Foundation Util Net XML Crypto JSON )


add_subdirectory( lib/libcbsl )


add_subdirectory( src )
