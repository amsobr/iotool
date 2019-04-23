#include <iostream>
#include <mutex>

#include <Poco/Logger.h>
#include <Poco/Format.h>

#include <shell_provider.hpp>
#include <common/string_utils.hpp>
#include "shell_backend_core.hpp"

using namespace std;
using namespace Poco;

ShellBackendCore::ShellBackendCore() :
    myMutex() ,
    logger( Logger::get("iotool"))
{


}

ShellBackendCore::~ShellBackendCore()
{

}

ShellProviderPtr ShellBackendCore::getProvider(std::string const &prefix)
{
    logger.information(format("ShellBackendCore: getting provider for prefix='%s'",prefix));
    lock_guard<recursive_mutex> locker(myMutex);

    for ( auto provider : myProviders ) {
        if ( provider->provides(prefix) ) {
            return provider;
        }
    }
    logger.information(format("ShellBackendCore: no provider found for prefix='%s'",prefix));
    return nullptr;
}

bool ShellBackendCore::addProvider(ShellProviderPtr provider)
{
    if ( logger.information() ) {
        stringstream ss;
        for ( auto s : provider->getPrefixes() ) {
            ss << "\n" << s;
        }
        logger.information(format("ShellBackendCore: adding provider for prefixes='%s'",ss.str()));
    }

    logger.information("ShellBackendCore: checking for prefix collisions...");
    lock_guard<recursive_mutex> locker(myMutex);
    for ( auto prefix : provider->getPrefixes() ) {
        if ( getProvider(prefix)!=nullptr ) {
            logger.error(format("ShellBackendCore: unable to add provider as prefix '%s' already registered.",prefix));
            return false;
        }
    }
    myProviders.push_back(provider);
    logger.information("ShellBackendCore: new shell provider was successfully registered.");
    return true;
}

