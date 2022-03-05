
#pragma once

#include <cassert>
#include <string>
#include <memory>

#include <common/result.hpp>
#include <common/Dac.hpp>
#include <common/PeripheralType.hpp>
#include <common/Board.hpp>

#include <rps/AbstractCommand.hpp>
#include <rps/Context.hpp>
#include <rps/Exception.hpp>

class CmdDacSet : public rps::AbstractCommand
{
public:
    CmdDacSet() :
        rps::AbstractCommand{ "set","Set one DAC output" }
    {
        setHelp(    "Usage:\n"
                    "  set ch=ARG level=ARG\n"
                    "\n"
                    "Arguments:\n"
                    "ch        ID of the DAC channel: 0 .. n\n"
                    "level     Desired voltage, in mV\n"
        );

    }

    ~CmdDacSet() override = default;
    
    void run(rps::ContextPtr& ctx, rps::ArgumentList const& args) const override
    {
        auto dac    = Board::get()->getPeripheral<Dac>(ctx->getCwd());
        assert(dac!=nullptr);

        int ch      = args.get("ch").getValueAsUnsigned();
        int level   = args.get("level").getValueAsUnsigned();
        if ( args.size()!=2 ) {
            throw rps::InvalidArgumentsException{"too many arguments"};
        }

        dac->setOutput(ch,level);
    }
}; /* class CmdDacSetOut */

