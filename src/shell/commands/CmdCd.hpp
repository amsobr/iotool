//
// Created by to on 22/02/22.
//


#pragma once

#include <rps/AbstractCommand.hpp>

class CmdCd : public rps::AbstractCommand
{
public:
    CmdCd();
    ~CmdCd() override   = default;
    
    void run(rps::ContextPtr& ctx, rps::ArgumentList const& args) const override;

private:

};



