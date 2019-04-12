//
// Created by to on 20/03/19.
//

#ifndef IOTOOL_CSV_WRITER_HPP
#define IOTOOL_CSV_WRITER_HPP


#include <common/output_channel.hpp>
#include <fstream>

class CsvWriter : public OutputChannel
{
private:
    std::list<std::string> myFields;
    std::string myPath;
    std::ofstream fileStream;

public:
    CsvWriter(const std::string &name, const std::list<std::string> &myFields, const std::string &myPath);

    virtual ~CsvWriter();

public:
    void incomingBucket(DataBucket &db) override;
};


#endif //IOTOOL_CSV_WRITER_HPP
