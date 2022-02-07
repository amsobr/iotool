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
    Context() = default;

    virtual ~Context() = default;

    virtual Operand pop() = 0;

    virtual void push(Operand const &value ) = 0;

    [[nodiscard]] virtual size_t stackSize() const = 0;

    virtual void clear() = 0;

    virtual void drop() = 0;

    virtual void dropN(size_t count ) = 0;

    virtual void assignVariable(std::string const &name , Operand const &value ) = 0;

    virtual Operand recallVariable(std::string const &name ) = 0;

    virtual void assignConstant(std::string const &name , Operand const &value ) = 0;

    virtual Operand recallConstant(std::string const &name ) = 0;

    [[nodiscard]] virtual Operand valueAt( int pos ) const = 0;

    typedef std::unique_ptr<Context> UniqPtr;
    typedef std::shared_ptr<Context> Ptr;
};

}


#endif //IOTOOL_CONTEXT_HPP
