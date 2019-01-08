#ifndef AGP01_INDICATOR_HPP
#define AGP01_INDICATOR_HPP


#include <string>
#include <list>
#include <memory>

#include <common/peripheral.hpp>
#include <common/peripheral_type.hpp>
#include <common/indicator.hpp>
#include <drivers/sysfs_gpio.hpp>


class Agp01Indicators : public Indicator
{
private:
    SysfsGpioPtr myGpio;
    int myDataBusValue;
    Indicator::Led myLedGsm;
    Indicator::Led myLedChg;
    Indicator::Led myLedBatLow;
    Indicator::Led myLedErr1;
    Indicator::Led myLedErr2;
    Indicator::Led myLedErr3;


public:
    Agp01Indicators( unsigned int id , SysfsGpioPtr gpio );
    virtual ~Agp01Indicators() {}

    virtual std::string getVendor() const { return "Yet To Be Named..."; }
    
    virtual std::string getModel() const { return "AGP01-STATUS-LEDS"; }
    
    virtual std::string getRevision() const { return "1.0a"; }
    
    virtual std::string getDriverVersion() const { return "1.0.0"; }
    
    virtual std::list<Indicator::Led> const getLeds() const;
    virtual int setLed( std::string name , bool value );
}; /* class Agp01Indicators */

typedef std::shared_ptr<Agp01Indicators> Agp01IndicatorsPtr;

#endif
