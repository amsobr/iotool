#pragma once


#include <string>
#include <list>
#include <memory>

#include "Peripheral.hpp"
#include "PeripheralType.hpp"

class Indicator : public Peripheral
{
public:
    struct Led {
        std::string name;
        bool value;

        explicit Led( std::string n , bool v=false) : name( std::move(n)) , value(v) {}
        Led( Led const&o ) = default;
        Led& operator=( Led const &o) = default;
    };

protected:
    explicit Indicator( int id) :
    Peripheral(PeripheralType::INDICATOR,id)
    {

    }

public:
    ~Indicator() override = default;
    
    [[nodiscard]] std::string getVendor() const override= 0;
    
    [[nodiscard]] std::string getModel() const override= 0;
    
    [[nodiscard]] std::string getRevision() const override= 0;
    
    [[nodiscard]] std::string getDriverVersion() const override = 0;
    
    [[nodiscard]] virtual std::vector<Led> getLeds() const = 0;
    
    virtual int setLed( std::string const& name , bool value ) = 0;
}; /* class Indicator */

typedef std::shared_ptr<Indicator> IndicatorPtr;


