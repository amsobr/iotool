#include <iostream>

#include <drivers/ads126x.hpp>
#include <drivers/ads126x_config.hpp>
#include <drivers/max581x.hpp>
#include <drivers/sysfs_gpio.hpp>
#include <drivers/ina21x.hpp>



#include "agripino.hpp"
#include "agp01_indicators.hpp"
#include "agp01_relays.hpp"
#include "acme-a5.hpp"

using namespace std;

Agripino::Agripino() :
Board("agp01" , "1.0")
{
    Ads126xConfig adcConfig;
    adcConfig.spiDevice = "/dev/spidev0.0";
    adcConfig.inpGain   = { 
        1, /* AIN1 to AIN6 - gain=1.0 (direct connection to ADC) */
        1,
        1,
        1,
        1,
        1,
        0.5, /* AIN7 to AIN10 - gain=0.5 (buffer + 100k/100k) */
        0.5,
        0.5,
        0.5
        };
    cout << "Creating ADC...\n";
    Ads126xPtr adc(new Ads126x(0,adcConfig));
    myPeripherals.push_back(adc);

    cout << "Creating DAC...\n";
    Max581x::Config dacConfig;
    dacConfig.i2cAddr   = 0x1a;
    dacConfig.i2cDev    = "/dev/i2c-0";
    dacConfig.fullScale = 5000;
    dacConfig.outputGain= 2.47;
    Max581xPtr dac(new Max581x(0,dacConfig));
    dac->init();
    myPeripherals.push_back(dac);

    PinMapperPtr pinMapper(new AcmeA5PinMapper());
    SysfsGpioPtr gpios(new SysfsGpio(pinMapper));
    IndicatorPtr leds(new Agp01Indicators(0,gpios));
    myPeripherals.push_back(leds);
    DigitalOutputPtr outs( new Agp01Relays(0,gpios) );
    myPeripherals.push_back(outs);

    /* Power Monitor 0: System power consumption */
    Ina21x::Config pm0Config;
    pm0Config.i2cDev    = "/dev/i2c-0";
    pm0Config.i2cAddr   = 0x4f;
    pm0Config.maxCurrent= 2000/12.0; /* 2000mW at 12V */
    pm0Config.maxVoltage= 12000;
    pm0Config.shuntResistance = 470000;
    Ina21xPtr pm0Ptr(new Ina21x(0,pm0Config));
    myPeripherals.push_back(pm0Ptr);
    
    /* Power Monitor 1: Charger Input + System power consumption */
    Ina21x::Config pm1Config;
    pm1Config.i2cDev    = "/dev/i2c-0";
    pm1Config.i2cAddr   = 0x4e;
    pm1Config.maxCurrent= 25000/12.0; /* 25000mW at 12V */
    pm1Config.maxVoltage= 12000;
    pm1Config.shuntResistance = 20000;
    Ina21xPtr pm1Ptr(new Ina21x(1,pm1Config));
    myPeripherals.push_back(pm1Ptr);
    
}

Agripino::~Agripino()
{
    myPeripherals.clear();
}


std::vector<PeripheralPtr> Agripino::getPeripherals()
{
    return myPeripherals;
}
