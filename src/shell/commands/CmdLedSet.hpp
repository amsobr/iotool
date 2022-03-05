
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
                    "  set led=ARG on|off\n"
                    "\n"
                    "Arguments:\n"
                    "led       ID of the LED\n"
                    "on|off    Desired LED state (case insensitive)\n"
        );
    }

    ~CmdLedSet() override   = default;
    
    void run(rps::ContextPtr& ctx, rps::ArgumentList const& args) const override
    {
        auto leds   = Board::get()->getPeripheral<Indicator>(ctx->getCwd());
        
        if ( args.size()!=2 ) {
            throw rps::InvalidArgumentsException{"need exactly two parameters"};
        }
    
        auto led    = args.at(0).getToken();
        auto val    = Poco::toLower(args.at(1).getToken());
    
        if ( val!="on" && val!="off" ) {
            throw rps::InvalidArgumentsException{"mode must be ON or OFF"};
        }

        bool state  = (val=="on");
        leds->setLed(led,state);
    }
}; /* class CmdLedSet */

