


add_subdirectory( common )
add_subdirectory( drivers )
add_subdirectory( brd/${BOARD_NAME} )
add_subdirectory( applets )
add_subdirectory( shell )
add_subdirectory( rpn-processor )
add_subdirectory( jobs )
add_subdirectory( benchmarks )




set( IOTOOL_SRCS
    iotool.cpp
    tcp_server.cpp
    tcp_data_server.cpp
    csv_writer.cpp
    output_channel_manager.cpp
    job_manager.cpp
    io_core.cpp
    periodic_scheduler.cpp
    acquisition_scheduler_factory.cpp
    )
    
    
add_executable( iotool ${IOTOOL_SRCS} )


target_compile_definitions( iotool PRIVATE
  -DVER_MAJOR="${VER_MAJOR}"
  -DVER_MINOR="${VER_MINOR}"
  -DVER_PATCH="${VER_PATCH}"
  -DVER_EXTRA="${VER_EXTRA}"
)

target_compile_options( iotool PRIVATE
-Wall
-Wextra
-Warray-bounds
-Werror=implicit-function-declaration
-Wno-psabi

)


target_compile_features(iotool PUBLIC cxx_std_11)
set_target_properties(iotool PROPERTIES CXX_EXTENSIONS OFF)

#target_link_libraries( iotool brd applets shell common cbsl  -lpthread -lrt
#set( CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE )
target_link_libraries( iotool ${BOARD_NAME}
    rpn-processor
    applets
    shell
    jobs
    common
    cbsl
    Poco::Foundation
    Poco::Net
    Poco::Util
    Poco::XML
    Poco::JSON
    -lpthread -lrt
)
