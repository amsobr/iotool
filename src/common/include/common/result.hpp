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

}; /* class Result */


#endif /* !defined RESULT_HPP */
