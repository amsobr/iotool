//
// Created by to on 05/05/19.
//

#ifndef IOTOOL_OPTION_HELP_HPP
#define IOTOOL_OPTION_HELP_HPP


#include <vector>
#include "option_value_descriptor.hpp"

class OptionHelp
{
private:
    std::string myName;
    std::string myDescription;
    std::vector<OptionValueDescriptorPtr> *myValues;

public:
    OptionHelp(std::string const &name , std::string const &desc , std::vector<OptionValueDescriptorPtr> *values= nullptr ) :
    myName(name) ,
    myDescription(desc)
    {
        if ( values!=nullptr ) {
            myValues    = new std::vector<OptionValueDescriptorPtr>(*values);
        }
    }

    virtual ~OptionHelp()
    {
        if ( myValues!= nullptr ) {
            delete myValues;
        }
    }

    const std::string &name() const
    {
        return myName;
    }

    const std::string &description() const
    {
        return myDescription;
    }

    std::vector<OptionValueDescriptorPtr> getValues() const
    {
        return *myValues;
    }

};


typedef std::shared_ptr<OptionHelp> OptionHelpPtr;


#endif //IOTOOL_OPTION_HELP_HPP
