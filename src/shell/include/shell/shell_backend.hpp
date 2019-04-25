#ifndef SHELL_BACKEND_HPP
#define SHELL_BACKEND_HPP

#include <list>
#include <string>
#include <memory>
#include <map>
#include <mutex>

#include <Poco/Logger.h>

#include <common/cmd_arguments.hpp>
#include <common/result.hpp>
#include <common/data_bucket_consumer.hpp>
#include "shell_provider.hpp"

class ShellBackendImpl;
class ShellBackend
{
private:
    ShellBackendImpl *impl;

public:

    ShellBackend( ShellBackendImpl *shellImpl );
    virtual ~ShellBackend();

    Result runCommand(std::string const &cmdLine, DataBucket *outcome=0);


    Result runHelp(std::string const &helpCmd, CmdArguments &args);

    void addConsumer(DataBucketConsumer *consumer);

    friend class ShellBackendFactory;
    //friend class ShellBackendFactory::Impl; //?????????? how to get around this?!?!?!?!?!?

}; /* class ShellBackend */


typedef std::shared_ptr<ShellBackend> ShellBackendPtr;



#endif /* !defined SHELL_BACKEND_HPP */
