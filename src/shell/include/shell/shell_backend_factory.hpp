#ifndef SHELL_BACKEND_FACTORY_HPP
#define SHELL_BACKEND_FACTORY_HPP


#include <Poco/Logger.h>

#include "shell_provider.hpp"
#include "shell_backend.hpp"


class ShellBackendFactory
{
private:
    class Impl;
    static Impl *impl;

    ShellBackendFactory();
    ~ShellBackendFactory();

public:
    static bool addProvider( ShellProviderPtr provider );

    static ShellBackendPtr createInstance();

}; /* class ShellBackendFactory */


#endif /* !defined SHELL_BACKEND_FACTORY_HPP */
