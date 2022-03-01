
#pragma once

#include <string>
#include <stdexcept>

namespace rps
{

class Exception : public std::runtime_error
{
public:
    explicit Exception(std::string const &msg ) :
        std::runtime_error(msg) ,
        myType{"Exception"}
    {
    }

    Exception(std::string type , std::string const &msg ) :
        std::runtime_error{ msg } ,
        myType{ std::move(type) }
    {
    }

    explicit Exception(char const *msg ) :
    std::runtime_error(msg)
    {
    }

    Exception() : std::runtime_error("") {}

    ~Exception() override = default;

    [[nodiscard]] std::string getText() const { return what(); }

    [[nodiscard]] std::string getType() const { return myType; };

private:
    std::string myType;
}; /* Exception */


#define DECL_RPN_EXCEPTION(name) \
class name : public Exception { \
public: \
explicit name( std::string const &msg ) : Exception(#name,msg) {} \
explicit name( char const *msg ) : Exception(#name,std::string{msg}) {} \
explicit name() : Exception(#name,"") {} \
~name() override = default; };




DECL_RPN_EXCEPTION(NoSuchVariableException)
DECL_RPN_EXCEPTION(NotEnoughElementsException)
DECL_RPN_EXCEPTION(InvalidSyntaxException)
DECL_RPN_EXCEPTION(MalformedFileException)
DECL_RPN_EXCEPTION(InvalidArgumentsException)
DECL_RPN_EXCEPTION(NotFoundException)
DECL_RPN_EXCEPTION(AlreadyExistsException)
DECL_RPN_EXCEPTION(PipeException)

} /* namespace rps */


