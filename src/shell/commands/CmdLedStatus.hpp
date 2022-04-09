
#pragma once

#include <string>
#include <memory>

#include <Poco/Format.h>

#include <common/result.hpp>
#include <common/PeripheralType.hpp>
#include <common/Indicator.hpp>
#include <common/Board.hpp>

#include <rps/Context.hpp>
#include <rps/AbstractCommand.hpp>
#include <rps/Exception.hpp>


class CmdLedStatus : public rps::AbstractCommand
{
public:
    CmdLedStatus() :
        rps::AbstractCommand{ "status","Show current LED status" }
    {
        setHelp(    "Usage:\n"
                    "  status          : Show current LED status\n"
                    "\n"
                    "This command takes no arguments.\n"
        );
    }

    ~CmdLedStatus() override = default;
    
    void run(rps::ContextPtr& ctx, rps::ArgumentList const& args) const override
    {
        auto leds   = Board::get()->getPeripheral<Indicator>(ctx->getCwd());
        
        if ( args.size()!=0 ) {
            throw rps::InvalidArgumentsException{"too many arguments"};
        }

        auto ledStatus  = leds->getLeds();
        std::string sOn("on");
        std::string sOff("off");
        ctx->stream->writeLine("Indicator Status:");
        for ( auto const& led : ledStatus ) {
            ctx->stream->writeLine(Poco::format("  %-10s : %s",led.name,led.value?"on":"off"));
        }
    }
}; /* class CmdLedStatus */

