#ifndef CMD_LED_STATUS_HPP
#define CMD_LED_STATUS_HPP

#include <string>
#include <memory>

#include <common/result.hpp>
#include <common/cmd_arguments.hpp>
#include <common/device_applet.hpp>
#include <common/PeripheralType.hpp>
#include <common/indicator.hpp>
#include <common/data_bucket.hpp>

#include <Poco/Format.h>

class CmdLedStatus : public DeviceApplet
{
private:
    std::string myHelp;

public:
    CmdLedStatus() :
    DeviceApplet(PeripheralType::INDICATOR,"status","Show current LED status")
    {
        myHelp  =   "Usage:\n"
                    "  status          : Show current LED status\n"
                    "\n"
                    "This command takes no arguments.\n"
                    ;
    }

    ~CmdLedStatus() { }

    virtual std::string help() const { return myHelp; }

    virtual Result execute( CmdArguments &args , PeripheralPtr p , DataBucket &db )
    {
        IndicatorPtr  leds = std::dynamic_pointer_cast<Indicator>(p);

        if ( args.size()!=0 ) {
            return Result::E_BAD_ARGS;
        }

        std::list<Indicator::Led> ledStatus = leds->getLeds();
        std::string sOn("on");
        std::string sOff("off");
        for ( auto led : ledStatus ) {
            db.addDataPoint(led.name,led.value,leds.get());
            //stream.writeLine( Poco::format("leds%u.%s=%s",leds->getId(),led.name,led.value?sOn:sOff) );
        }
        return Result::OK;
    }
}; /* class CmdLedStatus */


#endif
