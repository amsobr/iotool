//
// Created by to on 03/03/21.
//

#pragma once

#include <memory>
#include <string>
#include <utility>

#include "ArgumentList.hpp"
#include "RpsFwd.hpp"

namespace rps
{

class AbstractCommand
{
public:
    explicit AbstractCommand(std::string  name ,
                std::string  brief="" ,
                std::string  help="" ) :
        myName{ std::move( name ) } ,
        myBrief{ std::move( brief ) } ,
        myHelp{ std::move( help ) }
    {
    }

    virtual ~AbstractCommand();

    [[nodiscard]] std::string getName() const { return myName; }

    [[nodiscard]] std::string getBrief() const { return myBrief; }

    [[nodiscard]] std::string help() const { return myHelp; }

    /**
     * Execute the instruction on the given context with the provided arguments.
     *
     * @note Instructions @em MUST be thread-safe, therefore the run method
     * is const by definition
     * @param ctx The execution context
     * @param args Arguments provided in the call site
     * @throws TBD - pending docs
     */
    virtual void run( ContextPtr& ctx , ArgumentList const& args ) const = 0;


protected:
    void setBrief( std::string brief )
    {
        myBrief = std::move(brief);
    }

    void setHelp( std::string help )
    {
        myHelp  = std::move(help);
    }

private:
    std::string myName;
    std::string myBrief;
    std::string myHelp;
};

} /* namespace rps */


