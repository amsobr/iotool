//
// Created by to on 20/03/19.
//

#include <Poco/Logger.h>
#include <Poco/Format.h>

#include "csv_writer.hpp"
#include "periodic_scheduler.hpp"

using namespace std;
using Poco::Logger;
using Poco::format;
using namespace Poco::JSON;

void CsvWriter::incomingBucket(DataBucketPtr db)
{
    logger.information("CsvWriter(%s): received bucket:\n%s",myPath,db->toString());
    std::stringstream ss;
    ss << db->timestamp.epochMicroseconds()/1000;
    for ( std::string fieldName : myFields ) {
        ss << mySeparator << db->getDataPoint(fieldName);
    }
    fileStream << ss.str() << "\n";
    //fileStream.flush();
    //logger.information(format("CsvWriter: failbit=%b",fileStream.fail()));
}

CsvWriter::CsvWriter() :
logger(Logger::get("iotool"))
{

}

CsvWriter::~CsvWriter()
{
    fileStream.flush();
    fileStream.close();
}

OutputChannel *CsvWriter::loadFromJSON(Poco::JSON::Object::Ptr config)
{
    Logger &logger = Logger::get("iotool");

    CsvWriter *writer   = new CsvWriter();
    try {
        logger.information("CsvWriter: loading from JSON...");

        writer->myPath  = config->getValue<string>("fileName");
        string separator= config->getValue<string>("fieldSeparator");
        if ( separator=="tab" ) {
            writer->mySeparator = '\t';
        }
        else if ( separator=="colon" ) {
            writer->mySeparator = ':';
        }
        else if ( separator=="semicolon" ) {
            writer->mySeparator = ';';
        }
        else if ( separator=="space" ) {
            writer->mySeparator = ' ';
        }
        else if ( separator=="comma" ) {
            writer->mySeparator = ',';
        }
        else {
            throw runtime_error("Invalid value for 'separator': " + separator);
        }

        writer->myAppend = config->getValue<bool>("append");

        Array::Ptr jsonFields = config->getArray("fields");
        for (auto it = jsonFields->begin(); it != jsonFields->end(); it++) {
            writer->myFields.push_back((*it).convert<string>());
        }

        std::ios_base::openmode mode = std::fstream::out;
        mode |= writer->myAppend ? std::fstream::app : std::fstream::trunc;

        writer->fileStream.open(writer->myPath,mode);
        logger.information("CsvWriter: Things are looking good. writer was loaded.");
        return writer;
    }
    catch ( std::exception ex )
    {
        logger.error( format("CsvWriter: Something went wrong - %s",ex.what()));
    }
    delete writer;
    return nullptr;
}
