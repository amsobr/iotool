#ifndef DAC_HPP
#define DAC_HPP


#include <string>

#include "peripheral.hpp"
#include "peripheral_type.hpp"

class Dac : public Peripheral
{
protected:
    Dac( unsigned int id) :
    Peripheral(PeripheralType::DAC,id)
    {

    }

public:
    virtual ~Dac()
    {

    }   

    virtual std::string getVendor() const = 0;
    
    virtual std::string getModel() const = 0;
    
    virtual std::string getRevision() const = 0;
    
    virtual std::string getDriverVersion() const = 0;
    
    virtual size_t getNumChannels() const = 0;

    virtual int setOutput( unsigned int ch , unsigned int level ) = 0;

}; /* class Dac */

typedef std::shared_ptr<Dac> DacPtr;


#endif /* DAC_HPP */
