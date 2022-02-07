//
// Created by to on 03/03/21.
//

#ifndef IOTOOL_SUBROUTINEADAPTER_H
#define IOTOOL_SUBROUTINEADAPTER_H

#include <memory>
#include <vector>
#include <string>

#include "abstract_instruction.hpp"
#include "exception.hpp"
#include "context.hpp"

namespace Rpn
{

class SubroutineAdapter
{
public:
    SubroutineAdapter() = default;
    SubroutineAdapter(std::string name , std::vector<AbstractInstruction::Ptr> script ) :
    myName{ std::move(name) } ,
    myScript{ std::move(script) }
    {
    }

    ~SubroutineAdapter() = default;

    [[nodiscard]] std::string const& getName() const
    {
        return myName;
    }

    void execute( Context& ctx ) const
    {
        for ( auto & i : myScript ) {
            i->execute( ctx );
        }
    }

    typedef std::unique_ptr<SubroutineAdapter> UniqPtr;

private:
    std::string myName;
    std::vector<AbstractInstruction::Ptr> myScript;
};


} /* namespace Rpn */

#endif //IOTOOL_SUBROUTINEADAPTER_H
