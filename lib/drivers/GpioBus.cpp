#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <list>
#include <algorithm>
#include <gpiod.hpp>

#include <drivers/GpioBus.hpp>


GpioBus::GpioBus(std::string name, std::vector<gpiod::line> lines, GpioBus::Direction dir, uint32_t value) :
    myName{ std::move(name) } ,
    myLines{ std::move(lines) }
{
    setDirection(dir);
    if ( dir==Direction::OUTPUT ) {
        setValue(value);
    }
}

bool GpioBus::setDirection(GpioBus::Direction dir, uint32_t value)
{
    gpiod::line_request req;
    
return false;

}

bool GpioBus::setValue(uint32_t value)
{
    return false;
}

uint32_t GpioBus::readValue()
{
    return 0;
}

GpioBus::GpioBus(std::string name, std::vector<std::string> lines, GpioBus::Direction dir, uint32_t value)
{

}

GpioBus::~GpioBus()
{

}
