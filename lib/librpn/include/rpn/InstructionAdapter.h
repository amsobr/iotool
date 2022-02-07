//
// Created by to on 03/03/21.
//

#ifndef IOTOOL_INSTRUCTIONADAPTER_H
#define IOTOOL_INSTRUCTIONADAPTER_H

#include <string>

#include "ArgumentList.h"
#include "context.hpp"
#include "exception.hpp"


namespace Rpn
{

typedef void (*HandlerFuncPtr)(Context& , ArgumentList const&);

class InstructionAdapter
{

public:
    InstructionAdapter() = default;
    InstructionAdapter( InstructionAdapter const& o ) = default;
    InstructionAdapter& operator=( InstructionAdapter const& o ) = default;

    InstructionAdapter(std::string name , HandlerFuncPtr fnPtr ) :
    myName{ std::move(name) } ,
    myFnPtr{ fnPtr }
    {

    }

    ~InstructionAdapter() = default;

    [[nodiscard]] std::string const& getName() const
    {
        return myName;
    }


    void run( Context& ctx , ArgumentList const& args ) const
    {
        if ( myFnPtr==nullptr ) {
            throw ErrNotFound{"handler not set: " + myName};
        }
        (*myFnPtr)(ctx,args);
    }

private:
    std::string     myName;
    HandlerFuncPtr  myFnPtr;
};


} /* namespace Rpn */

#endif //IOTOOL_INSTRUCTIONADAPTER_H
