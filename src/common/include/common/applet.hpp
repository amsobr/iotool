#ifndef APPLET_HPP
#define APPLET_HPP

#include <string>
#include <memory>

class Applet
{
protected:
    std::string myName;
    std::string myBrief;

    Applet( std::string name , std::string brief="" ) :
    myName(name) ,
    myBrief(brief)
    {
    }


public:

    virtual ~Applet()
    {
    }

    std::string getName() const { return myName; }
    std::string brief() const { return myBrief; }
    virtual std::string help() const { return ""; }

};

typedef std::shared_ptr<Applet> AppletPtr;


#endif /* !defined APPLET_HPP */
