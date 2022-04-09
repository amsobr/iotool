
#pragma once


#include <string>
#include <list>
#include <memory>
#include <gpiod.hpp>

#include <common/Peripheral.hpp>
#include <common/PeripheralType.hpp>
#include <common/DigitalOut.hpp>

#include <drivers/GpioBus.hpp>


class Agp01Relays : public DigitalOut
{
public:
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

    explicit Agp01Relays( int id );
    ~Agp01Relays() override = default;

    [[nodiscard]] std::string getVendor() const override { return "Yet To Be Named..."; }
    
    [[nodiscard]] std::string getModel() const override { return "AGP01-RELAYS"; }
    
    [[nodiscard]] std::string getRevision() const override { return "1.0a"; }
    
    [[nodiscard]] std::string getDriverVersion() const override { return "1.0.0"; }
    
    [[nodiscard]] std::list<DigitalOut::Output> getOutputs() const override;
    int setOut( std::string name , bool value ) override;
    

private:
    std::vector<Relay> myRelays;
    
    gpiod::line myEN; ///< Latch Enable (Active low)
    gpiod::line myOHCL; ///< High for Open, Low for Close
    gpiod::line myOLCH; ///< Low for Open, High for Close
    GpioBus     mySel;
}; /* class Agp01Relays */

typedef std::shared_ptr<Agp01Relays> Agp01RelaysPtr;

