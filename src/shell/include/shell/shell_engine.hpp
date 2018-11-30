#ifndef SHELL_ENGINE_HPP
#define SHELL_ENGINE_HPP

#include <list>
#include <string>
#include <memory>

#include "cmd_result.hpp"
#include "cmd_arguments.hpp"
#include "cmd_handler.hpp"



class ShellEngine {
private:
    std::list<CmdHandlerPtr> myCmds;

public:
    ShellEngine( std::list<CmdHandlerPtr> const &cmds );
    ~ShellEngine();

    Result runCommand( std::string const &cmdName , CmdArguments const &args );

    std::string help( std::string const &cmdName="" ) const;

    std::list<std::string> history();


}; /* class ShellEngine */

typedef std::shared_ptr<ShellEngine> ShellEnginePtr;


#endif /* !defined SHELL_ENGINE_HPP */
