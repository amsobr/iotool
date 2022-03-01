#ifndef PERIPHERAL_HPP
#define PERIPHERAL_HPP

#include <memory>
#include <string>

#include <Poco/Format.h>

#include "PeripheralType.hpp"
#include "configurable_resource.hpp"

class Peripheral: public ConfigurableResource
{
public:
    Peripheral( PeripheralType const& t , int id ) :
    myType{ t } ,
    myId{ id }
    {}

    ~Peripheral() override = default;

    void setAlias( std::string alias )
    {
        myAlias = std::move(alias);
    }

    [[nodiscard]] PeripheralType const& getType() const { return myType; }
    [[nodiscard]] int getId() const { return myId; }
    [[nodiscard]] std::string const& getAlias() const { return myAlias; }
    [[nodiscard]] virtual std::string getVendor() const { return ""; }
    [[nodiscard]] virtual std::string getModel() const { return "";}
    [[nodiscard]] virtual std::string getRevision() const { return ""; }
    [[nodiscard]] virtual std::string getDriverVersion() const { return ""; }
    [[nodiscard]] virtual std::string getAuthor() const { return ""; }

private:
    Peripheral()    = default;

    PeripheralType  myType;
    int             myId;
    std::string     myAlias{};

}; /* class Peripheral */

typedef std::shared_ptr<Peripheral> PeripheralPtr;

#endif /* !defined PERIPHERAL_HPP */
