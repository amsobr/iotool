
add_library( drivers STATIC

    include/drivers/BasicDin.hpp
    include/drivers/BasicDout.hpp
    include/drivers/GpioBus.hpp

    ads126x.cpp
    BasicDin.cpp
    BasicDout.cpp
    GpioBus.cpp
    Max518x.cpp
    Ina21x.cpp
    emulated_adc.cpp
)

target_include_directories( drivers PUBLIC include/ )
target_include_directories( drivers INTERFACE include/ )

target_link_libraries( drivers PUBLIC 
    common
    Poco::Foundation
    ${gpiodcxx_LIBRARIES}
    )

target_compile_options( drivers PRIVATE
    -Wall
    -Wextra
    -Warray-bounds
    -Wno-psabi
    -Werror
    ${gpiod_CFLAGS}
    )
