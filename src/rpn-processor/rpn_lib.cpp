//
// Created by to on 17/03/19.
//

#include <fstream>
#include <fcntl.h>

#include <Poco/File.h>

#include <rpn-processor/rpn_lib.hpp>
#include <rpn-processor/script.hpp>
#include <rpn-processor/rpn_lib.hpp>
#include <rpn-processor/exception.hpp>


#include "context_impl.hpp"
#include "builtin_instruction_handlers.hpp"
#include "basic_instruction.hpp"
#include "instruction_catalog.hpp"
#include "subroutine.hpp"

namespace Rpn
{


RpnLib::RpnLib()
{
    myCatalog   = new InstructionCatalog();

}

RpnLib::~RpnLib()
{
    delete myCatalog;
}


void RpnLib::init()
{
    myCatalog->addResolver( new BasicInstruction("add",builtin_add) );
    myCatalog->addResolver( new BasicInstruction("sub",builtin_add) );
    myCatalog->addResolver( new BasicInstruction("mul",builtin_add) );
    myCatalog->addResolver( new BasicInstruction("div",builtin_add) );
    myCatalog->addResolver( new BasicInstruction("neg",builtin_add) );

}


bool RpnLib::loadAddons( std::string const &pathName )
{
    Poco::File dir(pathName);
    if ( !dir.isDirectory() ) {
        return false;
    }

    std::vector<Poco::File> allFiles;
    dir.list(allFiles);
    for ( auto file : allFiles ) {
        if ( !file.isFile() ) continue;
        loadAddon(file.path());
    }

    return true;

}

bool RpnLib::resolveDependencies()
{


}

Instruction *RpnLib::lookup( std::string name )
{
    return myCatalog->lookup(name);
}


ScriptPtr RpnLib::compile( std::vector<std::string> const &script )
{

}

void RpnLib::loadAddon(std::string const &path)
{
    std::ifstream fileStream;

    fileStream.open( path.c_str() );
    enum {
        INIT ,
        READ_NAME ,
        EXPECT_SCRIPT ,
        READ_SCRIPT ,
    } state = INIT;

    std::vector<std::string> script;
    std::string functionName;

    std::string line;
    while ( std::getline(fileStream,line,'\n') ) {
        switch( state ) {
            case INIT:
                if ( line=="[FUNCTION]" ) {
                    state   = READ_NAME;
                }
                else {
                    throw MalformedFileException( "File '" + path + "' is malformed" );
                }
                break;

            case READ_NAME:
                functionName    = line;
                state           = EXPECT_SCRIPT;
                break;

            case EXPECT_SCRIPT:
                if ( line=="[SCRIPT]" ) {
                    state       = READ_SCRIPT;
                }
                else {
                    throw MalformedFileException( "File '" + path + "' is malformed" );
                }
                break;

            case READ_SCRIPT:
                script.push_back(line);
                break;
        }
        Subroutine *subroutine  = new Subroutine(functionName,script);
        myCatalog->addResolver(subroutine);
    }
}


}; /* namespace Rpn */


