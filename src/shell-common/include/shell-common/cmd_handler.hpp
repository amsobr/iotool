#ifndef CMD_HANDLER_HPP
#define CMD_HANDLER_HPP

#include <string>
#include <memory>

#include "cmd_result.hpp"
#include "cmd_arguments.hpp"

class CmdHandler
{
protected:
    std::string myName;
    std::string myBrief;

public:
    CmdHandler( std::string name , std::string brief="" ) :
    myName(name) ,
    myBrief(brief)
    {
    }

    virtual ~CmdHandler()
    {
    }


    virtual CmdResult execute( CmdArguments const &args )    = 0;

    std::string name() const { return myName; }
    virtual std::string brief() const { return myBrief; }
    virtual std::string help() const { return ""; }
};

typedef std::shared_ptr<CmdHandler> CmdHandlerPtr;




#endif /* !defined CMD_HANDLER_HPP */
