


add_library( drivers STATIC 
    ads126x.cpp
    max581x.cpp
)

target_include_directories( drivers PRIVATE include/drivers ${CMAKE_CURRENT_SOURCE_DIR} )
target_include_directories( drivers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include )

target_link_libraries( drivers common cbsl )
