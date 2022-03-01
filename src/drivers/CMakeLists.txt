


add_library( drivers STATIC 
    ads126x.cpp
    max581x.cpp
    sysfs_gpio.cpp
    ina21x.cpp
    emulated_adc.cpp
)

target_include_directories( drivers PRIVATE include/drivers ${CMAKE_CURRENT_SOURCE_DIR} )
target_include_directories( drivers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include )

target_link_libraries( drivers PUBLIC 
    common
    Poco::Foundation
    )

target_compile_options( drivers PRIVATE
    -Wall
    -Wextra
    -Warray-bounds
    -Wno-psabi
    -Werror
    )
