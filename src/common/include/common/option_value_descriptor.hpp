//
// Created by to on 05/05/19.
//

#ifndef IOTOOL_OPTION_VALUE_DESCRIPTOR_HPP
#define IOTOOL_OPTION_VALUE_DESCRIPTOR_HPP


#include <memory>

class OptionValueDescriptor
{
private:
    std::string myName;
    std::string myDescription;

public:
    OptionValueDescriptor(const std::string &myName) : myName(myName)
    {}

    virtual ~OptionValueDescriptor()
    {

    }

    const std::string &name() const
    {
        return myName;
    }

    void setName(const std::string &name)
    {
        myName = name;
    }

    const std::string &description() const
    {
        return myDescription;
    }

    void setDescription(const std::string &description)
    {
        myDescription = description;
    }


};

typedef std::shared_ptr<OptionValueDescriptor> OptionValueDescriptorPtr;

#endif //IOTOOL_OPTION_VALUE_DESCRIPTOR_HPP
