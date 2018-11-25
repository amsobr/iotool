#ifndef CMD_RESULT_HPP
#define CMD_RESULT_HPP

#include <string>

class CmdResult
{
private:
    int         myCode;
    std::string myMessage;

public:
    CmdResult( int code=0 , std::string msg="" ) :
    myCode(code) ,
    myMessage(msg)
    {
    }

    CmdResult( CmdResult const &other ) :
    myCode(other.myCode)  ,
    myMessage(other.myMessage)
    {
    }

    CmdResult operator=( CmdResult const &other ) 
    {
        myCode      = other.myCode;
        myMessage   = other.myMessage;
        return *this;
    }

    ~CmdResult() { }

    int code() const { return myCode; }

    std::string message() const { return myMessage; }

}; /* class CmdResult */


#endif /* !defined CMD_RESULT_HPP */
