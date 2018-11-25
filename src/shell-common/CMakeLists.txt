
add_library( shell-common STATIC 
    shell_engine.cpp
    shell_frontend.cpp
    )

target_include_directories( shell-common INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/ )
target_include_directories( shell-common PRIVATE include/shell-common )

target_link_libraries( shell-common cbsl )