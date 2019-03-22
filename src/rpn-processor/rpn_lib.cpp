//
// Created by to on 17/03/19.
//

#include <fstream>
#include <fcntl.h>

#include <Poco/File.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Object.h>

#include <rpn-processor/rpn_lib.hpp>
#include <rpn-processor/script.hpp>
#include <rpn-processor/rpn_lib.hpp>
#include <rpn-processor/exception.hpp>


#include "context_impl.hpp"
#include "builtin_instruction_handlers.hpp"
#include "basic_instruction.hpp"
#include "instruction_catalog.hpp"
#include "subroutine.hpp"

using std::vector;
using std::string;
using namespace Poco::JSON;
using Poco::Dynamic::Var;

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
    myCatalog->add( new BasicInstruction("clear",builtin_clear) );
    myCatalog->add( new BasicInstruction("drop",builtin_drop) );
    myCatalog->add( new BasicInstruction("drop.n",builtin_dropN) );
    myCatalog->add( new BasicInstruction("var.set",builtin_assignVariable) );
    myCatalog->add( new BasicInstruction("var.get",builtin_recallVariable) );
    myCatalog->add( new BasicInstruction("const.set",builtin_assignConstant) );
    myCatalog->add( new BasicInstruction("const.get",builtin_recallConstant) );
    myCatalog->add( new BasicInstruction("add",builtin_add) );
    myCatalog->add( new BasicInstruction("sub",builtin_add) );
    myCatalog->add( new BasicInstruction("mul",builtin_add) );
    myCatalog->add( new BasicInstruction("div",builtin_add) );
    myCatalog->add( new BasicInstruction("neg",builtin_add) );
}


bool RpnLib::loadAddons( std::string const &pathName )
{
    Poco::File dir(pathName);
    if ( !dir.isDirectory() ) {
        printf("OOPS: '%s' doesn't name a directory...\n",pathName.c_str() );
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
    printf("OOPS: THIS WILL NOT WORK...\n");

    throw std::runtime_error("Not Implemented - RpnLin::resolveDependencies");
}

Instruction *RpnLib::lookup(const string &name)
{
    return myCatalog->lookup(name);
}


ScriptPtr RpnLib::compile( std::vector<std::string> const &script )
{
    ScriptImpl *s   = new ScriptImpl(script);
    if ( s->resolveDependencies(*this)) {
        return ScriptPtr(s);
    }
    else {
        printf( "Failed resolving dependencies of script");
        abort();
    }
    delete s;
    return nullptr;
}

void RpnLib::loadAddon(std::string const &path)
{
    std::ifstream fileStream(path);

    Parser parser;
    Var parsed = parser.parse(fileStream);


    Object::Ptr root    = parsed.extract<Object::Ptr>();
    string functionName = root->getValue<std::string>("functionName");
    Array::Ptr jScript  = root->getArray("script");

    vector<string> script;
    for ( auto it=jScript->begin() ; it!=jScript->end() ; it++ ) {
        script.push_back((*it).convert<string>());
    }

    printf("About to create subroutine '%s' with script...",functionName.c_str() );
    Subroutine *subroutine  = new Subroutine(functionName,script);
    subroutine->resolveDependencies(*this);

    myCatalog->add(subroutine);
}

ContextPtr RpnLib::createContext()
{
    return ContextPtr( new ContextImpl() );
}


}; /* namespace Rpn */


