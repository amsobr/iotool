#ifndef AGP01_RELAYS_HPP
#define AGP01_RELAYS_HPP


#include <string>
#include <list>
#include <memory>

#include <common/Peripheral.hpp>
#include <common/PeripheralType.hpp>
#include <common/digital_output.hpp>
#include <drivers/sysfs_gpio.hpp>


class Agp01Relays : public DigitalOutput
{
private:
    struct Relay {
        std::string name;
        bool state;
        int id;

        Relay(std::string n, bool s, int i) :
        name(n) ,
        state(s) ,
        id(i)
        {
        }
    };

    SysfsGpioPtr myGpio;
    std::list<Relay> myRelays;

public:
    Agp01Relays( unsigned int id , SysfsGpioPtr gpio );
    virtual ~Agp01Relays() {}

    virtual std::string getVendor() const { return "Yet To Be Named..."; }
    
    virtual std::string getModel() const { return "AGP01-RELAYS"; }
    
    virtual std::string getRevision() const { return "1.0a"; }
    
    virtual std::string getDriverVersion() const { return "1.0.0"; }
    
    virtual std::list<DigitalOutput::Output> const getOutputs() const;
    virtual int setOut( std::string name , bool value );
}; /* class Agp01Relays */

typedef std::shared_ptr<Agp01Relays> Agp01RelaysPtr;

#endif
