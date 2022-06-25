//
// Created by to on 17/03/19.
//

#pragma once

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
    
    void push( std::vector<Operand> const& values );

    [[nodiscard]] size_t stackSize() const;

    void clear();

    void drop();

    void dropN(int count );

    void assignVariable(std::string const &name , Operand const &value );

    Operand recallVariable(std::string const &name );

    void assignConstant(std::string const &name , Operand const &value );

    Operand recallConstant(std::string const &name );

    [[nodiscard]] Operand valueAt( int pos ) const;
    
    
    /**
     * @brief get all variables of the stack
     * @return an array with all variables
     */
    [[nodiscard]] std::vector<Variable> getVariables() const
    {
        return myVariables;
    }
    
    
    /**
     * @brief get all constants of the stack
     * @return an array with all constants
     */
    [[nodiscard]] std::vector<Variable> getConstants() const
    {
        return myConstants;
    }
    

    /**
     * @brief Get the current stack.
     *
     * Retrieves the current stack of the context. The element at position 0
     * is the deepest element of the stack, ie, the first element to be pushed.
     * The last element of the returned array is the newest element of the
     * stack, ie, the most recently pushed element.
     * @return the current stack.
     */
    [[nodiscard]] std::vector<Operand> getStack() const
    {
        return myStack;
    }
    
    [[nodiscard]] std::vector<Operand> getNewElements()
    {
        int x       = myNewElems;
        myNewElems  = 0;
        return { myStack.end()-x , myStack.end() };
    }

private:
    std::vector<Variable> myVariables;
    std::vector<Variable> myConstants;
    std::vector<Operand>  myStack;
    int                   myNewElems{ 0 };

    void requireOperands(size_t count) const
    {
        if ( myStack.size()<count ) {
            throw NotEnoughElementsException(Poco::format("Not enough operands. require=%d have=%z",count,myStack.size()));
        }
    }

};

using StackUniqPtr  = std::unique_ptr<Stack>;
using StackPtr      = std::shared_ptr<Stack>;

}

