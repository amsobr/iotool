//
// Created by to on 18/03/19.
//

#ifndef IOTOOL_INSTRUCTION_HPP
#define IOTOOL_INSTRUCTION_HPP

#include <string>
#include <vector>

#include "context.hpp"
#include "abstract_instruction.hpp"
#include "ArgumentList.h"
#include "InstructionAdapter.h"

namespace Rpn
{
class RpnLib;
class InstructionAdapter;
class Instruction : public AbstractInstruction
{
public:
    Instruction(Instruction const& ) = delete;
    Instruction& operator=(Instruction const&) = delete;

    Instruction(ArgumentList const& args , InstructionAdapter const& adapter ) :
    myAdapter{ adapter } ,
    myArguments{ args }
    {
    }

    ~Instruction() override = default;

    void execute( Context& ctx ) const override
    {
        myAdapter.run(ctx,myArguments);
    }

    [[nodiscard]] std::string const& getName() const override
    {
        return myAdapter.getName();
    }

private:
    InstructionAdapter const& myAdapter;
    ArgumentList myArguments;
};

} /* namespace Rpn */


#endif //IOTOOL_INSTRUCTION_HPP
