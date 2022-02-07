//
// Created by to on 17/03/19.
//

#ifndef IOTOOL_SUBROUTINE_HPP
#define IOTOOL_SUBROUTINE_HPP

#include <memory>
#include <vector>

#include "context.hpp"
#include "abstract_instruction.hpp"
#include "SubroutineAdapter.h"

namespace Rpn
{

class Subroutine : public AbstractInstruction
{
public:
    Subroutine(Subroutine const& ) = delete;
    Subroutine& operator=(Subroutine const& ) = delete;

    explicit Subroutine(SubroutineAdapter const& adapter ) :
    myAdapter{ adapter }
    {
    }

    ~Subroutine() override = default;

    void execute(Context& ctx) const override
    {
        myAdapter.execute(ctx);
    }

    [[nodiscard]] std::string const& getName() const override
    {
        return myAdapter.getName();
    }

private:
    SubroutineAdapter const& myAdapter;
};

typedef std::shared_ptr<Subroutine> ScriptPtr;

}


#endif //IOTOOL_SUBROUTINE_HPP
