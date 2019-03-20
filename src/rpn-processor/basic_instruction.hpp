#ifndef BASIC_INSTRUCTION_HPP
#define BASIC_INSTRUCTION_HPP

#include <string>

#include "rpn-processor/instruction.hpp"
#include "builtin_instruction_handlers.hpp"

namespace Rpn
{

class InstructionCatalog;

class BasicInstruction : public Instruction
{
private:
    BuiltinFunction *myHandler;

public:
    BasicInstruction( std::string const &name , BuiltinFunction *handler ) :
    Instruction(name) ,
    myHandler(handler)
    {}

    virtual ~BasicInstruction() = default;

    BasicInstruction( BasicInstruction const &) = delete;
    BasicInstruction operator=( BasicInstruction const & ) = delete;

    bool resolveDependencies(RpnLib &) override
    {
        /* as builtins, we are resolved by definition... */
        return true;
    }

    void execute(ContextPtr ctx, std::vector<std::string> const &args) override
    {
        (*myHandler)(ctx,args);
    }
};

}; /* namespace Rpn */

#endif /* BASIC_INSTRUCTION_HPP */
