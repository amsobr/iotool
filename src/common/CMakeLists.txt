
add_library( common STATIC 
    peripheral_type.cpp
)

target_link_libraries( common cbsl )

target_include_directories( common PRIVATE include/common )
target_include_directories( common INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include )
