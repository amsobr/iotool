# Either run this standalone or include it within another project.

if ( CBSL_ENABLE_SSL_CONNECTION )
    find_package( OpenSSL REQUIRED )
    if ( NOT OPENSSL_FOUND )
        message( FATAL_ERROR "OpenSSL is required by SAKpp." )
    endif()
endif( CBSL_ENABLE_SSL_CONNECTION )



set(CBSL_SRCS
    cbsl/result.hpp
    cbsl/hex_dump.c
    cbsl/logger.c
    cbsl/keyval.cpp
    cbsl/norder_fifo.c
    cbsl/util
    cbsl/addrmap
    cbsl/base64
    cbsl/crc
    cbsl/memchk
#    cbsl/threadpool
#    cbsl/threadpoolworker
#    cbsl/threadpooltask
    cbsl/thread
    cbsl/runner_pool.cpp
    cbsl/waitcondition
    cbsl/fs_ops
    cbsl/net/name_resolver.cpp
    cbsl/net/base_sock_addr.cpp
    cbsl/net/sock_addr_4.cpp
    cbsl/net/arp_querier.cpp
    cbsl/net/udp_skt.cpp
    cbsl/net/tcp_connection.cpp
    cbsl/timer
#    cbsl/Timeout.cpp
    cbsl/hoststats
    cbsl/one_shot.cpp
    cbsl/cksum/ieee_crc32
    cbsl/shellHelpers.cpp
    cbsl/strings.cpp
)

if( CBSL_ENABLE_SSL_CONNECTION )
    list( APPEND CBSL_SRCS  net/ssl_connection.cpp )
endif()


add_library(cbsl STATIC ${CBSL_SRCS})

#target_compile_options( cbsl PUBLIC )
target_compile_features(cbsl PUBLIC cxx_std_11 c_std_11 )
set_target_properties(cbsl PROPERTIES CXX_EXTENSIONS OFF)

target_include_directories( cbsl PRIVATE cbsl/ )
target_include_directories( cbsl INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} )


target_link_libraries( cbsl pthread )


# Set and propagate variables to upper scope, if included from within a
# project. Otherwise, these variables are not relevant/needed at all
#if( PROJECT_NAME )
#    set( CBSL_SHARED_LIBRARIES cbsl PARENT_SCOPE )

#    set( CBSL_STATIC_LIBRARIES cbsl-static PARENT_SCOPE )

#    set( CBSL_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/.. PARENT_SCOPE )
#    set( CBSL_LIBRARY_DIRS ${CMAKE_CURRENT_BIN_DIR}/.. PARENT_SCOPE )
    
#    if ( CBSL_ENABLE_SHARED )
#        set( CBSL_LIBRARIES cbsl)
#    else( CBSL_ENABLE_SHARED )
#        set( CBSL_LIBRARIES cbsl-static)
#    endif( CBSL_ENABLE_SHARED )
#    if ( CBSL_ENABLE_SSL_CONNECTION )
#        list( APPEND CBSL_LIBRARIES ${OPENSSL_LIBRARIES} )
#        set( CBSL_LIBRARIES ${CBSL_LIBRARIES} PARENT_SCOPE )
#    endif()
#endif( PROJECT_NAME )

