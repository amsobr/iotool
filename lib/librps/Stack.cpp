//
// Created by to on 17/03/19.
//


#include <Poco/Format.h>

#include <rps/Exception.hpp>
#include <rps/Stack.hpp>


namespace rps
{


Operand Stack::pop()
{
    requireOperands(1);
    Operand v = myStack.back();
    myStack.pop_back();
    return v;
}

void Stack::push(Operand const &value)
{
    myStack.push_back(value);
}

size_t Stack::stackSize() const
{
    return myStack.size();
}

void Stack::clear()
{
    myStack.clear();
    myConstants.clear();
    myVariables.clear();

}

void Stack::drop()
{
    requireOperands(1);
    myStack.pop_back();
}

void Stack::dropN(size_t count)
{
    requireOperands(count);
    for (size_t i = 0; i < count; i++)
        myStack.pop_back();
}

void Stack::assignVariable(std::string const &name, const Operand &value)
{
    for (Variable &v : myVariables) {
        if (v.isCalled(name)) {
            v.assign(value);
            return;
        }
    }
    /* at this point a variable wasn't found... */
    myVariables.push_back(Variable(name, value));
}



Operand Stack::recallVariable(std::string const &name)
{
    for (Variable const &v : myVariables) {
        if (v.isCalled(name)) {
            return v.value();
        }
    }
    throw rps::NoSuchVariableException("Variable " + name + " not found.");
}

void Stack::assignConstant(std::string const &name, const Operand &value)
{
    for (Variable &v : myConstants) {
        if (v.isCalled(name)) {
            throw Exception("Illegal assignment to existing constant:" + name);
        }
    }
    /* at this point a constant wasn't found... */
    myConstants.push_back(Variable(name, value));
}


Operand Stack::recallConstant(std::string const &name)
{
    for (Variable const &v : myConstants) {
        if (v.isCalled(name)) {
            return v.value();
        }
    }
    throw NotFoundException("Constant " + name + " not found.");
}


Operand Stack::valueAt(int pos) const
{
    requireOperands(pos+1);
    return myStack[myStack.size()-pos-1];
}

}; /* namespace Rpn */
