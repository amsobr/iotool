
#include <Poco/Logger.h>
#include <Poco/Format.h>

#include <shell_backend_factory.hpp>
#include <shell_backend.hpp>
#include <shell_provider.hpp>

#include "shell_backend_impl.hpp"

using namespace std;
using namespace Poco;

ShellBackendFactory::Impl *ShellBackendFactory::impl;

class ShellBackendFactory::Impl
{
    ShellBackendCore core;
public:
    Impl() :
    core()
    {}

    virtual ~Impl()
    {

    }

    bool addProvider(ShellProviderPtr provider)
    {
        return core.addProvider(provider);
    }

    ShellBackendPtr createInstance()
    {
        return ShellBackendPtr( new ShellBackend(new ShellBackendImpl(&core)) );
    }

}; /* class ShellBackendFactory::Impl */


bool ShellBackendFactory::addProvider(ShellProviderPtr provider)
{
    if ( impl==nullptr ) {
        impl    = new Impl();
    }
    return impl->addProvider(provider);
}

ShellBackendPtr ShellBackendFactory::createInstance()
{
    if ( impl==nullptr ) {
        impl    = new Impl();
    }
    return impl->createInstance();
}
