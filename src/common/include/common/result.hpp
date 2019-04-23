#ifndef RESULT_HPP
#define RESULT_HPP

#include <string>

class Result
{
private:
    int         myCode;
    std::string myMessage;

public:
    Result( int code=0 , std::string msg="" ) :
    myCode(code) ,
    myMessage(msg)
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

    ~Result() { }

    int code() const { return myCode; }

    std::string message() const { return myMessage; }

    std::string toString() const
    {
        return myMessage + (myCode==0 ? "OK\n\n" : "ERROR\n\n");
    }

    operator std::string() const
    {
        return toString();
    }


    bool isSuccess() const
    {
        return (myCode==0);
    }

    static Result const &OK;
    static Result const &E_BAD_ARGS;
    static Result const &E_NOT_SUPPORTED;
    static Result const &E_INVALID_STATE;
    static Result const &E_INVALID_SYNTAX;


}; /* class Result */


#endif /* !defined RESULT_HPP */
