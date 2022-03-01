
add_library( common STATIC
    include/common/Board.hpp
    include/common/Strings.hpp
    Strings.cpp
    result.cpp
    include/common/DeviceMapper.hpp )

target_link_libraries( common PUBLIC Poco::Foundation Poco::Util )

target_include_directories( common PUBLIC include )

target_compile_options( common PRIVATE
    -Wall
    -Wextra
    -Warray-bounds
    -Wno-psabi
    -Werror
    )


set_property(TARGET common PROPERTY CXX_STANDARD 20 )
set_property(TARGET common PROPERTY CXX_EXTENSIONS OFF )

