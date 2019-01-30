#ifndef PERIPHERAL_HPP
#define PERIPHERAL_HPP

#include <memory>
#include <string>

#include <Poco/Format.h>

#include "peripheral_type.hpp"

class Peripheral
{
private:
    PeripheralType  myType;
    unsigned int    myId;
    std::string     myName;

    /* Every peripheral must have name! */
    Peripheral() {}



public:
    Peripheral( PeripheralType &t , unsigned int id ) :
    myType(t) ,
    myId(id) , 
    myName( Poco::format("%s%u",t.toString(),id) )
    {}

    virtual ~Peripheral() {}

    PeripheralType getType() const { return myType; }
    unsigned int getId() const { return myId; }
    std::string getName() const { return myName; }
    virtual std::string getVendor() const { return ""; }
    virtual std::string getModel() const { return "";} 
    virtual std::string getRevision() const { return ""; }
    virtual std::string getDriverVersion() const { return ""; }
    virtual std::string getAuthor() const { return ""; }
}; /* class Peripheral */

typedef std::shared_ptr<Peripheral> PeripheralPtr;

#endif /* !defined PERIPHERAL_HPP */
