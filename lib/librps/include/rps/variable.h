#ifndef VARIABLE_HPP
#define VARIABLE_HPP

#include <string>

namespace rps
{

class Variable
{
private:
    std::string myName;
    double myValue;

public:
    Variable(std::string const &name , double const &value ) :
    myName(name) ,
    myValue(value) 
    {
    }

    virtual ~Variable() = default;

    Variable &operator=( Variable const &other ) = default;

    std::string const name() const
    {
        return myName;
    }

    double value() const
    {
        return myValue;
    }

    /**
     * @brief Assign a new value to the variable
     * 
     * @param value 
     */
    void assign( double value )
    {
        myValue = value;
    }

    /**
     * @brief Check Variable name, avoiding copies
     * 
     */
    bool isCalled( std::string const &name ) const
    {
        return (myName==name);
    }



};

};



#endif /* !defined VARIABLE_HPP */

