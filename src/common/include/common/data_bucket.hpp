#ifndef DATA_BUCKET_HPP
#define DATA_BUCKET_HPP

#include <string>
#include <vector>
#include <list>
#include <sstream>

#include <Poco/Timestamp.h>
#include <Poco/NumberFormatter.h>
#include <Poco/Format.h>

#include "peripheral.hpp"


static inline std::string generateLabel(std::string const &label , Peripheral const *p)
{
    if ( p==nullptr )
        return label;
    else
        return p->getName()+"."+label;
}

static inline std::string generateLabel( unsigned int idx , Peripheral const *p)
{
    if ( p==nullptr )
        return Poco::NumberFormatter::format(idx);
    else
        return p->getName()+"."+Poco::NumberFormatter::format(idx);
}

template<typename T>
static inline std::string formatValue( T const &value )
{
    return Poco::NumberFormatter::format(value);
}

template<>
inline std::string formatValue<double>(double const &value)
{
    return Poco::NumberFormatter::format(value,10);
}

template<>
inline std::string formatValue<std::string>(std::string const &value)
{
    return value;
}

struct DataPoint
{
    std::string const label;
    std::string const value;

    DataPoint(std::string l, std::string v) : label(l) , value(v) {}
    DataPoint( DataPoint const &o ) : label(o.label) , value(o.value) {}
    ~DataPoint() {}
};

struct DataBucket
{
    /** UNIX timestamp, in microseconds */
    Poco::Timestamp timestamp;

    /** The bucket's name */
    std::string tag;

    std::list<DataPoint> dataPoints;

    DataBucket( std::string t )
    {
        reset(t);
    }

    DataBucket() : DataBucket("") {}

    virtual ~DataBucket() {}

    void reset( std::string t="" )
    {
        tag = t;
        timestamp.update();
        dataPoints.clear();
    }

    template<typename T>
    void addDataPoint(std::string const &label , T const &value , Peripheral const *p)
    {
        dataPoints.push_back( DataPoint(
                                    generateLabel(label,p) ,
                                    formatValue(value) )
                            );
    }

    template<typename T>
    void addDataPoint( std::string const &label , std::vector<T> const &values , Peripheral const *p )
    {
        std::string labelVal( generateLabel(label,p) );
        
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
    void addDataPoint( unsigned int id , T const &value , Peripheral const *p )
    {
        std::string label( generateLabel(id,p) );
        dataPoints.push_back( DataPoint(label,formatValue(value)) );
    }
    
}; /* struct DataBucket */



#endif /* DATA_BUCKET_HPP */
