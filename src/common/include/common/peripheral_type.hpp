#ifndef PERIPHERAL_TYPE_HPP
#define PERIPHERAL_TYPE_HPP

#include <string>
#include <ostream>

class PeripheralType
{
private:
    int const myType;
    std::string const myName;

public:
    PeripheralType() : myType(-1) , myName("UNDEFINED") {}
    PeripheralType(int id , std::string name) : myType(id) , myName(name) {}
    PeripheralType( PeripheralType const &other) : myType(other.myType) , myName(other.myName) {}
    ~PeripheralType() {}

    PeripheralType operator=(PeripheralType const &rhs) = delete;

    bool operator==( PeripheralType const &rhs ) const { return myType==rhs.myType; }
    bool operator!=( PeripheralType const &rhs ) const { return !(*this==rhs); }

    operator std::string() const { return myName; }
    operator int() const { return myType; }

    int code() const { return myType; }
    //std::string name() const { return myName; }
    std::string toString() const { return myName; }

    static PeripheralType ADC;
    static PeripheralType DAC;
    static PeripheralType GPIO;
    static PeripheralType INDICATOR;
    static PeripheralType RELAY;
    static PeripheralType DIN;
    static PeripheralType PWR_MON;
    static PeripheralType DOUT;
}; /* class enum PeripheralType */


static inline std::ostream &operator << (std::ostream &out, const PeripheralType &t)
{
    out << t.toString();
    return out;
}

static inline std::string operator + ( std::string s , PeripheralType const &t )
{
    return s + t.toString();
}

static inline std::string operator +=( std::string s , PeripheralType const &t )
{
    s   = s + t.toString();
    return s;
}

#endif /* !defined( PERIPHERAL_TYPE_HPP) */
