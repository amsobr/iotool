


add_library( drivers STATIC 
    ads126x.cpp
)

target_include_directories( drivers PRIVATE include/drivers )
target_include_directories( drivers INTERFACE include )

target_link_libraries( drivers hal shell-common cbsl )
