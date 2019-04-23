//
// Created by to on 12/04/19.
//

#include <Poco/Logger.h>
#include <Poco/Format.h>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>
#include <Poco/File.h>
#include <Poco/Dynamic/Var.h>
#include <fstream>

#include "output_channel_manager.hpp"
#include "periodic_scheduler.hpp"
#include "csv_writer.hpp"
#include "output_channel_holder.hpp"

using namespace std;
using Poco::Logger;
using Poco::format;
using Poco::File;
using namespace Poco::JSON;
using namespace Poco::Dynamic;


OutputChannelManager::~OutputChannelManager()
{

}

void OutputChannelManager::workerFunction()
{
    while( !myStopped ) {
        DataBucketPtr bucketPtr = myQueue.getNext();

        for ( auto &slot : myOutputChannels ) {
            if ( slot.jobIsCalled(bucketPtr->name) ) {
                slot.channel()->incomingBucket(bucketPtr);
            }
        }

    }
}


bool OutputChannelManager::loadFromPath(std::string const &path)
{
    Logger &logger  = Logger::get("iotool");

    logger.information(format("OutputChannelManager: loading channels from path '%s'",path));

    File dir(path);
    if ( !dir.isDirectory() ) {
        logger.error( format("OOPS: '%s' doesn't name a directory...",path));
        return false;
    }

    std::vector<Poco::File> allFiles;
    dir.list(allFiles);
    for ( auto file : allFiles ) {
        try {
            if (!file.isFile()) {
                logger.information(format("OutputChannelManager: skipping non-file path '%s'", file.path()));
                continue;
            }

            ifstream fileStream(file.path());

            Parser parser;
            Var parsed = parser.parse(fileStream);

            Object::Ptr root = parsed.extract<Object::Ptr>();
            string channelType = root->getValue<std::string>("channelType");
            string channelName = root->getValue<std::string>("myChannelName");
            string jobName = root->getValue<std::string>("myJobName");
            Object::Ptr channelConfig = root->getObject("parameters");

            OutputChannelPtr ptr;
            if (channelType == "csv_writer") {
                ptr.reset(CsvWriter::loadFromJSON(channelConfig));
            } else {
                logger.warning(
                    format("OutputChannelManager: unknown channel type '%s'. Ignoring config file '%s'", channelType,
                           file.path())
                );
            }

            if (ptr == nullptr) {
                logger.error(format("OutputChannelManager: unable to load CsvWriter from config myChannelName='%s' myJobName='%s' path='%s'",channelName, jobName, file.path()));
            }
            else {
                myOutputChannels.push_back(OutputChannelHolder(channelName, jobName, ptr));
                logger.information(format(
                        "OutputChannelManager: loaded new channel jobName='%s' channelName='%s' channelType='%s' from '%s'",
                        jobName, channelName, channelType, file.path()));
            }
        }
        catch ( std::exception ex ) {
            logger.error(format("OutputChannelManater: Something went very wrong loading channel from '%s' - '%s'",file.path(),ex.what()));
        }
        catch( ... ) {
            logger.error(format("OutputChannelManager: unknwon error loading channel from '%s'",file.path()));
        }
    }
    return true;
}
