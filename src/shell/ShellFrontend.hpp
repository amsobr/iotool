#ifndef SHELL_FRONTEND_HPP
#define SHELL_FRONTEND_HPP

#include <memory>
#include <iostream>
#include <thread>


#include <rps/Context.hpp>


class ShellFrontend
{

public:
    explicit ShellFrontend( rps::ContextPtr ctx ) :
    myCtx{ std::move(ctx) }
    {
    }

    ~ShellFrontend()    = default;

    void run();

private:
    rps::ContextPtr myCtx;
};

using ShellFrontendPtr  = std::shared_ptr<ShellFrontend>;

#endif /* !defined SHELL_FRONTEND_HPP */
