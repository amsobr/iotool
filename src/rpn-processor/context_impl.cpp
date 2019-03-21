//
// Created by to on 17/03/19.
//

#include <rpn-processor/exception.hpp>
#include "context_impl.hpp"

namespace Rpn
{


ContextImpl::ContextImpl() :
    myVariables(),
    myConstants(),
    myStack() {

}



Operand ContextImpl::pop() {
    requireOperands(1);
    Operand v = myStack.back();
    myStack.pop_back();
    return v;
}

void ContextImpl::push(Operand const &value)
{
    myStack.push_back(value);
}

size_t ContextImpl::stackSize() const
{
    return myStack.size();
}

void ContextImpl::clear()
{
    myStack.clear();
    myConstants.clear();
    myVariables.clear();

}

void ContextImpl::drop()
{
    requireOperands(1);
    myStack.pop_back();
}

void ContextImpl::dropN(size_t count)
{
    requireOperands(count);
    for (size_t i = 0; i < count; i++)
        myStack.pop_back();
}

void ContextImpl::assignVariable(std::string const &name, const Operand &value)
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



Operand ContextImpl::recallVariable(std::string const &name)
{
    for (Variable const &v : myVariables) {
        if (v.isCalled(name)) {
            return v.value();
        }
    }
    throw ErrNoSuchVariable("Variable " + name + " not found.");
}

void ContextImpl::assignConstant(std::__cxx11::string const &name, const Operand &value)
{
    for (Variable &v : myConstants) {
        if (v.isCalled(name)) {
            throw RpnException("Illegal assignment to existing constant:" + name);
            return;
        }
    }
    /* at this point a constant wasn't found... */
    myConstants.push_back(Variable(name, value));
}


Operand ContextImpl::recallConstant(std::__cxx11::string const &name)
{
    for (Variable const &v : myConstants) {
        if (v.isCalled(name)) {
            return v.value();
        }
    }
    throw ErrNoSuchVariable("Constant " + name + " not found.");
}


bool ContextImpl::requireOperands(int count) const
{
    if ( myStack.size()<count ) {
        throw ErrNotEnoughElements();
    }
    return true;
}

Operand ContextImpl::valueAt(int pos) const
{
    requireOperands(pos+1);
    return myStack[myStack.size()-pos-1];
}

}; /* namespace Rpn */
