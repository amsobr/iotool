
#pragma once

#include <string>
#include <memory>

#include <Poco/String.h>

#include <common/PeripheralType.hpp>
#include <common/Indicator.hpp>
#include <common/Board.hpp>

#include <rps/Context.hpp>
#include <rps/AbstractCommand.hpp>
#include <rps/Exception.hpp>

class CmdLedSet : public rps::AbstractCommand
{
public:
    CmdLedSet() :
        rps::AbstractCommand{ "set","Set one LED state" }
    {
        setHelp(    "Usage:\n"
                    "  set LED1=on|off [LED2=on|off ...]\n"
                    "\n"
                    "Arguments:\n"
                    "LEDn      Name of the LED (check available LEDs with status)\n"
                    "on|off    Desired LED state (case insensitive)\n"
        );
    }

    ~CmdLedSet() override   = default;
    
    void run(rps::ContextPtr& ctx, rps::ArgumentList const& args) const override
    {
        auto leds   = Board::get()->getPeripheral<Indicator>(ctx->getCwd());
        
        if ( args.size()<1 ) {
            throw rps::InvalidArgumentsException{"need at least one parameter"};
        }
        
        for ( auto const& arg : args ) {
            if ( !arg.isKvPair() ) {
                throw rps::InvalidArgumentsException{"arguments must be LED=on|off pairs"};
            }
            auto led    = arg.getKey();
            auto val    = Poco::toLower(arg.getValue());
    
            if ( val!="on" && val!="off" ) {
                throw rps::InvalidArgumentsException{"mode must be ON or OFF"};
            }

            bool state  = (val=="on");
            leds->setLed(led,state);
        }
    }
}; /* class CmdLedSet */

