#ifndef CMD_LED_SET_HPP
#define CMD_LED_SET_HPP

#include <string>
#include <memory>

#include <common/result.hpp>
#include <common/cmd_arguments.hpp>
#include <common/device_applet.hpp>
#include <common/peripheral_type.hpp>
#include <common/indicator.hpp>
#include <common/data_bucket.hpp>

class CmdLedSet : public DeviceApplet
{
private:
    std::string myHelp;

public:
    CmdLedSet() :
    DeviceApplet(PeripheralType::INDICATOR,"set","Set one LED state")
    {
        myHelp  =   "Usage:\n"
                    "  set led=ARG on|off\n"
                    "\n"
                    "Arguments:\n"
                    "led       ID of the LED\n"
                    "on|off    Desired LED state\n"
                    ;
    }

    ~CmdLedSet() { }

    virtual std::string help() const { return myHelp; }

    virtual Result execute( CmdArguments &args , PeripheralPtr p , DataBucket &dataBucket )
    {
        IndicatorPtr  leds = std::dynamic_pointer_cast<Indicator>(p);

        if ( args.size()!=2 ) {
            return Result(1,"Invalid parameters.");
        }

        Argument a1(args.shift());
        if ( a1.name()!="led" ) {
            return Result(1,"Invalid argument: " + a1.name() );
        }

        Argument a2(args.shift());
        if ( !a2.isToken() ) {
            return Result(1,"Invalid argument " + a2.name() );
        }
        
        std::string v(a2.token());
        bool state;
        if ( v=="on" ) {
            state = true;
        }
        else if ( v=="off" ) {
            state = false;
        }
        else {
            return Result(1,"Invalid state: " + v);
        }

        int r   = leds->setLed(a1.value(),state);
        return r==0 ? Result::OK : Result(1,"ERROR");
    }
}; /* class CmdLedSet */


#endif
