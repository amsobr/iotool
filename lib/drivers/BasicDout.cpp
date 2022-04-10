
#include <drivers/BasicDout.hpp>


BasicDout::BasicDout( int id, Config const& config ) :
    DigitalOut{ id }
{
    myOutputs.reserve(config.size());
    for ( auto [name,pio,ini] : config ) {
        myOutputs.emplace_back(*this,name,pio,ini);
    }
}


std::vector<DigitalOut::Output> BasicDout::getOutputs() const
{
    std::vector<Output> outs;
    outs.reserve(myOutputs.size());
    for ( auto const& o: myOutputs ) {
        outs.emplace_back(o.name,o.state);
    }
    return outs;
}

int BasicDout::setOut( std::string name , bool value )
{
    for ( auto& o : myOutputs ) {
        if ( o.name==name ) {
            o.state   = value;
            o.io.set_value(value?1:0);
            break; // output names are unique
        }
        return 0;
    }
    return -1;
}

