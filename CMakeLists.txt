cmake_minimum_required( VERSION 3.8 )

project( iotool )

set( IOTOOL_VERSION "0.4.0" )

option( BOARD_NAME "Target board. Possible values are \"agp01\" (default) , emulator" "agp01" )
message( STATUS "Configuring iotool for board: ${BOARD_NAME}" )

set( CMAKE_CXX_STANDARD 20 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

add_compile_options( -Wall -Wextra -Wno-psabi -Wno-unused-parameter )

find_package( Poco REQUIRED Foundation Util Net XML Crypto JSON )

add_subdirectory( lib/librps )
add_subdirectory( lib/common )

add_subdirectory( src/drivers )





set( IOTOOL_SRCS

    #src/jobs/transform_job.cpp
    #src/jobs/transform_job.hpp

    src/shell/ConnecedTelnetClient.hpp
    src/shell/ShellFrontend.cpp
    src/shell/ShellFrontend.hpp
    src/shell/PeripheralCmdProvider.cpp
    src/shell/PeripheralCmdProvider.hpp
    #src/shell/shell_script_runner.hpp
    src/shell/socket_stream_adapter.hpp
    src/shell/TelnetStreamAdapter.hpp

    src/iotool.cpp
    #src/csv_writer.cpp
    #src/output_channel_manager.cpp
    #src/job_manager.cpp
    #src/io_core.cpp
    #src/periodic_scheduler.cpp
    #src/acquisition_scheduler_factory.cpp
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
    src/shell/commands/CmdPmState.hpp
    src/shell/commands/cmd_led_set.hpp
    src/shell/commands/cmd_led_status.hpp
    src/shell/commands/cmd_relay_set.hpp
    src/shell/commands/cmd_relay_status.hpp

    src/shell/commands/CmdHelp.cpp src/shell/commands/CmdHelp.hpp src/shell/commands/CmdShow.cpp src/shell/commands/CmdShow.hpp )



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
    -lpthread -lrt
    )

