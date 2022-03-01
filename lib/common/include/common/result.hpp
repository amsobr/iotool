#ifndef RESULT_HPP
#define RESULT_HPP

#include <string>

#include <Poco/Format.h>

class Result
{
public:
    explicit Result( int code=0 , std::string msg="" ) :
    myCode(code) ,
    myMessage(std::move(msg))
    {
    }

    Result( Result const &other ) :
    myCode(other.myCode)  ,
    myMessage(other.myMessage)
    {
    }

    Result operator=( Result const &other ) 
    {
        myCode      = other.myCode;
        myMessage   = other.myMessage;
        return *this;
    }

    ~Result() = default;

    [[nodiscard]] int code() const { return myCode; }

    [[nodiscard]] std::string message() const { return myMessage; }

    [[nodiscard]] std::string toString() const
    {
        if ( isSuccess() ) {
            if ( !myMessage.empty() ) {
                return Poco::format("%s\nOK",myMessage,myCode);
            }
            else {
                return "OK";
            }
        }
        else {
            return Poco::format("%s\nERROR(%d)",myMessage,myCode);
        }
    }

    [[nodiscard]] explicit operator std::string() const
    {
        return toString();
    }


    [[nodiscard]] bool isSuccess() const
    {
        return (myCode==0);
    }

    static Result const &OK;
    static Result const &E_BAD_ARGS;
    static Result const &E_NOT_SUPPORTED;
    static Result const &E_INVALID_STATE;
    static Result const &E_INVALID_SYNTAX;

private:
    int         myCode;
    std::string myMessage;

}; /* class Result */


#endif /* !defined RESULT_HPP */
