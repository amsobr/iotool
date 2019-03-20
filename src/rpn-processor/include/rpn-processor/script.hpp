//
// Created by to on 17/03/19.
//

#ifndef IOTOOL_SCRIPT_HPP
#define IOTOOL_SCRIPT_HPP

#include <memory>

#include "context.hpp"
#include "instruction.hpp"

namespace Rpn
{

class Script
{
public:
    virtual ~Script() = default;

    virtual void execute( ContextPtr ctx ) = 0;
};

typedef std::shared_ptr<Script> ScriptPtr;

};


#endif //IOTOOL_SCRIPT_HPP
