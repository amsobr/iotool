//
// Created by to on 25-06-2022.
//

#pragma once

#include <rps/AbstractCommand.hpp>


class CmdDinList : public rps::AbstractCommand
{
public:
    CmdDinList();
    
    ~CmdDinList() override;
    
    void run(rps::ContextPtr& ctx, rps::ArgumentList const& args) const override;
};


