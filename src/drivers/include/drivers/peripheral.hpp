#ifndef PERIPHERAL_HPP
#define PERIPHERAL_HPP

#include <string>
#include <memory>

class Peripheral
{
private:
    std::string myName;

    /* Every peripheral must have name! */
    Peripheral() {}



public:
    Peripheral( std::string name ) :
    myName(name)
    {}

    virtual ~Peripheral() {}

    std::string getName() const { return myName; }
    virtual std::string getClass() const { return ""; }
    virtual std::string getVendor() const { return ""; }
    virtual std::string getModel() const { return "";} 
    virtual std::string getRevision() const { return ""; }
    virtual std::string getDriverVersion() const { return ""; }
    virtual std::string getAuthor() const { return ""; }
}; /* class Peripheral */

typedef std::shared_ptr<Peripheral> PeripheralPtr;

#endif /* !defined PERIPHERAL_HPP */
