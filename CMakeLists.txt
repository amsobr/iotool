cmake_minimum_required( VERSION 3.8 )

project( iotool LANGUAGES CXX )

set( IOTOOL_VERSION "0.9.1" )

option( BOARD_NAME "Target board. Possible values are \"agp01\" (default) , emulator" "agp01" )
message( STATUS "Configuring iotool for board: ${BOARD_NAME}" )

set( CMAKE_CXX_STANDARD 20 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

add_compile_options( -Wall -Wextra -Wno-psabi -Wno-unused-parameter )

find_package( Poco REQUIRED Foundation Util Net XML Crypto JSON )

find_package( PkgConfig REQUIRED )

pkg_check_modules( gpiodcxx REQUIRED )

add_subdirectory( lib/librps )
add_subdirectory( lib/common )
add_subdirectory( lib/drivers )


message("gpiod cflags is ${gpiodcxx_CFLAGS}" )
message("gpiod ldlags is ${gpiodcxx_LDLAGS}" )
message("gpiod libs is ${gpiodcxx_LIBRARIES}" )


set( IOTOOL_SRCS
    src/shell/ConnecedTelnetClient.hpp
    src/shell/ShellFrontend.cpp
    src/shell/ShellFrontend.hpp
    src/shell/PeripheralCmdProvider.cpp
    src/shell/PeripheralCmdProvider.hpp
    src/shell/socket_stream_adapter.hpp
    src/shell/TelnetStreamAdapter.hpp

    src/iotool.cpp
    src/board/Board.cpp

    # Shell + Commands
    src/shell/commands/CmdAdcRead.hpp
    src/shell/commands/CmdLogger.cpp
    src/shell/commands/CmdLogger.hpp
    src/shell/ParseLine.hpp
    src/shell/ParseLine.cpp
    src/shell/TelnetSessionFactory.hpp
    src/shell/TelnetStreamAdapter.cpp
    src/shell/commands/CmdAdcIsource.hpp
    src/shell/commands/CmdCd.cpp
    src/shell/commands/CmdCd.hpp
    src/shell/commands/CmdDacSet.hpp
    src/shell/commands/CmdDinList.cpp
    src/shell/commands/CmdDinList.hpp
    src/shell/commands/CmdDinRead.hpp
    src/shell/commands/CmdHelp.cpp
    src/shell/commands/CmdHelp.hpp
    src/shell/commands/CmdLedSet.hpp
    src/shell/commands/CmdLedStatus.hpp
    src/shell/commands/CmdPmState.hpp
    src/shell/commands/CmdRelaySet.hpp
    src/shell/commands/CmdRelayStatus.hpp
    src/shell/commands/CmdShow.cpp
    src/shell/commands/CmdShow.hpp
    )



add_subdirectory( src/board/${BOARD_NAME} )

add_executable( iotool ${IOTOOL_SRCS} )

target_compile_options( iotool PRIVATE
    -Wall
    -Wextra
    -Warray-bounds
    -Wno-psabi
    -Werror
    )

target_compile_definitions( iotool PRIVATE IOTOOL_VERSION="${IOTOOL_VERSION}" )


set_property(TARGET iotool PROPERTY CXX_STANDARD 20 )
set_property(TARGET iotool PROPERTY CXX_EXTENSIONS OFF )

target_link_libraries( iotool PRIVATE ${BOARD_NAME}
    rps
    common
    ${BOARD_NAME}
    drivers
    Poco::Foundation
    Poco::Net
    Poco::Util
    Poco::XML
    Poco::JSON
    gpiodcxx
#    ${gpiodcxx_LIBRARIES}
    -lpthread -lrt
    )

install( TARGETS iotool
    RUNTIME DESTINATION /usr/bin
)

