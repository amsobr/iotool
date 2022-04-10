#include <iostream>

#include "Agp01Leds.hpp"


Agp01Leds::Agp01Leds( int id ) :
Indicator(id) ,
myLE{ gpiod::find_line("pioB6") } ,
myLeds{ {"ERR3","pioB0"} ,
        {"ERR2","pioB1"} ,
        {"ERR1","pioB2"} ,
        {"BAT_LOW","pioB3"} ,
        {"CHG","pioB4"} ,
        {"GSM","pioB5"} }
{
    gpiod::line_request req;
    req.request_type    = gpiod::line_request::DIRECTION_OUTPUT;
    req.consumer        = "agp01/leds/LE";
    myLE.request(req,0);
    
}

std::vector<Indicator::Led> Agp01Leds::getLeds() const
{
    std::vector<Indicator::Led> leds;
    leds.reserve(myLeds.size());
    for ( auto const& l : myLeds ) {
        leds.emplace_back(l.name,l.state);
    }
    return leds;
}

int Agp01Leds::setLed(std::string const& name , bool value )
{
    for ( auto& led : myLeds) {
        if ( led.name==name ) {
            led.state   = value;
            led.io.set_value(value?1:0);
            break; // led names are unique
        }
    }
    myLE.set_value(1);
    myLE.set_value(0);
    return 0;
}
