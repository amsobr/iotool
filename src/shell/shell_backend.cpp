#include <iostream>
#include <mutex>

#include <Poco/Logger.h>
#include <Poco/Format.h>

#include <shell_backend.hpp>
#include <shell_provider.hpp>
#include <common/string_utils.hpp>
#include <shell_backend.hpp>

#include "shell_backend_core.hpp"
#include "shell_backend_impl.hpp"

using namespace std;
using namespace Poco;


ShellBackend::ShellBackend(ShellBackendImpl *shellImpl) :
impl(shellImpl)
{

}

ShellBackend::~ShellBackend()
{
    delete impl;

}


Result ShellBackend::runCommand(string const &cmdLine)
{
    return impl->runCommand(cmdLine);
}

Result ShellBackend::runHelp(std::string const &helpCmd, CmdArguments &args)
{
    return impl->runHelp(helpCmd,args);
}

void ShellBackend::addConsumer(DataBucketConsumer *consumer)
{
    impl->addConsumer(consumer);
}

