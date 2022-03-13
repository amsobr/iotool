
#include <unistd.h>

#include <Poco/Format.h>

#include <common/DigitalOut.hpp>

#include "Agp01Relays.hpp"
#include "Agp01PinMapper.hpp"

using namespace std;

static int const gpioEN_n = PIN_B(21);
static int const gpio_oh_cl = PIN_B(20);
static int const gpio_ol_ch = PIN_B(19);

Agp01Relays::Agp01Relays( unsigned int id , SysfsGpioPtr gpio ) :
    DigitalOut(id) ,
    myGpio(gpio)
{
    /* Populate the relay entries.
     * Note that the very first AGP01 board had a bug that inverted
     * the order of the Yn contacts.
     * For these versions, the software can circumvent the issue by
     * inverting the numerical order to put on the relay_sel bus,
     * which is most likely seen below in '7-i'.
     * With no bug the line should simply have 'i'
     */
    for ( int i=0 ; i<8 ; i++ ) {
         myRelays.push_back(Relay(Poco::format("Y%d",i+1),false,7-i));
    }

    myGpio->addPin(gpioEN_n);
    myGpio->addPin(gpio_oh_cl);
    myGpio->addPin(gpio_ol_ch);

    myGpio->configureBus( "relay_sel" , {PIN_B(16),PIN_B(17),PIN_B(18)} , Direction::OUTPUT );
    myGpio->setBusValue("relay_sel",0);

    myGpio->setDirection(gpioEN_n,Direction::OUTPUT);
    myGpio->setValue(gpioEN_n,true); /* NB: active low... */

    myGpio->setDirection(gpio_oh_cl,Direction::OUTPUT);
    myGpio->setValue(gpio_oh_cl,false);

    myGpio->setDirection(gpio_ol_ch,Direction::OUTPUT);
    myGpio->setValue(gpio_ol_ch,false);
}

list<DigitalOut::Output> Agp01Relays::getOutputs() const
{
    list<Output> outs;
    for ( auto const& relay : myRelays ) {
        outs.emplace_back(relay.name,relay.state);
    }
    return outs;
}

int Agp01Relays::setOut( string name , bool value )
{
    for ( auto const&relay : myRelays ) {
        if ( relay.name==name ) {
            myGpio->setBusValue("relay_sel",relay.id);
            myGpio->setValue(gpio_ol_ch,value);
            myGpio->setValue(gpio_oh_cl,!value);
            myGpio->setValue(gpioEN_n,false);
            usleep(100000);
            myGpio->setValue(gpioEN_n,true);
            return 0;
        }
    }
    return -1;
}    
