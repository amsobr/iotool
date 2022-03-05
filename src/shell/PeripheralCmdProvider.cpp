
#include "commands/CmdAdcRead.hpp"
#include "commands/CmdAdcIsource.hpp"

#include "PeripheralCmdProvider.hpp"


PeripheralCmdProvider::PeripheralCmdProvider()
{
    groups.clear();
    groups[PeripheralType::ADC].emplace_back( std::make_shared<CmdAdcRead>() );
    groups[PeripheralType::ADC].emplace_back( std::make_shared<CmdAdcIsource>() );
}


