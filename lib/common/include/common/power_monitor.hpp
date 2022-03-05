#ifndef POWER_MONITOR_HPP
#define POWER_MONITOR_HPP


#include <string>

#include "Peripheral.hpp"
#include "PeripheralType.hpp"

class PowerMonitor : public Peripheral
{
protected:
    PowerMonitor( unsigned int id) :
    Peripheral(PeripheralType::PM, id)
    {

    }

public:
    virtual ~PowerMonitor()
    {

    }   

    virtual std::string getVendor() const = 0;
    
    virtual std::string getModel() const = 0;
    
    virtual std::string getRevision() const = 0;
    
    virtual std::string getDriverVersion() const = 0;
    
    virtual size_t getNumChannels() const = 0;

    virtual double getCurrent( unsigned int ch ) = 0;
    virtual double getVoltage( unsigned int ch ) = 0;
    virtual double getPower( unsigned int ch ) = 0;

}; /* class PowerMonitor */

typedef std::shared_ptr<PowerMonitor> PowerMonitorPtr;


#endif /* POWER_MONITOR_HPP */
