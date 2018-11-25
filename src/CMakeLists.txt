
add_subdirectory( hal )
add_subdirectory( shell-common )

add_subdirectory( drivers )
add_subdirectory( applets )



set( ADS126x_UTIL_SRCS
    ads126x-util.cpp
    )
    
    
add_executable( ads126x-util ${ADS126x_UTIL_SRCS} )

target_compile_options( ads126x-util PRIVATE -std=c++11 )

target_link_libraries( ads126x-util applets shell-common cbsl hal drivers )
