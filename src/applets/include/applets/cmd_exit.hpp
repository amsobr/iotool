#ifndef CMD_EXIT_HPP
#define CMD_EXIT_HPP

#include <string>
#include <memory>

#include <shell-common/cmd_handler.hpp>
//#include "cmd_result.hpp"
//#include "cmd_arguments.hpp"

class CmdExit : public CmdHandler
{

public:
    CmdExit():
    CmdHandler("exit" , "exits the application." )
    {
    }

    ~CmdExit()
    {
    }


    virtual CmdResult execute( CmdArguments const &/*args*/ )
    {
        exit(0);
        return CmdResult(); /* just making the compiler happy... :-) */
    }

};


#endif /* !defined CMD_EXIT_HPP */

