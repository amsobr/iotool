#ifndef DIGITAL_OUTPUT_HPP
#define DIGITAL_OUTPUT_HPP


#include <string>
#include <list>
#include <memory>

#include "Peripheral.hpp"
#include "PeripheralType.hpp"

class DigitalOutput : public Peripheral
{
public:
    struct Output {
        std::string const name;
        bool value;

        Output( std::string const &n , bool v=false) : name(n) , value(v) {}
        Output( Output const &o) : name(o.name) , value(o.value) {}
        //Output operator=( Output const &o) { name=o.name; value=o.value; return *this; }
    };

protected:
    DigitalOutput( unsigned int id) :
    Peripheral(PeripheralType::DOUT,id)
    {

    }

public:
    virtual ~DigitalOutput()
    {

    }   

    virtual std::string getVendor() const = 0;
    
    virtual std::string getModel() const = 0;
    
    virtual std::string getRevision() const = 0;
    
    virtual std::string getDriverVersion() const = 0;
    
    virtual std::list<Output> const getOutputs() const = 0;
    virtual int setOut( std::string name , bool value ) = 0;
}; /* class DigitalOutput */

typedef std::shared_ptr<DigitalOutput> DigitalOutputPtr;


#endif
