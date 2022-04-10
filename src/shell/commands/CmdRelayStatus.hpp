
#pragma once

#include <string>
#include <memory>

#include <Poco/Format.h>

#include <common/result.hpp>
#include <common/PeripheralType.hpp>
#include <common/DigitalOut.hpp>
#include <common/Board.hpp>

#include <rps/AbstractCommand.hpp>
#include <rps/Exception.hpp>
#include <rps/Context.hpp>


class CmdRelayStatus : public rps::AbstractCommand
{
public:
    CmdRelayStatus() :
        rps::AbstractCommand{"status","Show current RELAY status"}
    {
        setHelp(    "Usage:\n"
                    "  status          : Show current RELAY status\n"
                    "\n"
                    "This command takes no arguments.\n"
        );
    }

    ~CmdRelayStatus() override  = default;
    
    void run(rps::ContextPtr& ctx, rps::ArgumentList const& args) const override
    {
        auto douts  = Board::get()->getPeripheral<DigitalOut>(ctx->getCwd());

        if ( args.size()!=0 ) {
            throw rps::InvalidArgumentsException{"too many arguments"};
        }

        auto doutStatus = douts->getOutputs();
        std::string sOn("on");
        std::string sOff("off");
        ctx->stream->writeLine( "Relay Status:");
        for ( auto const& o : doutStatus ) {
            ctx->stream->writeLine( Poco::format("  %12s : %s",o.name,o.value?sOn:sOff));
        }
    }
}; /* class CmdRelayStatus */

