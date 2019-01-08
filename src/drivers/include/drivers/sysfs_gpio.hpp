#ifndef SYSFS_GPIO_HPP
#define SYSFS_GPIO_HPP

#include <vector>
#include <string>
#include <list>
#include <memory>

#include <common/pin_mapper.hpp>

enum class Direction {
    OUTPUT ,
    INPUT
};


class SysfsGpio
{
private:
    class Bus;

    std::vector<unsigned int> myPins;
    std::list<Bus*> myBuses;
    PinMapperPtr myMapper;

    Bus *getBus( std::string const &name );

public:

    SysfsGpio( PinMapperPtr pinMapper );
    SysfsGpio( std::vector<unsigned int> const &pins , PinMapperPtr pinMapper );
    ~SysfsGpio();

    void addPin( unsigned int pinId);
    void setDirection(unsigned int pinId , Direction dir );
    void setValue( unsigned int pinId , bool value );
    bool getValue( unsigned int pinId );

    void configureBus( std::string name , std::vector<unsigned int> pins , Direction dir );
    void setBusDirection( std::string name , Direction dir );
    void setBusValue( std::string name , unsigned int value );
    unsigned int readBus( std::string name );
}; /* class SysfsGpio */

typedef std::shared_ptr<SysfsGpio> SysfsGpioPtr;

#endif /* !defined(SYSFS_GPIO_HPP) */
