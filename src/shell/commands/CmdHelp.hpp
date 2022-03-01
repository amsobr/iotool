//
// Created by to on 28/02/22.
//


#pragma once

#include <rps/AbstractCommand.hpp>
#include <rps/CommandTree.hpp>
#include <rps/ArgumentList.hpp>

#include "../PeripheralCmdProvider.hpp"


class CmdHelp : public rps::AbstractCommand
{
public:
    CmdHelp( rps::CommandTreeConstPtr cmdTree ,
             PeripheralCmdProvider const& periphCmds ) :
    rps::AbstractCommand{ "help" , "show online help" } ,
    myCmdTree{ std::move(cmdTree) } ,
    myPerCmds{ periphCmds }
    {
    }
    
    ~CmdHelp() override = default;
    

    void run(rps::ContextPtr& ctx, rps::ArgumentList const& args) const override;
    
private:
    rps::CommandTreeConstPtr myCmdTree;
    PeripheralCmdProvider const& myPerCmds;
};



