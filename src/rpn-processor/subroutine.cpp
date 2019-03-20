//
// Created by to on 18/03/19.
//

#include <sstream>

#include <rpn-processor/exception.hpp>

#include "subroutine.hpp"
#include "script_impl.hpp"
#include "instruction_catalog.hpp"



namespace Rpn
{

bool Subroutine::resolveDependencies(RpnLib &lib)
{
    return myScript.resolveDependencies(lib);
}

void Subroutine::execute(ContextPtr ctx, std::vector<std::string> const &args)
{
    myScript.execute();
}


}; /* namespace Rpn */

