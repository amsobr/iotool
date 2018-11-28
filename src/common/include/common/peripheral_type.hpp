#ifndef PERIPHERAL_TYPE_HPP
#define PERIPHERAL_TYPE_HPP

#include <string>

enum class PeripheralType
{
    ADC ,
    DAC ,
    GPIO ,

}; /* class enum PeripheralType */

static inline std::string const peripheralTypeName( PeripheralType t )
{
    switch ( t) {
        case PeripheralType::ADC: return "ADC";
        case PeripheralType::DAC: return "DAC";
        case PeripheralType::GPIO: return "GPIO";
        default: return "UNKNOWN";
    }

}



#endif /* !defined( PERIPHERAL_TYPE_HPP) */
