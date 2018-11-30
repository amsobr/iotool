#ifndef DAC_SET_OUT_HPP
#define DAC_SET_OUT_HPP

#include <string>
#include <memory>

#include <shell-common/cmd_result.hpp>
#include <shell-common/cmd_arguments.hpp>
#include "max581x.hpp"

class CmdDacSetOut : public CmdHandler
{
private:
    Max581xPtr myDac;
    std::string myHelp;

public:
    CmdDacSetOut( Max581xPtr dac ) :
    CmdHandler("dac.set","Set one DAC output") ,
    myDac(dac)
    {
        myHelp  =   "Usage:\n"
                    "  dac.set ch=ARG level=ARG\n"
                    "\n"
                    "Arguments:\n"
                    "ch        ID of the DAC channel: 0 .. n\n"
                    "level     Desired voltage, in mV\n"
                    ;

    }

    ~CmdDacSetOut() { }

    virtual CmdResult execute( CmdArguments const &args )
    {
        if ( !args.hasArg("ch") ) {
            return CmdResult(1,"Missing argument: ch\n");
        }

        if ( !args.hasArg("level") ) {
            return CmdResult(1,"Missing argument: level\n");
        }

        if ( args.size()!=2 ) {
            return CmdResult(1,"Invalid number of arguments\n");
        }

        unsigned int ch     = std::atoi(args.getValue("ch").c_str());
        unsigned int level  = std::atoi(args.getValue("level").c_str());
        
        myDac->setOutput(ch,level);
        return CmdResult(0,"OK");
    }
}; /* class CmdDacSetOut */


#endif /* !defined ADS126X_READ_HPP */
