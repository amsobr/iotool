#ifndef SHELL_BACKEND_CORE_HPP
#define SHELL_BACKEND_CORE_HPP

#include <list>
#include <string>
#include <memory>
#include <map>
#include <mutex>

#include <Poco/Logger.h>

#include "shell_provider.hpp"


class ShellBackendCore
{
private:
    std::recursive_mutex myMutex;

    std::vector<ShellProviderPtr> myProviders;

    Poco::Logger &logger;

public:
    ShellBackendCore();
    ~ShellBackendCore();

    bool addProvider(ShellProviderPtr provider);

    ShellProviderPtr getProvider(const std::string &prefix);

}; /* class ShellBackendCore */


typedef std::shared_ptr<ShellBackendCore> ShellBackendCorePtr;



#endif /* !defined SHELL_BACKEND_CORE_HPP */
