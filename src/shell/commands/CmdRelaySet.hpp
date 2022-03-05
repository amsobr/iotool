
#pragma once

#include <string>
#include <memory>

#include <Poco/String.h>

#include <common/PeripheralType.hpp>
#include <common/digital_output.hpp>
#include <common/Board.hpp>

#include <rps/Context.hpp>
#include <rps/Exception.hpp>
#include <rps/AbstractCommand.hpp>


class CmdRelaySet : public rps::AbstractCommand
{
public:
    CmdRelaySet() :
        rps::AbstractCommand{"set","Set one ore more relays"}
    {
        setHelp(    "Usage:\n"
                    "  set name1=on|off [name2=on|off ...]\n"
                    "\n"
                    "Arguments:\n"
                    "name      Name of the relay\n"
                    "on|off    Desired relay state\n"
        );
    }

    ~CmdRelaySet() override = default;
    
    void run(rps::ContextPtr& ctx, rps::ArgumentList const& args) const override
    {
        auto douts  = Board::get()->getPeripheral<DigitalOutput>(ctx->getCwd());
        assert(douts!=nullptr);
        
        if ( args.size()==0 ) {
            throw rps::InvalidArgumentsException{"need at least one relay=val pair"};
        }

        for ( auto const& arg : args ) {
            std::string relayName   = arg.getKey();
            std::string relaySttStr = Poco::toLower(arg.getValue());
            
            if ( relaySttStr!="on" && relaySttStr!="off" ) {
                throw rps::InvalidArgumentsException{"invalid relay state"};
            }
            bool relayState = (relaySttStr=="on");

            douts->setOut(relayName,relayState);
        }
    }
}; /* class CmdRelaySet */

