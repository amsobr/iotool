//
// Created by to on 17/03/19.
//

#ifndef IOTOOL_CONTEXT_HPP
#define IOTOOL_CONTEXT_HPP

#include <memory>
#include <vector>
#include "operand.hpp"

namespace Rpn {

class Context {

public:
    Context() { }

    virtual ~Context() { }

    virtual Operand pop() = 0;

    virtual void push(Operand const &value ) = 0;

    virtual size_t stackSize() const = 0;

    virtual void clear() = 0;

    virtual void drop() = 0;

    virtual void dropN(size_t count ) = 0;

    virtual void assignVariable(std::string const &name , Operand const &value ) = 0;

    virtual Operand recallVariable(std::string const &name ) = 0;

    virtual void assignConstant(std::string const &name , Operand const &value ) = 0;

    virtual Operand recallConstant(std::string const &name ) = 0;

    virtual Operand valueAt( int pos ) const = 0;
};

typedef std::shared_ptr<Context> ContextPtr;

}


#endif //IOTOOL_CONTEXT_HPP
