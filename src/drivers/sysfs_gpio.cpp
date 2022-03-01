#include <unistd.h>
#include <fcntl.h>

#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <algorithm>

#include "sysfs_gpio.hpp"

using namespace std;

static int echoToFile( string const &fileName , string const &value )
{
    cout << "Echoing '" + value + "' into '" + fileName +"'\n";
    int fd  = open(fileName.c_str(),O_WRONLY);
    if ( fd<0 ) {
        perror( "opening file WRONLY" );
        cerr << "File was " << fileName;
        return -1;
    }
    
    write(fd,value.c_str(),value.length());
    close(fd);
    return 0;
}

static string catFromFile( string const &fileName )
{
    int fd  = open(fileName.c_str(),O_RDONLY);
    if ( fd<0 ) {
        perror( "opening file RDONLY" );
        cerr << "File was " << fileName;
        return "";
    }
    char buf[32];
    read(fd,buf,sizeof(buf));
    close(fd);
    /* ensure string is null-terminated */
    buf[sizeof(buf)-1]  = '\0';
    return buf;
}

static inline void exportPin( unsigned int pinId )
{
    echoToFile("/sys/class/gpio/export",to_string(pinId));
}


class SysfsGpio::Bus
{
public:
    string name;
    std::vector<unsigned int> pinIds;
};


SysfsGpio::SysfsGpio( PinMapperPtr mapper ) :
myMapper(mapper)
{
    cout << "####################Names of GPIOS.....\n\n";
    cout << mapper->nameOf(0) + " " + mapper->nameOf(31) +
    " " + mapper->nameOf(32) + " " + mapper->nameOf(63) +"\n";
    cout << "####################Names of GPIOS.....\n\n";
}

SysfsGpio::SysfsGpio( vector<unsigned int> const &pins , PinMapperPtr m )
: myPins(pins) ,
myMapper(m)
{
    for ( unsigned int pinId : myPins ) {
        exportPin(pinId);
    }

}

SysfsGpio::~SysfsGpio()
{
    for ( Bus *b : myBuses ) {
        delete b;
    }
    myBuses.clear();

    /* Although this could be done, it might be better not to,
     * as other processes on the system may be using the same
     * pins...
     */
    //for ( unsigned int pinId : myPins ) {
    //    unexportPin(pinId);
    //}
}

void SysfsGpio::addPin( unsigned int pinId ) {
    if ( find(myPins.begin() , myPins.end() , pinId )==myPins.end() ) {
        myPins.push_back(pinId);
        exportPin(pinId);
    }
}

void SysfsGpio::setDirection(unsigned int pinId , Direction dir )
{
    echoToFile( "/sys/class/gpio/"+myMapper->nameOf(pinId)+"/direction" , dir==Direction::INPUT ? "in" : "out" );
}

void SysfsGpio::setValue( unsigned int pinId , bool value )
{
    echoToFile( "/sys/class/gpio/"+myMapper->nameOf(pinId)+"/value" , value==true ? "1" : "0" );
}

bool SysfsGpio::getValue( unsigned int pinId )
{
    string value    = catFromFile("/sys/class/gpio/"+myMapper->nameOf(pinId)+"/value");
    if ( value=="0" ) return false;
    return true;
}

SysfsGpio::Bus *SysfsGpio::getBus( string const &name )
{
    for ( Bus *b : myBuses ) {
        if ( b->name==name ) {
            /* already exists... */
            return b;
        }
    }
    return 0;
}

void SysfsGpio::configureBus( std::string name , std::vector<unsigned int> pins , Direction dir )
{
    if ( getBus(name)!=0 ) return;

    Bus *b   = new Bus;
    b->name  = name;
    
    for ( unsigned int pinId : pins ) {
        if ( find(myPins.begin(),myPins.end(),pinId)==myPins.end() ) {
            exportPin(pinId);
            myPins.push_back(pinId);
        }
        b->pinIds.push_back(pinId);
        setDirection(pinId,dir);
    }
    myBuses.push_back(b);
}


void SysfsGpio::setBusDirection( std::string name , Direction dir )
{
    Bus *bus    = getBus(name);
    if ( bus!=0 ) {
        for ( auto pinId : bus->pinIds ) {
            setDirection(pinId,dir);
        }
    }
}

void SysfsGpio::setBusValue( std::string name , unsigned int value )
{
    Bus *bus = getBus(name);
    if ( bus!=0 ) {
        for ( auto pinId : bus->pinIds ) {
            setValue( pinId , value&0x01 ? true : false );
            value >>= 1;
        }
    }
}

unsigned int SysfsGpio::readBus( std::string name )
{
    Bus *bus    = getBus(name);
    if ( bus!=0 ) {
        unsigned int value(0);
        for ( auto pinId : bus->pinIds ) {
            value |= getValue(pinId) ? 1 : 0;
            value <<= 1;
        }
        return value;
    }
    return 0;
}

