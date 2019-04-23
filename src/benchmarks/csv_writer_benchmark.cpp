//
// Created by to on 20/03/19.
//


#include <string>
#include <iostream>

#include <Poco/Stopwatch.h>

#include "../csv_writer.hpp"

int main(int argc , char const *argv[] )
{
    if ( argc<3 ) {
        fprintf( stderr , "Usage: CMD CSV_FILE_NAME NUM_ENTRIES\n" );
        std::exit(1);
    }

    std::string csvFilePath(argv[1]);

    OutputChannelPtr writer( new CsvWriter("csvbench",{"vsupply","isupply","power"},csvFilePath) );


    long int delta;
    double deltaMs;
    double deltaSecs;
    double rate;


    int NUM_BUCKETS = 100000;
    printf( "Generating %d buckets..." , NUM_BUCKETS );
    fflush(stdout);
    std::vector<DataBucketPtr> buckets;
    buckets.reserve(NUM_BUCKETS);
    Poco::Stopwatch stopwatch;
    stopwatch.start();
    for ( int cnt=0 ; cnt<NUM_BUCKETS ; cnt++ ) {
        DataBucket *p = new DataBucket("power");
        p->addDataPoint("vsupply",0.2);
        p->addDataPoint("isupply",0.081);
        p->addDataPoint("power",0.1235);
        buckets.push_back(DataBucketPtr(p));
    }
    delta       = stopwatch.elapsed();
    deltaMs     = delta*0.001;
    deltaSecs   = delta*0.000001;
    rate        = NUM_BUCKETS/1000.0/deltaSecs;
    printf( " %.1fms ( %.3fk Iter./sec\n",deltaMs,rate);


    int NUM_ITERATIONS = 100000;
    printf( "Writing %d buckets to CSV..." , NUM_ITERATIONS );
    fflush(stdout);
    stopwatch.restart();
    for ( int cnt=0 ; cnt<NUM_ITERATIONS ; cnt++ ) {
        DataBucketPtr p( buckets[cnt%NUM_BUCKETS] );
        writer->incomingBucket(p);
    }
    delta       = stopwatch.elapsed();
    deltaMs     = delta*0.001;
    deltaSecs   = delta*0.000001;
    rate        = NUM_ITERATIONS/1000.0/deltaSecs;
    printf( " %.1fms ( %.3fk Iter./sec\n",deltaMs,rate);



}
