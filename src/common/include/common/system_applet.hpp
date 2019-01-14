#ifndef SYSTEM_APPLET_HPP
#define SYSTEM_APPLET_HPP

#include <string>
#include <memory>
#include <vector>

#include <common/stream_adapter.hpp>

#include "applet.hpp"
#include "result.hpp"
#include "cmd_arguments.hpp"

class SystemApplet : public Applet
{
protected:
    SystemApplet( std::string name , std::string brief="" ) :
    Applet(name,brief)
    {
    }



public:
    virtual ~SystemApplet()
    {
    }


    virtual Result execute( CmdArguments &args , StreamAdapter &stream )    = 0;
    virtual std::string help() const { return ""; }
};

typedef std::shared_ptr<SystemApplet> SystemAppletPtr;




#endif /* !defined SYSTEM_APPLET_HPP */
