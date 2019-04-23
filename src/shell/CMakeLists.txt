
add_library( shell STATIC 
    shell_backend.cpp
    shell_frontend.cpp
    shell_backend_core.cpp
    shell_peripheral_provider.cpp
    shell_backend_factory.cpp
    )

target_include_directories( shell INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/ )
target_include_directories( shell PRIVATE include/shell )

target_link_libraries( shell common cbsl Poco::Foundation Poco::Util )