#ifndef POWER_MONITOR_HPP
#define POWER_MONITOR_HPP


#include <string>

#include "Peripheral.hpp"
#include "PeripheralType.hpp"

class PowerMonitor : public Peripheral
{
public:
    ~PowerMonitor() override = default;

    [[nodiscard]]virtual size_t getNumChannels() const = 0;

    virtual double getCurrent( unsigned int ch ) = 0;
    virtual double getVoltage( unsigned int ch ) = 0;
    virtual double getPower( unsigned int ch ) = 0;

protected:
    explicit PowerMonitor( int id) :
        Peripheral(PeripheralType::PM, id)
    {
    }
}; /* class PowerMonitor */

typedef std::shared_ptr<PowerMonitor> PowerMonitorPtr;


#endif /* POWER_MONITOR_HPP */
