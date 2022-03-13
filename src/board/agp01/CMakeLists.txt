

add_library( agp01 STATIC
    Agp01Board.cpp
    Agp01Board.cpp
    Agp01Leds.cpp
    Agp01Relays.cpp
)

target_link_libraries( agp01 PRIVATE drivers common Poco::Foundation )
