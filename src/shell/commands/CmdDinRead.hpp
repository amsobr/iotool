//
// Created by to on 23-04-2022.
//


#pragma once

#include <rps/AbstractCommand.hpp>
#include <rps/Exception.hpp>

#include <common/Board.hpp>
#include <common/DigitalIn.hpp>


class CmdDinRead : public rps::AbstractCommand
{
public:
    CmdDinRead() :
        rps::AbstractCommand{"read","read from a digital input"}
    {
        setHelp(
            "Usage:\n"
            "  read DIN [state]\n"
            "Commands:\n"
            "  state      Read the current state of the input into the stack.\n"
            "             0 indicates low and 1 indicated high state.\n"
            "             This is the default command.\n"
            "\n"
            "Arguments:\n"
            "  DIN      : The name of the input to read\n"
            "\n"
            );
        
    }
    
    ~CmdDinRead() override  = default;
    
    void run(rps::ContextPtr& ctx, rps::ArgumentList const& args) const override
    {
        auto din    = Board::get()->getPeripheral<DigitalIn>(ctx->getCwd());
        if ( args.isEmpty() || args.size()>2 ) {
            throw rps::InvalidArgumentsException{"need 1 or 2 arguments"};
        }
        
        if ( args.at(0).isKvPair() ) {
            throw rps::InvalidSyntaxException{"invalid input name"};
        }
        auto const& name    = args.at(0).getToken();
        std::string cmd     = "state";
        if ( args.size()>1 ) {
            cmd             = args.at(1).getToken();
        }
        
        if ( cmd=="state" ) {
            int val = din->get(name);
            ctx->stack.push(val);
        }
        else {
            throw rps::InvalidSyntaxException{"Invalid command: " + cmd};
        }
    }

private:

};
