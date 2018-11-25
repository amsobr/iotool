#include <list>
#include <string>

#include "cmd_result.hpp"
#include "cmd_handler.hpp"
#include "cmd_arguments.hpp"
#include "shell_engine.hpp"

using namespace std;

ShellEngine::ShellEngine( list<CmdHandlerPtr> const &cmds ) :
myCmds(cmds)
{
}
    
ShellEngine::~ShellEngine()
{
    myCmds.clear();

}

CmdResult ShellEngine::runCommand( string const &cmdName , CmdArguments const &args )
{
    for( auto &handler : myCmds ) {
        if ( handler->name()==cmdName ) {
            return handler->execute( args );
        }
    }
    return CmdResult(1,"Command not found: "+ cmdName + "\n" );    
}

string ShellEngine::help( string const &cmdName ) const
{
    if ( cmdName.empty() ) {
        string msg;
        for ( auto const &handler : myCmds ) {
            char s[128];
            snprintf( s , sizeof(s) , "  %-20s%s\n", handler->name().c_str() , handler->brief().c_str() );
            msg +=s;
        }
        return msg;
    }
    else {
        for ( auto const &handler : myCmds ) {
            if ( handler->name()==cmdName ) {
                char s[128];
                snprintf( s , sizeof(s) , "  %s - %s\n", handler->name().c_str() , handler->brief().c_str() );
                return s + handler->help() + "\n";
            }
        }
    }
    return "Help not available for '" + cmdName + "'\n";
}

list<string> ShellEngine::history()
{
    return list<string>();
}
