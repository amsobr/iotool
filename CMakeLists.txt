cmake_minimum_required( VERSION 3.8 )

project( iotool )

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
#set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
list( APPEND  CMAKE_PREFIX_PATH  ${CMAKE_CURRENT_SOURCE_DIR}/cmake )



set( VER_MAJOR 0 )
set( VER_MINOR 1 )
set( VER_PATCH 0 )
set( VER_EXTRA m0 )

set( USE_MCHECK FALSE )
set( USE_MTRACE FALSE )

#enabl_testing()

#set(CMAKE_BUILD_TYPE Release)

#configure_file( "tools/mk_pkg.sh.cmake" ${CMAKE_BINARY_DIR}/tools/mk_pkg.sh @ONLY )

add_compile_options( -Wno-psabi )

if ( USE_MCHECK )
    add_definitions( -DUSE_MCHECK )
    set( INSTRUMENTATION_LIBS -lmcheck )
endif()

if ( USE_MTRACE )
    add_definitions( -DUSE_MTRACE )
endif()



find_package( Poco REQUIRED Foundation Util Net XML )


add_subdirectory( lib/libcbsl )


add_subdirectory( src )


#TODO: some new task...
