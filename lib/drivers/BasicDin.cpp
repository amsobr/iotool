//
// Created by to on 22-04-2022.
//

#include <drivers/BasicDin.hpp>

BasicDin::BasicDin(int id, BasicDin::Config const& pins) :
    DigitalIn{ id }
{
    myPins.reserve(pins.size());
    myNames.reserve(pins.size());
    for ( auto const& [name,ioName,flags] : pins ) {
        myPins.emplace_back(*this,name,ioName,flags);
        myNames.emplace_back(name);
    }
    

}

std::vector<DigitalIn::Input> BasicDin::getAll() const
{
    std::vector<Input> ret;
    ret.reserve(myPins.size());
    for ( auto& din: myPins ) {
        if ( din.inverted ) {
            ret.emplace_back( din.name , din.io.get_value()==0 );
        }
        else {
            ret.emplace_back( din.name , din.io.get_value()!=0 );
        }
    }
    return ret;
}

int BasicDin::get(std::string const& name) const
{
    for ( auto& din: myPins ) {
        if ( din.name==name ) {
            int v   = din.io.get_value();
            if ( din.inverted ) {
                return v!=0 ? 0 : 1;
            }
            else {
                return v;
            }
        }
    }
    throw std::runtime_error{"pin not found: " + name};
}
