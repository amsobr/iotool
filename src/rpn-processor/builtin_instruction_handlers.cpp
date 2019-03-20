

#include <rpn-processor/context.hpp>
#include <rpn-processor/operand.hpp>
#include "builtin_instruction_handlers.hpp"

namespace Rpn
{

void builtin_add( ContextPtr ctx , std::vector<std::string> const & )
{
    Operand p0 = ctx->pop();
    Operand p1 = ctx->pop();

    Operand result = p1+p0;
    ctx->push(result);
}


void builtin_sub( ContextPtr ctx , std::vector<std::string> const & )
{
    Operand p0 = ctx->pop();
    Operand p1 = ctx->pop();

    Operand result = p1-p0;
    ctx->push(result);
}

void builtin_mul( ContextPtr ctx , std::vector<std::string> const & )
{
    Operand p0 = ctx->pop();
    Operand p1 = ctx->pop();

    Operand result = p1*p0;
    ctx->push(result);
}

void builtin_div( ContextPtr ctx , std::vector<std::string> const & )
{
    Operand p0 = ctx->pop();
    Operand p1 = ctx->pop();

    Operand result = p1/p0;
    ctx->push(result);
}

void builtin_neg( ContextPtr ctx , std::vector<std::string> const & )
{
    Operand p0 = ctx->pop();

    Operand result = -p0;
    ctx->push(result);
}


}; /* namespace Rpn; */