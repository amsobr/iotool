#ifndef CMD_RELAY_STATUS_HPP
#define CMD_RELAY_STATUS_HPP

#include <string>
#include <memory>

#include <common/result.hpp>
#include <common/cmd_arguments.hpp>
#include <common/device_applet.hpp>
#include <common/peripheral_type.hpp>
#include <common/digital_output.hpp>
#include <common/stream_adapter.hpp>

#include <Poco/Format.h>

class CmdRelayStatus : public DeviceApplet
{
private:
    std::string myHelp;

public:
    CmdRelayStatus() :
    DeviceApplet(PeripheralType::DOUT,"status","Show current RELAY status")
    {
        myHelp  =   "Usage:\n"
                    "  status          : Show current RELAY status\n"
                    "\n"
                    "This command takes no arguments.\n"
                    ;
    }

    ~CmdRelayStatus() { }

    virtual std::string help() const { return myHelp; }

    virtual Result execute( CmdArguments &args , PeripheralPtr p , StreamAdapter &stream )
    {
        DigitalOutputPtr  douts = std::dynamic_pointer_cast<DigitalOutput>(p);

        if ( args.size()!=0 ) {
            return Result::E_BAD_ARGS;
        }

        std::list<DigitalOutput::Output> doutStatus = douts->getOutputs();
        std::string sOn("on");
        std::string sOff("off");
        for ( auto o : doutStatus ) {
            stream.writeLine( Poco::format("leds%u.%s=%s",douts->getId(),o.name,o.value?sOn:sOff) );
        }
        return Result::OK;
    }
}; /* class CmdRelayStatus */


#endif
