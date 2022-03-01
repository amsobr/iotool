#ifndef BASIC_PERIPHERAL_HPP
#define BASIC_PERIPHERAL_HPP

#include <list>

#include "Peripheral.hpp"
#include "PeripheralType.hpp"
#include "result.hpp"


class BasicPeripheral : public Peripheral
{

protected:
    explicit BasicPeripheral( unsigned int id) :
    Peripheral(PeripheralType::BASIC,id)
    {

    }

public:
    virtual ~BasicPeripheral()
    {

    }   

    virtual std::string getVendor() const = 0;
    
    virtual std::string getModel() const = 0;
    
    virtual std::string getRevision() const = 0;
    
    virtual std::string getDriverVersion() const = 0;

    virtual std::list<std::string> getAttributes() const = 0;

    virtual std::string readAttribute( std::string const &name );

    virtual Result setAttribute( std::string const &name , std::string const &value );

    
}; /* class BasicPeripheral */

typedef std::shared_ptr<BasicPeripheral> BasicPeripheralPtr;


#endif /* !defined BASIC_PERIPHERAL_HPP */
