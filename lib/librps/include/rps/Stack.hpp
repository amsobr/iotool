//
// Created by to on 17/03/19.
//

#ifndef IOTOOL_STACK_HPP
#define IOTOOL_STACK_HPP

#include <memory>
#include <vector>

#include <Poco/Format.h>

#include "Exception.hpp"
#include "variable.h"
#include "operand.hpp"

namespace rps
{

class Stack
{
public:
    Stack() = default;

    ~Stack() = default;

    Operand pop();

    void push(Operand const &value );

    [[nodiscard]] size_t stackSize() const;

    void clear();

    void drop();

    void dropN(size_t count );

    void assignVariable(std::string const &name , Operand const &value );

    Operand recallVariable(std::string const &name );

    void assignConstant(std::string const &name , Operand const &value );

    Operand recallConstant(std::string const &name );

    [[nodiscard]] Operand valueAt( int pos ) const;

private:
    std::vector<Variable> myVariables;
    std::vector<Variable> myConstants;
    std::vector<Operand> myStack;

    void requireOperands(size_t count) const
    {
        if ( myStack.size()<count ) {
            throw NotEnoughElementsException(Poco::format("Not enough operands. require=%d have=%z",count,myStack.size()));
        }
    }

};

typedef std::unique_ptr<Stack> StackUniqPtr;
typedef std::shared_ptr<Stack> StackPtr;

}


#endif //IOTOOL_STACK_HPP
