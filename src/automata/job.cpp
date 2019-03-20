//
// Created by to on 20/03/19.
//

#include <fstream>

#include <Poco/NumberParser.h>

#include "job.hpp"

Job::Job(std::string const &path)
{
    std::ifstream fileStream(path);

    enum {
        INIT ,
        READ_NAME ,
        EXPECT_SCRIPT ,
        READ_SCRIPT ,
        READ_OUTPUTS ,
    } state = INIT;

    std::vector<std::string> script;

    std::string line;
    while ( std::getline(fileStream,line,'\n') ) {
    switch( state ) {
        case INIT:
            if ( line=="[JOB]" ) {
                state   = READ_NAME;
            }
            else {
                throw std::runtime_error( "File '" + path + "' is malformed. [JOB] is missing" );
            }
            break;

        case READ_NAME:
            myName      = line;
            state       = EXPECT_SCRIPT;
            break;

        case EXPECT_SCRIPT:
            if ( line=="[SCRIPT]" ) {
                state       = READ_SCRIPT;
            }
            else {
                throw std::runtime_error( "File '" + path + "' is malformed. [SCRIPT] is missing" );
            }
            break;

        case READ_SCRIPT:
            if ( line=="[OUTPUTS]" ) {
                state = READ_OUTPUTS;
            }
            else {
                script.push_back(line);
            }
            break;

        case READ_OUTPUTS: {
            size_t separatorPos = line.find('@');
            if ( separatorPos==0 || separatorPos==(line.size()-1) ) {
                throw std::runtime_error(path + ": malformed output mapping: '" + line + "'");
            }
            std::string destVar = line.substr(0,separatorPos);
            int varPos          = Poco::NumberParser::parse(line.substr(separatorPos+1));
            myOutputMappings.push_back(ValueMapping(destVar,varPos));

        }
    }
    Script *s =
}



    }


}
