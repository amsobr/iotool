
add_library( applets INTERFACE )

target_include_directories( applets INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include )
target_link_libraries( applets INTERFACE shell-common )