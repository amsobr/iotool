
#include <thread>

#include <Poco/Format.h>

#include <common/DigitalOut.hpp>

#include "Agp01Relays.hpp"

using namespace std;


Agp01Relays::Agp01Relays( int id ) :
    DigitalOut(id) ,
    myEN{gpiod::find_line("pioB21") } ,
    myOHCL{ gpiod::find_line("pioB20") } ,
    myOLCH{ gpiod::find_line("pioB19") } ,
    mySel{ "relaySel" , {"pioB16","pioB17","pioB18"} , GpioBus::Direction::OUTPUT , 0 }
{
    /* Populate the relay entries.
     * Note that the very first AGP01 board had a bug that inverted
     * the order of the Yn contacts.
     * For these versions, the software can circumvent the issue by
     * inverting the numerical order to put on the relay_sel bus,
     * which is most likely seen below in '7-i'.
     * With no bug the line should simply have 'i'
     */
    myRelays.reserve(8);
    for ( int i=0 ; i<8 ; i++ ) {
         myRelays.emplace_back(Poco::format("Y%d",i+1),false,7-i);
    }
    
    gpiod::line_request req;
    req.request_type    = gpiod::line_request::DIRECTION_OUTPUT;

    req.consumer        = "agp01/relays/en";
    myEN.request(req,1);
    
    req.consumer        = "agp01/relays/ol_ch";
    myOLCH.request(req,0);

    req.consumer        = "agp01/relays/oh_cl";
    myOHCL.request(req,0);

    mySel.setValue(0);
}

std::vector<DigitalOut::Output> Agp01Relays::getOutputs() const
{
    std::vector<Output> outs;
    outs.reserve(myRelays.size());
    for ( auto const& relay : myRelays ) {
        outs.emplace_back(relay.name,relay.state);
    }
    return outs;
}

int Agp01Relays::setOut( string name , bool value )
{
    for ( auto const&relay : myRelays ) {
        if ( relay.name==name ) {
            mySel.setValue(relay.id);
            myOLCH.set_value(value?1:0);
            myOHCL.set_value(value?0:1);
            myEN.set_value(0);
            std::this_thread::sleep_for(std::chrono::milliseconds{50});
            myEN.set_value(1);
            
        }
    }
    return -1;
}    
