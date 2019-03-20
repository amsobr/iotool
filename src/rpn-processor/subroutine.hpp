//
// Created by to on 18/03/19.
//

#ifndef IOTOOL_SUBROUTINE_HPP
#define IOTOOL_SUBROUTINE_HPP

#include <vector>
#include <string>

#include <rpn-processor/instruction.hpp>

#include "script_impl.hpp"

namespace Rpn
{


class Subroutine : public Instruction
{
private:
    ScriptImpl myScript;

public:
    Subroutine( std::string const &name , std::vector<std::string> const &script ) :
    Instruction(name) ,
    myScript(script)
    {
    }


    virtual ~Subroutine() = default;

    bool resolveDependencies(RpnLib &lib) override;

    void execute(ContextPtr ctx, std::vector<std::string> const &args) override;
};


}; /* namespace Rpn */


#endif //IOTOOL_SUBROUTINE_HPP
