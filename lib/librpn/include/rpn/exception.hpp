#ifndef RPN_EXCEPTION_HPP
#define RPN_EXCEPTION_HPP

#include <string>
#include <stdexcept>

namespace Rpn
{

class RpnException : public std::runtime_error
{
public:
    explicit RpnException( std::string const &msg ) :
    std::runtime_error(msg)
    {
    }

    explicit RpnException( char const *msg ) :
    std::runtime_error(msg)
    {
    }

    RpnException() : std::runtime_error("") {}

    ~RpnException() override = default;

    std::string getText() const { return what(); }
}; /* RpnException */


#define DECL_RPN_EXCEPTION(name) \
class name : public RpnException { \
public: \
explicit name( std::string const &msg ) : RpnException(msg) {} \
explicit name( char const *msg ) : RpnException(msg) {} \
explicit name() : RpnException() {} \
~name() override = default; };




DECL_RPN_EXCEPTION(ErrNoSuchVariable)
DECL_RPN_EXCEPTION(ErrNotEnoughElements)
DECL_RPN_EXCEPTION(ErrInvalidSyntax)
DECL_RPN_EXCEPTION(ErrMalformedFile)
DECL_RPN_EXCEPTION(ErrInvalidArguments)
DECL_RPN_EXCEPTION(ErrNotFound)
DECL_RPN_EXCEPTION(ErrAlreadyExists)

} /* namespace Rpn */




#endif /* !defined RPN_EXCEPTION_HPP */
