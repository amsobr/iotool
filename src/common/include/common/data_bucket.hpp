#ifndef DATA_BUCKET_HPP
#define DATA_BUCKET_HPP

#include <string>
#include <vector>
#include <list>
#include <sstream>

#include <Poco/Timestamp.h>
#include <Poco/NumberFormatter.h>
#include <Poco/Format.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/DateTimeFormat.h>

#include "peripheral.hpp"


static inline std::string generateLabel(std::string const &myLabel , Peripheral const *p)
{
    if ( p==nullptr )
        return myLabel;
    else
        return p->getName()+"."+myLabel;
}

static inline std::string generateLabel( unsigned int idx , Peripheral const *p)
{
    if ( p==nullptr )
        return Poco::NumberFormatter::format(idx);
    else
        return p->getName()+"."+Poco::NumberFormatter::format(idx);
}

template<typename T>
static inline std::string formatValue( T const &myValue )
{
    return Poco::NumberFormatter::format(myValue);
}

template<>
inline std::string formatValue<double>(double const &myValue)
{
    return Poco::NumberFormatter::format(myValue,10);
}

template<>
inline std::string formatValue<std::string>(std::string const &myValue)
{
    return myValue;
}

struct DataPoint
{
    std::string myLabel;
    std::string myValue;

    DataPoint(std::string l, std::string v) : myLabel(l) , myValue(v) {}
    DataPoint( DataPoint const &o ) : myLabel(o.myLabel) , myValue(o.myValue) {}
    ~DataPoint() {}

    std::string label() const { return myLabel; }
    std::string value() const { return myValue; }
};

struct DataBucket
{
    /** UNIX timestamp, in microseconds */
    Poco::Timestamp timestamp;

    /** The bucket's name */
    std::string name;

    std::list<DataPoint> dataPoints;

    DataBucket( std::string &t )
    {
        reset(t);
    }

    DataBucket( DataBucket const &o ) :
    timestamp(o.timestamp) ,
    name(o.name) ,
    dataPoints(o.dataPoints)
    {}

    DataBucket operator=( DataBucket const &rhs ) 
    {
        timestamp   = rhs.timestamp;
        name         = rhs.name;
        dataPoints  = rhs.dataPoints;
        return *this;
    }

    
    DataBucket() : DataBucket("") {}

    virtual ~DataBucket() {}

    void reset( std::string t="" )
    {
        name = t;
        timestamp.update();
        dataPoints.clear();
    }

    template<typename T>
    void addDataPoint(std::string const &myLabel , T const &myValue , Peripheral const *p)
    {
        dataPoints.push_back( DataPoint(
                                    generateLabel(myLabel,p) ,
                                    formatValue(myValue) )
                            );
    }

    template<typename T>
    void addDataPoint( std::string const &myLabel , std::vector<T> const &values , Peripheral const *p )
    {
        std::string labelVal( generateLabel(myLabel,p) );
        
        if (values.empty()) {
            dataPoints.push_back(DataPoint(labelVal,""));
        }
        else {
            std::ostringstream ss;
            auto it = values.begin();
            ss << *it;
            it++;
            while ( it!=values.end() ) {
                ss << ";" << formatValue(*it);
                it++;
            }
            std::string str = ss.str();
            dataPoints.push_back(DataPoint(labelVal,str));
        }
    }
    
    template<typename T>
    void addDataPoint( unsigned int id , T const &myValue , Peripheral const *p )
    {
        std::string myLabel( generateLabel(id,p) );
        dataPoints.push_back( DataPoint(myLabel,formatValue(myValue)) );
    }

    std::string isoTimestamp() const
    {
        return Poco::DateTimeFormatter::format(timestamp,Poco::DateTimeFormat::ISO8601_FORMAT);
    }
    
}; /* struct DataBucket */

typedef std::shared_ptr<DataBucket> DataBucketPtr;



#endif /* DATA_BUCKET_HPP */
