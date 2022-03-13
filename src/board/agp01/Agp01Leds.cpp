
#include "Agp01Leds.hpp"
#include "Agp01PinMapper.hpp"

using namespace std;

static int const gpioLE = PIN_B(6);
static int const gpioOEn= PIN_B(22);

Agp01Leds::Agp01Leds(unsigned int id , SysfsGpioPtr gpio ) :
Indicator(id) ,
myGpio(gpio) ,
myLedGsm("GSM") ,
myLedChg("CHG") ,
myLedBatLow("BATLOW") ,
myLedErr1("ERR1"),
myLedErr2("ERR2"),
myLedErr3("ERR3")
{
    myGpio->addPin(PIN_B(0));
    myGpio->addPin(PIN_B(1));
    myGpio->addPin(PIN_B(2));
    myGpio->addPin(PIN_B(3));
    myGpio->addPin(PIN_B(4));
    myGpio->addPin(PIN_B(5));
    myGpio->addPin(gpioLE);
    myGpio->addPin(gpioOEn);
    gpio->configureBus("data",{PIN_B(0),PIN_B(1),PIN_B(2),PIN_B(3),PIN_B(4),PIN_B(5)},Direction::OUTPUT);
    myGpio->setBusValue("data",0);
    myGpio->setDirection(gpioLE,Direction::OUTPUT);
    myGpio->setValue(gpioLE,false);
    myGpio->setDirection(gpioOEn,Direction::OUTPUT);
    myGpio->setValue(gpioOEn,false);



}

list<Indicator::Led> const Agp01Leds::getLeds() const
{
    return list<Indicator::Led>({myLedGsm,myLedChg,myLedBatLow,myLedErr1,myLedErr2,myLedErr3});
}

int Agp01Leds::setLed(string name , bool value )
{
    int mask;
    if ( name==myLedGsm.name ) {
        mask            = 0x1<<5;
        myLedGsm.value  = value;
    }
    else if ( name==myLedChg.name ) {
        mask            = 0x1<<4;
        myLedChg.value  = value;
    }
    else if ( name==myLedBatLow.name ) {
        mask                = 0x1<<3;
        myLedBatLow.value   = value;
    }
    else if ( name==myLedErr1.name ) {
        mask            = 0x1<<2;
        myLedErr1.value = value;
    }
    else if ( name==myLedErr2.name ) {
        mask            = 0x1<<1;
        myLedErr2.value = value;
    }
    else if ( name==myLedErr3.name ) {
        mask            = 0x1;
        myLedErr3.value = value;
    }
    else {
        mask    = 0;
    }
    
    if ( value==true ) {
        myDataBusValue  |= mask;

    }
    else {
        myDataBusValue &= ~mask;
    }
    myGpio->setBusValue("data",myDataBusValue);
    myGpio->setValue(gpioLE,true);
    myGpio->setValue(gpioLE,true);
    myGpio->setValue(gpioLE,false);
    myGpio->setValue(gpioLE,false);
    return 0;
}    
