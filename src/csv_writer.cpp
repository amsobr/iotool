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
    std::stringstream ss;
    ss << db->isoTimestamp();
    for ( std::string fieldName : myFields ) {
        ss << '\t' << db->getDataPoint(fieldName);
    }
    ss << '\n';
    fileStream << ss.str();
}

CsvWriter::CsvWriter()
{}

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
        if ( separator=="tabl" ) {
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
