#ifndef INDICATOR_HPP
#define INDICATOR_HPP


#include <string>
#include <list>
#include <memory>

#include "peripheral.hpp"
#include "peripheral_type.hpp"

class Indicator : public Peripheral
{
public:
    struct Led {
        std::string name;
        bool value;

        Led( std::string const &n , bool v=false) : name(n) , value(v) {}
        Led( Led const &o) : name(o.name) , value(o.value) {}
        Led operator=( Led const &o) { name=o.name; value=o.value; return *this; }
    };

protected:
    Indicator( unsigned int id) :
    Peripheral(PeripheralType::INDICATOR,id)
    {

    }

public:
    virtual ~Indicator()
    {

    }   

    virtual std::string getVendor() const = 0;
    
    virtual std::string getModel() const = 0;
    
    virtual std::string getRevision() const = 0;
    
    virtual std::string getDriverVersion() const = 0;
    
    virtual std::list<Led> const getLeds() const = 0;
    virtual int setLed( std::string name , bool value ) = 0;
}; /* class Indicator */

typedef std::shared_ptr<Indicator> IndicatorPtr;


#endif
