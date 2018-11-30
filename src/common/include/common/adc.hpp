#ifndef ADC_HPP
#define ADC_HPP


#include <string>

#include "peripheral.hpp"
#include "peripheral_type.hpp"

class Adc : public Peripheral
{
protected:
    Adc( unsigned int id) :
    Peripheral(PeripheralType::ADC,id)
    {

    }

public:
    virtual ~Adc()
    {

    }   

    virtual std::string getVendor() const = 0;
    
    virtual std::string getModel() const = 0;
    
    virtual std::string getRevision() const = 0;
    
    virtual std::string getDriverVersion() const = 0;
    
    virtual double readAnalog( unsigned int ch) = 0;
    virtual long int readDigital(unsigned int ch) = 0;
    virtual std::string getUnits() const = 0;    
}; /* class Adc */

typedef std::shared_ptr<Adc> AdcPtr;


#endif /* ADC_HPP */
