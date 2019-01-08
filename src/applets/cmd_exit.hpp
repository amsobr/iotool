#ifndef CMD_EXIT_HPP
#define CMD_EXIT_HPP

#include <string>
#include <memory>

#include <common/system_applet.hpp>

class CmdExit : public SystemApplet
{

public:
    CmdExit():
    SystemApplet("exit" , "exits the application." )
    {
    }

    ~CmdExit()
    {
    }


    virtual Result execute( CmdArguments &/*args*/ )
    {
        exit(0);
        return Result(); /* just making the compiler happy... :-) */
    }

};


#endif /* !defined CMD_EXIT_HPP */

