#ifndef RPN_EXCEPTION_HPP
#define RPN_EXCEPTION_HPP

#include <string>
#include <stdexcept>

namespace Rpn
{

class RpnException : public std::runtime_error
{
public:
    RpnException( std::string const &msg ) :
    std::runtime_error(msg)
    {
    }

    RpnException( char const *msg ) :
    std::runtime_error(msg)
    {
    }

    RpnException() : std::runtime_error("") {}

    virtual ~RpnException() {}
}; /* RpnException */


#define DECL_RPN_EXCEPTION(name) \
class name : public RpnException { \
public: \
name( std::string const &msg ) : RpnException(msg) {} \
name( char const *msg ) : RpnException(msg) {} \
name() : RpnException() {} \
virtual ~name() {} };




DECL_RPN_EXCEPTION(ErrNoSuchVariable)
DECL_RPN_EXCEPTION(ErrNotEnoughElements)
DECL_RPN_EXCEPTION(ErrInvalidSyntax)
DECL_RPN_EXCEPTION(ErrMalformedFile)
DECL_RPN_EXCEPTION(ErrInvalidArguments)

}; /* namespace Rpn */




#endif /* !defined RPN_EXCEPTION_HPP */
