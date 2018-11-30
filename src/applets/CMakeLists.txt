
add_library( applets STATIC
    applet_factory.cpp
)

#target_include_directories( applets PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} )
target_link_libraries( applets PRIVATE  drivers common )