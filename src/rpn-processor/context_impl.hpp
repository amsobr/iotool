//
// Created by to on 17/03/19.
//

#ifndef IOTOOL_CONTEXT_IMPL_HPP
#define IOTOOL_CONTEXT_IMPL_HPP

#include <rpn-processor/context.hpp>
#include "variable.hpp"

namespace Rpn {

class ContextImpl : public Context
{
private:
    std::vector<Variable> myVariables;
    std::vector<Variable> myConstants;
    std::vector<Operand> myStack;

    bool requireOperands(int count) const;

public:
    ContextImpl();

    virtual ~ContextImpl() {}

    Operand pop() override;

    void push(Operand const &value) override;

    size_t stackSize() const override;

    void clear() override;

    void drop() override;

    void dropN(size_t count) override;

    void assignVariable(std::string const &name, Operand const &value) override;

    Operand recallVariable(std::string const &name) override;

    void assignConstant(std::string const &name, Operand const &value) override;

    Operand recallConstant(std::string const &name) override;

    Operand valueAt(int pos) const override;

};

}; /* namespace Rpn */


#endif //IOTOOL_CONTEXT_IMPL_HPP
