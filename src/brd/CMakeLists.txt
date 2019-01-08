

add_library( brd STATIC
    board_factory.cpp
    agp01/agripino.cpp
    agp01/agp01_indicators.cpp
)

target_include_directories( brd PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} )

target_link_libraries( brd drivers common )
