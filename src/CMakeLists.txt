
add_subdirectory( common )
add_subdirectory( drivers )
add_subdirectory( brd )
add_subdirectory( applets )
add_subdirectory( shell )




set( ADS126x_UTIL_SRCS
    ads126x-util.cpp
    )
    
    
add_executable( ads126x-util ${ADS126x_UTIL_SRCS} )


target_compile_definitions( ads126x-util PRIVATE
  -DVER_MAJOR="${VER_MAJOR}"
  -DVER_MINOR="${VER_MINOR}"
  -DVER_PATCH="${VER_PATCH}"
  -DVER_EXTRA="${VER_EXTRA}"
)

target_compile_options( ads126x-util PRIVATE
-Wall
-Wextra
-Warray-bounds
-Werror=implicit-function-declaration
-Wno-psabi

)


target_compile_features(ads126x-util PUBLIC cxx_std_11)
set_target_properties(ads126x-util PROPERTIES CXX_EXTENSIONS OFF)

target_link_libraries( ads126x-util brd applets shell common cbsl )
