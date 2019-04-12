//
// Created by to on 20/03/19.
//

#include "include/csv_writer.hpp"

void CsvWriter::incomingBucket(DataBucket &db)
{
    std::stringstream ss;
    ss << db->isoTimestamp();
    for ( std::string fieldName : myFields ) {
        ss << '\t' << db->getDataPoint(fieldName);
    }
    ss << '\n';
    fileStream << ss.str();
}

CsvWriter::CsvWriter( const std::string &name, const std::list<std::string> &myFields, const std::string &myPath ) :
        OutputChannel(name),
        myFields(myFields),
        myPath(myPath),
        fileStream(myPath)
{}

CsvWriter::~CsvWriter()
{
    fileStream.flush();
    fileStream.close();
}
