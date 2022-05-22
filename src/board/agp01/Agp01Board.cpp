#include <iostream>
#include <gpiod.hpp>

#include <drivers/ads126x.hpp>
#include <drivers/ads126x_config.hpp>
#include <drivers/Max518x.hpp>
#include <drivers/ina21x.hpp>
#include <drivers/BasicDout.hpp>
#include <drivers/BasicDin.hpp>

#include "Agp01Board.hpp"
#include "Agp01Leds.hpp"
#include "Agp01Relays.hpp"

using namespace std;

Agp01Board::Agp01Board()
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
    myPeripherals.push_back(dac);

    cout << "Creating LEDs...\n";
    IndicatorPtr leds{ std::make_shared<Agp01Leds>(0) };
    myPeripherals.push_back(leds);
    
    cout << "Creating Relays...\n";
    DigitalOutPtr outs{ std::make_shared<Agp01Relays>(0) };
    myPeripherals.push_back(outs);
    
    cout << "Creating LED OE...\n";
    BasicDout::Config boardPinsCfg{
        { "LEDS_OE", "pioB22", false }
    };
    auto boardPins{ std::make_shared<BasicDout>(1,boardPinsCfg) };
    myPeripherals.push_back(boardPins);
    

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

    unsigned const DIN_FLAGS   = BasicDin::DinFlag::INVERTED;
    BasicDin::Config dinConfig {
        { "x1" , "pioB23", DIN_FLAGS } ,
        { "x2" , "pioB24", DIN_FLAGS } ,
        { "x3" , "pioB25", DIN_FLAGS } ,
        { "x4" , "pioB26", DIN_FLAGS } ,
        { "x5" , "pioB27", DIN_FLAGS } ,
        { "x6" , "pioB28", DIN_FLAGS } ,
        { "x7" , "pioB29", DIN_FLAGS } ,
        { "x8" , "pioB30", DIN_FLAGS } ,
    };
    auto din    = std::make_shared<BasicDin>(0,dinConfig);
    myPeripherals.push_back(din);
}


void Board::create()
{
    myInstance  = make_shared<Agp01Board>();
}

