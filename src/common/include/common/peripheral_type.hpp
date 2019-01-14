#ifndef PERIPHERAL_TYPE_HPP
#define PERIPHERAL_TYPE_HPP

#include <string>
#include <ostream>

enum class PeripheralType
{
    ADC ,
    DAC ,
    GPIO ,
    INDICATOR ,
    RELAY ,
    DIN ,
    PWR_MON ,
}; /* class enum PeripheralType */


extern std::ostream & operator << (std::ostream &out, const PeripheralType &t);
extern std::string operator + ( std::string s , PeripheralType const &t );
extern std::string operator +=( std::string s , PeripheralType const &t );

#endif /* !defined( PERIPHERAL_TYPE_HPP) */
