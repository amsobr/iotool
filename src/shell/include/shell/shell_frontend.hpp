#ifndef SHELL_FRONTEND_HPP
#define SHELL_FRONTEND_HPP

#include <memory>
#include <iostream>
#include <thread>


#include <common/stream_adapter.hpp>

#include "shell_backend.hpp"

class ShellFrontend
{
private:
    StreamAdapter *myStream;
    ShellBackendPtr myEngine;

public:
    ShellFrontend( StreamAdapter *ioAdapter , ShellBackendPtr engine );

    ~ShellFrontend();

    void run();
};

typedef std::shared_ptr<ShellFrontend> ShellFrontendPtr;

#endif /* !defined SHELL_FRONTEND_HPP */