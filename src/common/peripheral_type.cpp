#include <ostream>
#include <peripheral_type.hpp>

using namespace std;

static std::string getStr( PeripheralType const &t )
{
    switch ( t) {
        case PeripheralType::ADC: return "adc";
        case PeripheralType::DAC: return "dac";
        case PeripheralType::GPIO: return "gpio";
        default: return "????";
    }
}


ostream & operator << (ostream &out, const PeripheralType &t)
{
    out << getStr(t);
    return out;
}


std::string operator + ( std::string s , PeripheralType const &t )
{
    return s + getStr(t);
}

std::string operator +=( std::string s , PeripheralType const &t )
{
    s   = s + getStr(t);
    return s;
}


