//
// Created by to on 18/03/19.
//

#ifndef IOTOOL_BUILTIN_INSTRUCTIONS_HPP
#define IOTOOL_BUILTIN_INSTRUCTIONS_HPP

#include <vector>
#include <string>
#include <rpn-processor/context.hpp>

#include "rpn-processor/operand.hpp"


namespace Rpn
{
typedef void (BuiltinFunction)(ContextPtr,std::vector<std::string> const &);

extern void builtin_clear( ContextPtr ctx , std::vector<std::string> const & );
extern void builtin_drop( ContextPtr ctx , std::vector<std::string> const & );
extern void builtin_dropN( ContextPtr ctx , std::vector<std::string> const &args );
extern void builtin_assignVariable( ContextPtr ctx , std::vector<std::string> const &args );
extern void builtin_recallVariable( ContextPtr ctx , std::vector<std::string> const &args );
extern void builtin_assignConstant( ContextPtr ctx , std::vector<std::string> const &args );
extern void builtin_recallConstant( ContextPtr ctx , std::vector<std::string> const &args );
extern void builtin_add( ContextPtr ctx , std::vector<std::string> const & );
extern void builtin_sub( ContextPtr ctx , std::vector<std::string> const & );
extern void builtin_mul( ContextPtr ctx , std::vector<std::string> const & );
extern void builtin_div( ContextPtr ctx , std::vector<std::string> const & );
extern void builtin_neg( ContextPtr ctx , std::vector<std::string> const & );
extern void builtin_push( ContextPtr ctx , std::vector<std::string> const & );
extern void builtin_dupN( ContextPtr ctx , std::vector<std::string> const &args );
extern void builtin_dup( ContextPtr ctx , std::vector<std::string> const & );


}; /* namespace Rpn; */




#endif //IOTOOL_BUILTIN_INSTRUCTIONS_HPP
