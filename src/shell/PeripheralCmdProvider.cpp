
#include "commands/CmdAdcRead.hpp"
#include "commands/CmdAdcIsource.hpp"
#include "commands/CmdPmState.hpp"
#include "commands/CmdDacSet.hpp"

#include "PeripheralCmdProvider.hpp"


PeripheralCmdProvider::PeripheralCmdProvider()
{
    groups.clear();
    groups[PeripheralType::ADC].emplace_back( std::make_shared<CmdAdcRead>() );
    groups[PeripheralType::ADC].emplace_back( std::make_shared<CmdAdcIsource>() );
    
    groups[PeripheralType::PM].emplace_back(std::make_shared<CmdPmState>() );
    
    groups[PeripheralType::DAC].emplace_back(std::make_shared<CmdDacSet>() );
}


