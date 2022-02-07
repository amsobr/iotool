//
// Created by to on 18/03/19.
//

#ifndef IOTOOL_BUILTIN_INSTRUCTIONS_HPP
#define IOTOOL_BUILTIN_INSTRUCTIONS_HPP

#include <vector>
#include <string>
#include <rpn/context.hpp>
#include <rpn/ArgumentList.h>
#include "rpn/operand.hpp"



namespace Rpn
{
typedef void (BuiltinFunction)(Context&,ArgumentList const&);

extern void builtin_clear( Context& ctx , ArgumentList const& );
extern void builtin_drop( Context& ctx , ArgumentList const& );
extern void builtin_dropN( Context& ctx , ArgumentList const&args );
extern void builtin_assignVariable( Context& ctx , ArgumentList const&args );
extern void builtin_recallVariable( Context& ctx , ArgumentList const&args );
extern void builtin_assignConstant( Context& ctx , ArgumentList const&args );
extern void builtin_recallConstant( Context& ctx , ArgumentList const&args );
extern void builtin_add( Context& ctx , ArgumentList const& );
extern void builtin_sub( Context& ctx , ArgumentList const& );
extern void builtin_mul( Context& ctx , ArgumentList const& );
extern void builtin_div( Context& ctx , ArgumentList const& );
extern void builtin_neg( Context& ctx , ArgumentList const& );
extern void builtin_push( Context& ctx , ArgumentList const& );
extern void builtin_dupN( Context& ctx , ArgumentList const&args );
extern void builtin_dup( Context& ctx , ArgumentList const& );


}; /* namespace Rpn; */




#endif //IOTOOL_BUILTIN_INSTRUCTIONS_HPP
