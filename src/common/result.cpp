#include <result.hpp>

Result const &Result::OK = Result(0,"");
Result const &Result::E_BAD_ARGS = Result(1,"Invalid Arguments");
Result const &Result::E_NOT_SUPPORTED = Result(2,"Not Supported");
Result const &Result::E_INVALID_STATE = Result(3,"Invalid State");
Result const &Result::E_INVALID_SYNTAX = Result(4,"Invalid Syntax");
