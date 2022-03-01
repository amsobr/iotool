//
// Created by to on 20/03/19.
//

#ifndef IOTOOL_CSV_WRITER_HPP
#define IOTOOL_CSV_WRITER_HPP


#include <fstream>
#include <Poco/JSON/Object.h>

class CsvWriter : public OutputChannel
{
private:
    Poco::Logger &logger;
    std::list<std::string> myFields;
    std::string myPath;
    char mySeparator;
    bool myAppend;
    std::ofstream fileStream;

    CsvWriter();
public:

    virtual ~CsvWriter();

    void incomingBucket(DataBucketPtr db) override;

    static OutputChannel * loadFromJSON(Poco::JSON::Object::Ptr jsonParameters);
};


#endif //IOTOOL_CSV_WRITER_HPP
