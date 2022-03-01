#include <memory>


#include <Poco/Logger.h>

#include <drivers/emulated_adc.hpp>
#include <drivers/emulated_dac.hpp>

#include "EmulatorBoard.hpp"

using namespace std;

EmulatorBoard::~EmulatorBoard() = default;

EmulatorBoard::EmulatorBoard()
{
    Poco::Logger &logger = Poco::Logger::get("emulator");

    logger.information( "Creating EMULATED ADC..." );
    AdcPtr adc(new EmulatedAdc(0,8));
    myPeripherals.push_back(adc);

    logger.information( "Creating EMULATED DAC..." );
    DacPtr dac(new EmulatedDac(0,4));
    myPeripherals.push_back(dac);
}

void Board::create()
{
    myInstance  = make_shared<EmulatorBoard>();
}
