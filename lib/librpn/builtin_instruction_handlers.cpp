

#include <rpn/context.hpp>
#include <rpn/operand.hpp>
#include <rpn/exception.hpp>
#include "builtin_instruction_handlers.hpp"
#include <Poco/NumberParser.h>

namespace Rpn
{

void builtin_clear( Context& ctx , ArgumentList const& )
{
    ctx.clear();
}


void builtin_drop( Context& ctx , ArgumentList const& )
{
    ctx.drop();
}

void builtin_dropN( Context& ctx , ArgumentList const&args )
{
    if ( args.size()!=1 ) {
        throw Rpn::ErrInvalidArguments("DROP_N requires one and only one argument(int)");
    }
    int count   = args.at(0).getAsInteger();
    if ( count<1 ) {
        throw Rpn::ErrInvalidArguments("DROP_N need an positive integer argument");
    }

    ctx.dropN(count);
}

void builtin_assignVariable( Context& ctx , ArgumentList const&args )
{
    if ( args.size()!=1 ) {
        throw Rpn::ErrInvalidArguments("DROP_N requires one and only one argument(string)");
    }
    if ( ctx.stackSize()<1 ) {
        throw Rpn::ErrNotEnoughElements("ASSIGN requires one value");
    }
    Operand v    = ctx.pop();
    ctx.assignVariable(args.at(0).getKey(),v);
}

void builtin_recallVariable( Context& ctx , ArgumentList const&args )
{
    if ( args.size()!=1 ) {
        throw Rpn::ErrInvalidArguments("RECALL_VAR requires one and only one argument(string)");
    }
    ctx.push( ctx.recallVariable(args.at(0).getKey()));
}


void builtin_assignConstant( Context& ctx , ArgumentList const&args )
{
    if ( args.size()!=1 ) {
        throw Rpn::ErrInvalidArguments("ASSIGN_CONSTANT requires one and only one argument(string)");
    }
//    if ( ctx.stackSize()<1 ) {
//        throw Rpn::ErrNotEnoughElements("ASSIGN_CONSTANT requires one value");
//    }
    Operand v    = ctx.pop();
    ctx.assignConstant(args.at(0).getKey(),v);
}


void builtin_recallConstant( Context& ctx , ArgumentList const&args )
{
    if ( args.size()!=1 ) {
        throw Rpn::ErrInvalidArguments("RECALL_VAR requires one and only one argument(string)");
    }
    ctx.push( ctx.recallConstant(args.at(0).getKey()) );
}


void builtin_add( Context& ctx , ArgumentList const& )
{
    Operand p0 = ctx.pop();
    Operand p1 = ctx.pop();

    Operand result = p1+p0;
    ctx.push(result);
}


void builtin_sub( Context& ctx , ArgumentList const& )
{
    Operand p0 = ctx.pop();
    Operand p1 = ctx.pop();

    Operand result = p1-p0;
    ctx.push(result);
}

void builtin_mul( Context& ctx , ArgumentList const& )
{
    Operand p0 = ctx.pop();
    Operand p1 = ctx.pop();

    Operand result = p1*p0;
    ctx.push(result);
}

void builtin_div( Context& ctx , ArgumentList const& )
{
    Operand p0 = ctx.pop();
    Operand p1 = ctx.pop();

    Operand result = p1/p0;
    ctx.push(result);
}

void builtin_neg( Context& ctx , ArgumentList const& )
{
    Operand p0 = ctx.pop();

    Operand result = -p0;
    ctx.push(result);
}

void builtin_push( Context& ctx , ArgumentList const&args )
{
    if ( args.size()!=1 ) {
        throw Rpn::ErrInvalidArguments("PUSH needs exactly one (numeric) argument");
    }

    double value = args.at(0).getAsDouble();
    ctx.push(value);
}

void builtin_dupN( Context& ctx , ArgumentList const&args )
{
    if ( args.size()!=1 ) {
        throw Rpn::ErrInvalidArguments("DUP_N requires one and only one argument(int)");
    }
    int count   = args.at(0).getAsInteger();
    if ( count<1 ) {
        throw Rpn::ErrInvalidArguments("DUP_N need an positive integer argument");
    }
    Operand value = ctx.valueAt(0);
    for ( int i(0) ; i<count ; i++ ) {
        ctx.push(value);
    }
}

void builtin_dup( Context& ctx , ArgumentList const& )
{
    ctx.push( ctx.valueAt(0) );
}


}; /* namespace Rpn; */