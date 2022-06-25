//
// Created by to on 25-06-2022.
//

#include <rps/Context.hpp>

#include <common/Board.hpp>
#include <common/PeripheralType.hpp>
#include <common/DigitalIn.hpp>

#include "CmdDinList.hpp"

CmdDinList::CmdDinList():
    AbstractCommand{"list","list available digital inputs"}
{
    setHelp(
        "Usage:\n"
        "  list [DIN]            list available inputs\n"
        "\n"
        "Arguments:\n"
        "  DIN       Optional name of the peripheral. May be provided to list\n"
        "            inputs from a specific peripheral, otherwise all peripherals\n"
        "            will be shown.\n"
        "\n"
        );
}

CmdDinList::~CmdDinList() = default;

void CmdDinList::run(rps::ContextPtr& ctx, rps::ArgumentList const& args) const
{
    if ( args.size()>1 ) {
        throw rps::InvalidArgumentsException{"at most one argument is allowed"};
    }
    
    std::string dinName;
    if ( args.size()==1 ) {
        dinName     = args.at(0).getAsString();
    }

    auto board          = Board::get();
    std::vector<PeripheralPtr> peripherals;
    
    if ( dinName.empty() ) {
        peripherals = board->findPeripherals(PeripheralType::DIN);
    }
    else {
        auto p      = board->getPeripheral<DigitalIn>(dinName);
        if ( p!=nullptr ) {
            peripherals.push_back(p);
        }
    }
    if ( peripherals.empty() ) {
        ctx->stream->writeLine("No Digital Inputs found.");
    }
    else {
        for ( auto const &p : peripherals ) {
            auto dinPtr = std::dynamic_pointer_cast<DigitalIn>(p);
            ctx->stream->writeLine( Poco::format("\n%s:",dinPtr->getAlias()) );
            auto inputs = dinPtr->getAll();
            for ( auto const& input: inputs ) {
                ctx->stream->writeLine(Poco::format("    %s",input.name));
            }
        }
    }
    
 
    
    

}
