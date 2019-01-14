#ifndef CMD_PM_READ_HPP
#define CMD_PM_READ_HPP

#include <string>
#include <memory>
#include <iostream>

#include <Poco/Format.h>
#include <Poco/String.h>
#include <Poco/NumberParser.h>

#include <common/result.hpp>
#include <common/cmd_arguments.hpp>
#include <common/device_applet.hpp>
#include <common/power_monitor.hpp>
#include <common/stream_adapter.hpp>


/**
 * @brief Read Power Monitor state.
 * 
 */
class CmdPmState : public DeviceApplet
{
private:
    std::string myHelp;

public:
    CmdPmState() :
    DeviceApplet( PeripheralType::PWR_MON , "state","Read current Power Monitor state")
    {
        myHelp  =   "Usage:\n"            
                    "  DEV state            : Show current state (current,voltate,power)\n"
                    "\n"
                    "Arguments: This command takes no arguments\n"
                    ;
    }

    virtual ~CmdPmState() { }


    virtual std::string help() const { return myHelp; }

    virtual Result execute( CmdArguments &args , PeripheralPtr p , StreamAdapter &stream )
    {
        try {
            PowerMonitorPtr pm  = std::dynamic_pointer_cast<PowerMonitor>(p);

            int count = 1;
            if ( args.hasArg("count") ) {
                count = Poco::NumberParser::parseUnsigned(args.getValue("count"));
            }
            for ( int i=0 ; i<count ; ++i )  {
                double curr = pm->getCurrent(0);
                double volt = pm->getVoltage(0);
                stream.writeLine( Poco::format("%s.current=%.7f",myName,curr) );
                stream.writeLine( Poco::format("%s.voltage=%.4f",myName,volt) );
                stream.writeLine( Poco::format("%s.power=%.7f\n",myName,volt*curr) );                    
            }
        }
        catch ( Poco::SyntaxException &e ) {
            return Result::E_BAD_ARGS;
        }
        return Result::OK;
    }        
}; /* class CmdPmState */

#endif /* !defined CMD_PM_READ_HPP */
