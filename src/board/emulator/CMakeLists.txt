

add_library( emulator STATIC
    EmulatorBoard.cpp
    board_factory.cpp
)

target_include_directories( emulator PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} )

target_link_libraries( emulator drivers common Poco::Foundation )
