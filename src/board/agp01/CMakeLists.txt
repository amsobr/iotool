

add_library( agp01 STATIC
    Agp01Board.cpp
    Agp01Board.cpp
    agp01_indicators.cpp
    agp01_relays.cpp
)

target_link_libraries( agp01 PRIVATE drivers common Poco::Foundation )
