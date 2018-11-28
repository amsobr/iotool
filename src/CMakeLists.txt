
add_subdirectory( hal )
add_subdirectory( shell-common )

add_subdirectory( drivers )
add_subdirectory( applets )



set( ADS126x_UTIL_SRCS
    ads126x-util.cpp
    )
    
    
add_executable( ads126x-util ${ADS126x_UTIL_SRCS} )

target_compile_features(ads126x-util PUBLIC cxx_std_11)
set_target_properties(ads126x-util PROPERTIES CXX_EXTENSIONS OFF)

target_link_libraries( ads126x-util applets shell-common cbsl hal drivers )
