
#
# Reverse Polish Scripts
# A Library for scripting based on Reverse Polish Notation

project( librps LANGUAGES CXX )

add_library( rps STATIC
    include/rps/Argument.hpp
    include/rps/ArgumentList.hpp
    include/rps/AbstractCommand.hpp
    include/rps/CommandTree.hpp
    include/rps/Exception.hpp
    include/rps/NoCopy.hpp
    include/rps/NullStreamAdapter.hpp
    include/rps/Stack.hpp
    include/rps/StdioStreamAdapter.hpp
    include/rps/StreamAdapter.hpp
    include/rps/Context.hpp
    AbstractCommand.cpp
    Argument.cpp
    ArgumentList.cpp
    CommandTree.cpp
    Command.cpp
    Decoder.cpp
    Stack.cpp
    Command.cpp
    Context.cpp
    include/rps/RpsFwd.hpp
    include/rps/CliPath.hpp
    include/rps/ContextFactory.hpp
    BuiltinCommands.cpp StreamAdapter.cpp )

target_include_directories( rps PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include )

target_link_libraries( rps PUBLIC common Poco::Foundation Poco::JSON )

target_compile_options( rps PRIVATE
    -Wall
    -Wextra
    -Warray-bounds
    -Wno-psabi
    -Werror
    )

set_property(TARGET rps PROPERTY CXX_STANDARD 20 )
set_property(TARGET rps PROPERTY CXX_EXTENSIONS OFF )


