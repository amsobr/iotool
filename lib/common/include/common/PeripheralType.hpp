

#pragma once


#include <string>
#include <ostream>

enum class PeripheralType
{
    BOARD   ,
    ADC ,
    DAC ,
    GPIO ,
    INDICATOR ,
    RELAY ,
    DIN ,
    PM ,
    DOUT ,
    BASIC ,
};


static inline std::string str(PeripheralType t)
{
    switch (t) {
        case PeripheralType::BOARD:     return "BOARD";
        case PeripheralType::ADC:       return "ADC";
        case PeripheralType::DAC:       return "DAC";
        case PeripheralType::GPIO:      return "GPIO";
        case PeripheralType::INDICATOR: return "INDICATOR";
        case PeripheralType::RELAY:     return "RELAY";
        case PeripheralType::DIN:       return "DIN";
        case PeripheralType::PM:        return "PM";
        case PeripheralType::DOUT:      return "DOUT";
        case PeripheralType::BASIC:     return "BASIC";
        default:                        return "????";
    }
}

static inline std::ostream &operator << (std::ostream &out, const PeripheralType &t)
{
    out << str(t);
    return out;
}
