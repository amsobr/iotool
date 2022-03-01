#ifndef OPERAND_HPP
#define OPERAND_HPP

namespace rps
{

typedef double Operand;

#if 0
class Operand
{
private:
    double myValue;

public:
    Operand() : myValue(0)
    {
    }

    virtual ~Operand()
    {
    }

    Operand( Operand const &other )
    {
        myValue = other.myValue;
    }

    Operand operator=( Operand const &other )
    {
        myValue = other.myValue;
        return *this;
    }

};
#endif
}; /* namespace Rpn */

#endif
