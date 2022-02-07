//
// Created by to on 03/03/21.
//

#ifndef IOTOOL_ABSTRACT_INSTRUCTION_HPP
#define IOTOOL_ABSTRACT_INSTRUCTION_HPP

#include "context.hpp"

namespace Rpn
{

class AbstractInstruction
{
public:
    AbstractInstruction() = default;
    virtual ~AbstractInstruction() = default;

    virtual void execute( Context& ) const = 0;

    [[nodiscard]] virtual std::string const& getName() const = 0;

    typedef std::unique_ptr<AbstractInstruction> UniqPtr;
    typedef std::shared_ptr<AbstractInstruction> Ptr;
};



} /* namespace Rpn */


#endif //IOTOOL_ABSTRACT_INSTRUCTION_HPP
