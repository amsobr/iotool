

add_library( agp01 STATIC
    board_factory.cpp
    agripino.cpp
    agp01_indicators.cpp
    agp01_relays.cpp
)

target_include_directories( agp01 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} )

target_link_libraries( agp01 drivers common Poco::Foundation )
