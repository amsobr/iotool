#ifndef AGP01_RELAYS_HPP
#define AGP01_RELAYS_HPP


#include <string>
#include <list>
#include <memory>

#include <common/Peripheral.hpp>
#include <common/PeripheralType.hpp>
#include <common/DigitalOut.hpp>
#include <drivers/sysfs_gpio.hpp>


class Agp01Relays : public DigitalOut
{
private:
    struct Relay {
        std::string name;
        bool state;
        int id;

        Relay(std::string n, bool s, int i) :
        name( std::move(n) ) ,
        state(s) ,
        id(i)
        {
        }
    };

    SysfsGpioPtr myGpio;
    std::list<Relay> myRelays;

public:
    Agp01Relays( unsigned int id , SysfsGpioPtr gpio );
    ~Agp01Relays() override = default;

    [[nodiscard]] std::string getVendor() const override { return "Yet To Be Named..."; }
    
    [[nodiscard]] std::string getModel() const override { return "AGP01-RELAYS"; }
    
    [[nodiscard]] std::string getRevision() const override { return "1.0a"; }
    
    [[nodiscard]] std::string getDriverVersion() const override { return "1.0.0"; }
    
    [[nodiscard]] std::list<DigitalOut::Output> getOutputs() const override;
    int setOut( std::string name , bool value ) override;
}; /* class Agp01Relays */

typedef std::shared_ptr<Agp01Relays> Agp01RelaysPtr;

#endif
