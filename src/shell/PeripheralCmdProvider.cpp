
#include "commands/CmdAdcRead.hpp"
#include "commands/CmdAdcIsource.hpp"
#include "commands/CmdPmState.hpp"
#include "commands/CmdDacSet.hpp"
#include "commands/CmdLedSet.hpp"
#include "commands/CmdLedStatus.hpp"
#include "commands/CmdRelaySet.hpp"
#include "commands/CmdRelayStatus.hpp"

#include "PeripheralCmdProvider.hpp"


PeripheralCmdProvider::PeripheralCmdProvider()
{
    groups.clear();
    groups[PeripheralType::ADC].emplace_back( std::make_shared<CmdAdcRead>() );
    groups[PeripheralType::ADC].emplace_back( std::make_shared<CmdAdcIsource>() );
    
    groups[PeripheralType::PM].emplace_back(std::make_shared<CmdPmState>() );
    
    groups[PeripheralType::DAC].emplace_back(std::make_shared<CmdDacSet>() );
    
    groups[PeripheralType::INDICATOR].emplace_back( std::make_shared<CmdLedSet>() );
    groups[PeripheralType::INDICATOR].emplace_back( std::make_shared<CmdLedStatus>() );
    
    groups[PeripheralType::DOUT].emplace_back( std::make_shared<CmdRelaySet>() );
    groups[PeripheralType::DOUT].emplace_back( std::make_shared<CmdRelayStatus>() );
}


