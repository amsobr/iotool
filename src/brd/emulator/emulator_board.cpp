#include <iostream>

#include <Poco/Logger.h>

#include <drivers/emulated_adc.hpp>
#include <drivers/emulated_dac.hpp>

//#include <drivers/ads126x.hpp>
//#include <drivers/ads126x_config.hpp>
//#include <drivers/max581x.hpp>
//#include <drivers/sysfs_gpio.hpp>
//#include <drivers/ina21x.hpp>



#include "emulator_board.hpp"
//#include "agp01_indicators.hpp"
//#include "agp01_relays.hpp"
//#include "acme-a5.hpp"

using namespace std;

static char const *LOGGER_NAME = "emulator";

EmulatorBoard::EmulatorBoard() :
Board("emubrd" , "1.0")
{
    Poco::Logger &logger = Poco::Logger::get(LOGGER_NAME);

    logger.information( "Creating EMULATED ADC..." );
    AdcPtr adc(new EmulatedAdc(0,8));
    myPeripherals.push_back(adc);

    logger.information( "Creating EMULATED DAC..." );
    DacPtr dac(new EmulatedDac(0,4));
    myPeripherals.push_back(dac);
//    Max581x::Config dacConfig;
//    dacConfig.i2cAddr   = 0x1a;
//    dacConfig.i2cDev    = "/dev/i2c-0";
//    dacConfig.fullScale = 5000;
//    dacConfig.outputGain= 2.47;
//    Max581xPtr dac(new Max581x(0,dacConfig));
//    dac->init();
//    myPeripherals.push_back(dac);
//
//    PinMapperPtr pinMapper(new AcmeA5PinMapper());
//    SysfsGpioPtr gpios(new SysfsGpio(pinMapper));
//    IndicatorPtr leds(new Agp01Indicators(0,gpios));
//    myPeripherals.push_back(leds);
//    DigitalOutputPtr outs( new Agp01Relays(0,gpios) );
//    myPeripherals.push_back(outs);
//
//    /* Power Monitor 0: System power consumption */
//    Ina21x::Config pm0Config;
//    pm0Config.i2cDev    = "/dev/i2c-0";
//    pm0Config.i2cAddr   = 0x4f;
//    pm0Config.maxCurrent= 2000/12.0; /* 2000mW at 12V */
//    pm0Config.maxVoltage= 12000;
//    pm0Config.shuntResistance = 470000;
//    Ina21xPtr pm0Ptr(new Ina21x(0,pm0Config));
//    myPeripherals.push_back(pm0Ptr);
//    
//    /* Power Monitor 1: Charger Input + System power consumption */
//    Ina21x::Config pm1Config;
//    pm1Config.i2cDev    = "/dev/i2c-0";
//    pm1Config.i2cAddr   = 0x4e;
//    pm1Config.maxCurrent= 25000/12.0; /* 25000mW at 12V */
//    pm1Config.maxVoltage= 12000;
//    pm1Config.shuntResistance = 20000;
//    Ina21xPtr pm1Ptr(new Ina21x(1,pm1Config));
//    myPeripherals.push_back(pm1Ptr);
    
}

EmulatorBoard::~EmulatorBoard()
{
    myPeripherals.clear();
}


std::vector<PeripheralPtr> EmulatorBoard::getPeripherals()
{
    return myPeripherals;
}
