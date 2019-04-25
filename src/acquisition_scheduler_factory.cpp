//
// Created by to on 19/04/19.
//

#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <fstream>
#include <Poco/File.h>


#include "acquisition_scheduler_factory.hpp"
#include "periodic_scheduler.hpp"


using namespace Poco::JSON;
using Poco::Logger;
using Poco::format;
using Poco::Dynamic::Var;
using Poco::File;
using std::string;
using std::vector;






AcquisitionScheduler::Ptr loadScheduler(const string &path, ShellBackendPtr shellBackend)
{
    Logger &logger = Logger::get("iotool");

    logger.information( format("Loading scheduler definitions from '%s'",path));

    std::ifstream fileStream(path);

    Parser parser;
    Var parsed = parser.parse(fileStream);

    Object::Ptr root        = parsed.extract<Object::Ptr>();
    string schedType        = root->getValue<std::string>("schedulerType");
    Object::Ptr schedConfig = root->getObject("schedulerConfig");

    if ( schedType=="periodic") {
        return PeriodicScheduler::loadFromJSON(schedConfig, shellBackend);
    }
    else {
        return nullptr;
    }
}





std::vector<AcquisitionScheduler::Ptr>
AcquisitionSchedulerFactory::loadFromPath(std::string const &path, ShellBackendPtr shellBackend)
{
    std::vector<AcquisitionScheduler::Ptr> schedulers;
    Logger &logger  = Logger::get("iotool");

    File dir(path);
    if ( !dir.isDirectory() ) {
        logger.error( format("OOPS: '%s' doesn't name a directory...",path));
        return schedulers;
    }

    std::vector<Poco::File> allFiles;
    dir.list(allFiles);
    for ( auto file : allFiles ) {
        if ( !file.isFile() ) continue;
        AcquisitionScheduler::Ptr p = loadScheduler(file.path(), shellBackend);
        if ( p!= nullptr ) {
            logger.information( format("loaded scheduler from '%s'",file.path()));
            schedulers.push_back(p);
        }
        else {
            logger.warning( format("Failed to load scheduler from '%s'",file.path()));
        }
    }
    return schedulers;
}
