//
// Created by to on 05/03/22.
//

#pragma once

#include <rps/AbstractCommand.hpp>

#include <common/Board.hpp>
#include <common/Peripheral.hpp>


class CmdShow : public rps::AbstractCommand
{
public:
    CmdShow() :
        rps::AbstractCommand{"show","show system information"}
    {
        setHelp(
            "show - show information about the current context\n"
            "Usage:\n"
            "  show vars|variables   : show stack named variables\n"
            "  show consts|constants : show stacn named constants\n"
            "  show stack            : show the stack, from oldest to newest\n"
            "  show devices          : show all devices\n"
            ""
        );
    }
    ~CmdShow() override = default;
    
    void run(rps::ContextPtr& ctx, rps::ArgumentList const& args) const override;
    
};



