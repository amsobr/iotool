#pragma once

#include <string>
#include <memory>
#include <iostream>

#include <Poco/Format.h>
#include <Poco/String.h>
#include <Poco/NumberParser.h>

#include <rps/AbstractCommand.hpp>
#include <rps/Context.hpp>

#include <common/Board.hpp>
#include <common/power_monitor.hpp>
#include <common/result.hpp>


/**
 * @brief Read Power Monitor state.
 * 
 */
class CmdPmState : public rps::AbstractCommand
{
public:
    CmdPmState() :
        rps::AbstractCommand{ "state","Read current Power Monitor state" }
    {
        setHelp(    "Usage:\n"
                    "  state            : Show current state (current,voltate,power)\n"
                    "\n"
                    "Arguments: This command takes no arguments\n"
        );
    }

    ~CmdPmState() override = default;
    
    void run(rps::ContextPtr& ctx, rps::ArgumentList const& args) const override
    {
        auto pm = Board::get()->getPeripheral<PowerMonitor>(ctx->getCwd());
        assert(pm!=nullptr);
        
        double curr = pm->getCurrent(0);
        double volt = pm->getVoltage(0);
        ctx->stack.push(curr);
        ctx->stack.push(volt);
    }
}; /* class CmdPmState */

