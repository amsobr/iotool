//
// Created by to on 03/05/19.
//

#ifndef IOTOOL_COMMAND_HELP_HPP
#define IOTOOL_COMMAND_HELP_HPP


#include <string>
#include <memory>

class CommandHelp
{

private:
    std::string myCmdName;
    std::string myCmdBrief;

public:
    CommandHelp()
    {
    }

    CommandHelp( std::string const &name, std::string const &brief) :
    myCmdName(name),
    myCmdBrief(brief)
    {
    }

    virtual ~CommandHelp()
    {

    }

    void setName(std::string const &name)
    {
        myCmdName   = name;
    }

    void setBrief(std::string const &brief)
    {
        myCmdBrief  = brief;
    }



    std::string const &getName() const
    {
        return myCmdName;
    }

    std::string const &getBrief() const
    {
        return myCmdBrief;
    }
};

typedef std::shared_ptr<CommandHelp> CommandHelpPtr;


#endif //IOTOOL_COMMAND_HELP_HPP
