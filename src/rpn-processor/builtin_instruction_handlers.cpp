

#include <rpn-processor/context.hpp>
#include <rpn-processor/operand.hpp>
#include <rpn-processor/exception.hpp>
#include "builtin_instruction_handlers.hpp"
#include <Poco/NumberParser.h>

namespace Rpn
{

void builtin_clear( ContextPtr ctx , std::vector<std::string> const & )
{
    ctx->clear();
}


void builtin_drop( ContextPtr ctx , std::vector<std::string> const & )
{
    ctx->drop();
}

void builtin_dropN( ContextPtr ctx , std::vector<std::string> const &args )
{
    if ( args.size()!=1 ) {
        throw Rpn::ErrInvalidArguments("DROP_N requires one and only one argument(int)");
    }
    int count(0);
    bool res = Poco::NumberParser::tryParse(args.at(0),count);
    if ( !res || count<1 ) {
        throw Rpn::ErrInvalidArguments("DROP_N need an positive integer argument");
    }

    ctx->dropN(count);
}

void builtin_assignVariable( ContextPtr ctx , std::vector<std::string> const &args )
{
    if ( args.size()!=1 ) {
        throw Rpn::ErrInvalidArguments("DROP_N requires one and only one argument(string)");
    }
    if ( ctx->stackSize()<1 ) {
        throw Rpn::ErrNotEnoughElements("ASSIGN requires one value");
    }
    Operand v    = ctx->pop();
    ctx->assignVariable(args.at(0),v);
}

void builtin_recallVariable( ContextPtr ctx , std::vector<std::string> const &args )
{
    if ( args.size()!=1 ) {
        throw Rpn::ErrInvalidArguments("RECALL_VAR requires one and only one argument(string)");
    }
    ctx->push( ctx->recallVariable(args.at(0)));
}


void builtin_assignConstant( ContextPtr ctx , std::vector<std::string> const &args )
{
    if ( args.size()!=1 ) {
        throw Rpn::ErrInvalidArguments("ASSIGN_CONSTANT requires one and only one argument(string)");
    }
//    if ( ctx->stackSize()<1 ) {
//        throw Rpn::ErrNotEnoughElements("ASSIGN_CONSTANT requires one value");
//    }
    Operand v    = ctx->pop();
    ctx->assignConstant(args.at(0),v);
}


void builtin_recallConstant( ContextPtr ctx , std::vector<std::string> const &args )
{
    if ( args.size()!=1 ) {
        throw Rpn::ErrInvalidArguments("RECALL_VAR requires one and only one argument(string)");
    }
    ctx->push( ctx->recallConstant(args.at(0)) );
}


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

void builtin_push( ContextPtr ctx , std::vector<std::string> const &args )
{
    if ( args.size()!=1 ) {
        throw Rpn::ErrInvalidArguments("PUSH needs exactly one (numeric) argument");
    }

    double value(0);
    bool res = Poco::NumberParser::tryParseFloat(args.at(0),value);
    if ( !res ) {
        throw Rpn::ErrInvalidArguments("DROP_N argument must be numeric");
    }
    ctx->push(value);
}

void builtin_dupN( ContextPtr ctx , std::vector<std::string> const &args )
{
    if ( args.size()!=1 ) {
        throw Rpn::ErrInvalidArguments("DUP_N requires one and only one argument(int)");
    }
    int count(0);
    bool res = Poco::NumberParser::tryParse(args.at(0),count);
    if ( !res || count<1 ) {
        throw Rpn::ErrInvalidArguments("DUP_N need an positive integer argument");
    }
    Operand value = ctx->valueAt(0);
    for ( int i(0) ; i<count ; i++ ) {
        ctx->push(value);
    }
}

void builtin_dup( ContextPtr ctx , std::vector<std::string> const & )
{
    ctx->push( ctx->valueAt(0) );
}


}; /* namespace Rpn; */