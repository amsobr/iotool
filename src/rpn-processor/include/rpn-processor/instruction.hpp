//
// Created by to on 18/03/19.
//

#ifndef IOTOOL_INSTRUCTION_HPP
#define IOTOOL_INSTRUCTION_HPP

#include <string>
#include <vector>
#include "context.hpp"

namespace Rpn
{
class RpnLib;

class Instruction
{
private:
    std::string myName;
public:
    Instruction( std::string name ) :
    myName(name) {}

    virtual ~Instruction() = default;

    std::string getName() const { return myName; }
    bool isCalled( std::string const &name ) const { return (myName==name); }

    virtual bool resolveDependencies( RpnLib &lib ) = 0;

    virtual void execute( ContextPtr ctx , std::vector<std::string> const &args ) = 0;

};

}; /* namespace Rpn */


#endif //IOTOOL_INSTRUCTION_HPP
