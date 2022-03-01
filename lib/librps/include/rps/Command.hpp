//
// Created by to on 18/03/19.
//

#pragma once


#include <string>
#include <functional>
#include <vector>

#include "AbstractCommand.hpp"
#include "ArgumentList.hpp"
#include "Context.hpp"

namespace rps
{

class Stack;

class Command : public AbstractCommand
{
public:
    typedef std::function< void(ContextPtr&, ArgumentList const&) > SimpleHandler;


    Command(Command const& other ) = delete;
    Command& operator=(Command const& rhs ) = delete;

    Command(SimpleHandler handler,
                std::string name,
                std::string brief="",
                std::string help="") :
        AbstractCommand{ std::move(name), std::move(brief), std::move(help) } ,
        myHandler{ std::move(handler) }
    {
    }

    ~Command() override = default;

    void run( ContextPtr& ctx , ArgumentList const& args ) const override
    {
        myHandler(ctx,args);
    }

private:
    SimpleHandler myHandler;
};


} /* namespace rps */

