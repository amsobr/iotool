//
// Created by to on 22/02/22.
//

#include <rps/Command.hpp>
#include <rps/Exception.hpp>
#include <rps/BuiltinCommands.hpp>

namespace rps
{

static inline void builtin_clear( ContextPtr const& ctx , ArgumentList const& )
{
    ctx->stack.clear();
}


static inline void builtin_drop( ContextPtr const& ctx , ArgumentList const& )
{
    ctx->stack.drop();
}

static inline void builtin_dropN( ContextPtr const& ctx , ArgumentList const&args )
{
    if ( args.size()!=1 ) {
        throw rps::InvalidArgumentsException("DROP_N requires one and only one argument(int)");
    }
    int count   = args.at(0).getAsInteger();
    if ( count<1 ) {
        throw rps::InvalidArgumentsException("DROP_N need an positive integer argument");
    }
    
    ctx->stack.dropN(count);
}

static inline void builtin_setVar( ContextPtr const& ctx , ArgumentList const&args )
{
    if ( args.size()!=1 ) {
        throw rps::InvalidArgumentsException("DROP_N requires one and only one argument(string)");
    }
    if ( ctx->stack.stackSize()<1 ) {
        throw rps::NotEnoughElementsException("setvar requires one value");
    }
    Operand v    = ctx->stack.pop();
    ctx->stack.assignVariable(args.at(0).getToken(),v);
}

static inline void builtin_getVar( ContextPtr const& ctx , ArgumentList const&args )
{
    if ( args.size()!=1 ) {
        throw rps::InvalidArgumentsException("RECALL_VAR requires one and only one argument(string)");
    }
    ctx->stack.push( ctx->stack.recallVariable(args.at(0).getToken()));
}


static inline void builtin_setConst( ContextPtr const& ctx , ArgumentList const&args )
{
    if ( args.size()!=1 ) {
        throw rps::InvalidArgumentsException("ASSIGN_CONSTANT requires one and only one argument(string)");
    }
//    if ( ctx->stackSize()<1 ) {
//        throw Rpn::ErrNotEnoughElements("ASSIGN_CONSTANT requires one value");
//    }
    Operand v    = ctx->stack.pop();
    ctx->stack.assignConstant(args.at(0).getKey(),v);
}


static inline void builtin_getConst( ContextPtr const& ctx , ArgumentList const&args )
{
    if ( args.size()!=1 ) {
        throw rps::InvalidArgumentsException("RECALL_VAR requires one and only one argument(string)");
    }
    ctx->stack.push( ctx->stack.recallConstant(args.at(0).getKey()) );
}


static inline void builtin_add( ContextPtr const& ctx , ArgumentList const& )
{
    Operand p0 = ctx->stack.pop();
    Operand p1 = ctx->stack.pop();
    
    Operand result = p1+p0;
    ctx->stack.push(result);
}


static inline void builtin_sub( ContextPtr const& ctx , ArgumentList const& )
{
    Operand p0 = ctx->stack.pop();
    Operand p1 = ctx->stack.pop();
    
    Operand result = p1-p0;
    ctx->stack.push(result);
}


static inline void builtin_mul( ContextPtr const& ctx , ArgumentList const& )
{
    Operand p0 = ctx->stack.pop();
    Operand p1 = ctx->stack.pop();
    
    Operand result = p1*p0;
    ctx->stack.push(result);
}

static inline void builtin_div( ContextPtr const& ctx , ArgumentList const& )
{
    Operand p0 = ctx->stack.pop();
    Operand p1 = ctx->stack.pop();
    
    Operand result = p1/p0;
    ctx->stack.push(result);
}

void builtin_neg( ContextPtr const& ctx , ArgumentList const& )
{
    Operand p0 = ctx->stack.pop();
    
    Operand result = -p0;
    ctx->stack.push(result);
}

void builtin_push( ContextPtr const& ctx , ArgumentList const&args )
{
    if ( args.size()!=1 ) {
        throw rps::InvalidArgumentsException("PUSH needs exactly one (numeric) argument");
    }
    
    double value = args.at(0).getAsDouble();
    ctx->stack.push(value);
}

void builtin_dupN( ContextPtr const& ctx , ArgumentList const&args )
{
    if ( args.size()!=1 ) {
        throw rps::InvalidArgumentsException("DUP_N requires one and only one argument(int)");
    }
    int count   = args.at(0).getAsInteger();
    if ( count<1 ) {
        throw rps::InvalidArgumentsException("DUP_N need an positive integer argument");
    }
    Operand value = ctx->stack.valueAt(0);
    for ( int i(0) ; i<count ; i++ ) {
        ctx->stack.push(value);
    }
}

void builtin_dup( ContextPtr const& ctx , ArgumentList const& )
{
    ctx->stack.push( ctx->stack.valueAt(0) );
}

constexpr auto showHelp =
    "show - show information about the current context\n"
    "Usage:\n"
    "  show WHAT\n"
    "Arguments:\n"
    "  WHAT    Selects what to show:\n"
    "          vars|variables   : show current variables\n"
    "          consts|constants : show current constants\n"
    "          stack            : show the stack, from oldest to newest\n"
    "          all              : show all the above\n";

void builtin_show( ContextPtr const& ctx, ArgumentList const& args )
{
    if ( args.size()!=1 ) {
        throw InvalidArgumentsException{"show needs exactly one argument"};
    }
    
    if ( !args.at(0).isToken() ) {
        throw InvalidArgumentsException{"show needs exactly one token argument (got one KV pair)"};
    }
    
    auto const& mode    = args.at(0).getToken();
    bool showStack      = false;
    bool showVars       = false;
    bool showConsts     = false;
    if ( mode=="stack" ) {
        showStack   = true;
    }
    else if ( mode=="vars" || mode=="variables" ) {
        showVars    = true;
    }
    else if ( mode=="consts" || mode=="constants" ) {
        showConsts  = true;
    }
    else if ( mode=="stack" ) {
        showStack   = true;
    }
    else if ( mode=="all" ) {
        showStack = showVars = showConsts = true;
    }
    else {
        throw InvalidArgumentsException{"invalid mode: " + mode};
    }
    
    if ( showVars ) {
        ctx->stream->writeLine("Variables:");
        for ( auto const& var: ctx->stack.getVariables() ) {
            ctx->stream->writeLine(Poco::format("  '%12s' = %f\n",var.name(),var.value()));
        }
        ctx->stream->writeLine("");
    }
    
    if ( showConsts ) {
        auto allConsts  = ctx->stack.getConstants();
        ctx->stream->writeLine("Constants:");
        for ( auto const& ct : ctx->stack.getConstants() ) {
            ctx->stream->writeLine(Poco::format("  '%12s' = %f",ct.name(),ct.value()));
        }
        ctx->stream->writeLine("\n");
    }
    
    if ( showStack ) {
        ctx->stream->writeLine("Stack:");
        auto stack  = ctx->stack.getStack();
        auto depth  = stack.size() - 1;
        for ( auto const& v : stack ) {
            ctx->stream->writeLine(Poco::format("[%02z]: %f",depth,v));
            depth--;
        }
    }
}


typedef std::function< void(ContextPtr&,ArgumentList const&) > CommandHandler;
typedef std::tuple< CommandHandler, std::string , std::string, std::string > CmdDescriptor;


std::vector<AbstractCommandConstPtr> getBuiltinCommands()
{
    static std::vector<CmdDescriptor> descriptors {
        { builtin_clear , "clear"   , "clear the stack and variables"   , "" } ,
        { builtin_drop  , "drop"    , "drop item 0 from stack"          , "" } ,
        { builtin_dropN , "dropn"   , "drop N elements from stack"      , "" } ,
        { builtin_setVar, "setvar"  , "assign [0] to variable"          , "" } ,
        { builtin_getVar, "getvar"  , "recall variable to stack"        , "" } ,
        { builtin_setConst, "setconst" , "assign one constant (write only)" , "" } ,
        { builtin_getConst, "getconst" , "push constant to stack"       , "" } ,
        { builtin_add   , "add"     , "add: [0] + [1] -> [0]"            , "" } ,
        { builtin_sub   , "sub"     , "subtract: [0] - [1] -> [0]"       , "" } ,
        { builtin_mul   , "mul"     , "multiply: [0] * [1] -> [0]"      , "" } ,
        { builtin_div   , "div"     , "divide: [1] / [0] -> [0]"        , "" } ,
        { builtin_neg   , "neg"     , "negate sign: -1 * [0] -> [0]"    , "" } ,
        { builtin_push  , "push"    , "push arg (int) to [0]"           , "" } ,
        { builtin_dupN  , "dupn"    , "duplicate [0] N times"           , "" } ,
        { builtin_dup   , "dup"     , "duplicate [0]"                   , "" } ,
        { builtin_show  , "show"    , "show context information"        , showHelp } ,
    };
    
    std::vector<AbstractCommandConstPtr> allCmds;
    allCmds.reserve(descriptors.size());
    for ( auto [ hnd , name , brief , help ] : descriptors ) {
        allCmds.emplace_back(std::make_shared<Command>(hnd,name,brief,help));
    }
    return allCmds;
}



} /* namespace rps */
