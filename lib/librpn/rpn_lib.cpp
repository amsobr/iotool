//
// Created by to on 17/03/19.
//

#include <memory>
#include <fstream>
#include <mutex>
#include <fcntl.h>


#include <Poco/File.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Object.h>

#include <rpn/rpn_lib.hpp>
#include <rpn/Subroutine.hpp>
#include <rpn/exception.hpp>


#include "context_impl.hpp"
#include "builtin_instruction_handlers.hpp"

using namespace std;
using namespace Poco::JSON;
using Poco::Dynamic::Var;

namespace Rpn
{

RpnLib::RpnLib()
{
    scoped_lock lock{myMutex};

    p_registerAdapter( InstructionAdapter{ "clear"      , builtin_clear } );
    p_registerAdapter( InstructionAdapter{ "drop"       , builtin_drop } );
    p_registerAdapter( InstructionAdapter{ "drop.n"     , builtin_dropN } );
    p_registerAdapter( InstructionAdapter{ "var.set"    , builtin_assignVariable } );
    p_registerAdapter( InstructionAdapter{ "var.get"    , builtin_recallVariable } );
    p_registerAdapter( InstructionAdapter{ "const.set"  , builtin_assignConstant } );
    p_registerAdapter( InstructionAdapter{ "const.get"  , builtin_recallConstant } );
    p_registerAdapter( InstructionAdapter{ "add"        , builtin_add } );
    p_registerAdapter( InstructionAdapter{ "sub"        , builtin_sub } );
    p_registerAdapter( InstructionAdapter{ "mul"        , builtin_mul } );
    p_registerAdapter( InstructionAdapter{ "div"        , builtin_div } );
    p_registerAdapter( InstructionAdapter{ "neg"        , builtin_neg } );
    p_registerAdapter( InstructionAdapter{ "push"       , builtin_push } );
    p_registerAdapter( InstructionAdapter{ "dup.n"      , builtin_dupN } );
    p_registerAdapter( InstructionAdapter{ "dup"        , builtin_dup} );
}

//RpnLib::~RpnLib() = default;


bool RpnLib::loadAddons( std::string const &pathName )
{
    Poco::File dir(pathName);
    if ( !dir.exists() ) {
        printf("RpnLib: addon dir does not exist\n");
        return false;
    }
    if ( !dir.isDirectory() ) {
        printf("OOPS: '%s' doesn't name a directory...\n",pathName.c_str() );
        return false;
    }

    std::vector<Poco::File> allFiles;
    dir.list(allFiles);
    for ( auto const& file : allFiles ) {
        if ( !file.isFile() ) continue;
        loadAddon(file.path());
    }
    return true;
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
    createSubroutine(functionName,script);
}

RpnLib& RpnLib::getInstance()
{
//    static mutex theMutex;
    static RpnLib theInstance;
    return theInstance;
//    //static unique_ptr<RpnLibBuilder> theInstance;
//    theMutex.lock();
//    if ( theInstance==nullptr ) {
//        auto x = std::make_unique<RpnLibBuilder>();
//        theInstance.swap(x);
//    }
//    theMutex.lock();
//    return *theInstance;
}


AbstractInstruction::UniqPtr RpnLib::compile(string const& line)
{
    scoped_lock lock{myMutex};
    return p_compile(line);
}



bool RpnLib::p_lookupInstruction(string const& name, InstructionAdapter* adapter ) const
{

    for ( auto const& p : myInstructionAdapters ) {
        if ( p.getName()==name ) {
            if ( adapter!=nullptr ) {
                *adapter    = p;
            }
            return true;
        }
    }
    return false;
}

bool RpnLib::p_lookupSubroutine(string const& name , SubroutineAdapter* adapter) const
{
    static SubroutineAdapter::UniqPtr nullAdapter{ nullptr };
    for ( auto const& p : myScriptAdapters ) {
        if ( p.getName()==name ) {
            if ( adapter!=nullptr ) {
                *adapter    = p;
            }
            return true;
        }
    }
    return false;
}

void RpnLib::p_registerAdapter(InstructionAdapter adapter)
{
    if ( p_exists(adapter.getName()) ) {
        throw ErrAlreadyExists{ "instruction already registered: " + adapter.getName() };
    }
    myInstructionAdapters.push_back(move(adapter));
}

void RpnLib::p_registerAdapter(SubroutineAdapter adapter)
{
    if ( p_exists(adapter.getName()) ) {
        throw ErrAlreadyExists{ "subroutine already registered: " + adapter.getName() };
    }
    myScriptAdapters.push_back(move(adapter));
}

AbstractInstruction::UniqPtr RpnLib::createSubroutine(string const& name, vector<std::string> const& script)
{
    vector<AbstractInstruction::Ptr> instructions;
    for ( auto const& s : script ) {
        auto instr{ p_compile(s) };
        if ( instr==nullptr ) {
            throw ErrNotFound{"unknown instruction: " + s};
        }
        instructions.push_back(move(instr));
    }

    SubroutineAdapter adapter{name,instructions};
    scoped_lock lock{ myMutex };
    if ( p_exists(name) ) {
        throw ErrAlreadyExists{ "instruction/subroutine already exists: " + name };
    }
    p_registerAdapter(adapter);
    return make_unique<Subroutine>(adapter);
}

Instruction::UniqPtr RpnLib::p_compile(string const& s)
{
    size_t strLen   = s.size();
    size_t curPos   = 0;
    size_t curTokPos= 0;
    vector<string> tokens;

    while ( curPos<strLen ) {
        if ( s[curPos]==' ' ) {
            if ( curPos>curTokPos ) {
                /* save token and advance to next one.
                 * consecutive spaces are skipped.
                 */
                tokens.emplace_back(s.substr(curTokPos,curPos-curTokPos));
            }
            while ( s[curPos]==' ' && curPos<strLen ) {
                curPos++;
            }
            curTokPos   = curPos;
        }
        else {
            curPos++;
        }
    }
    if ( curPos>curTokPos ) {
        tokens.emplace_back(s.substr(curTokPos,curPos-curTokPos));
    }

    if ( tokens.empty() ) {
        throw ErrInvalidArguments{"cannot compile empty instruction"};
    }

    string const& name  = tokens[0];

    if ( tokens.size()>1 ) {
        /* most likely an instruction... */
        InstructionAdapter instrAdapter;
        if ( !p_lookupInstruction(name,&instrAdapter) ) {
            if ( p_lookupSubroutine(name) ) {
                throw ErrInvalidArguments{"subroutines can't take arguments"};
            }
            else {
                throw ErrNotFound{ "cannot find: " + name };
            }
        }
        ArgumentList args{ vector<string>{tokens.begin()+1,tokens.end()} };
        return make_unique<Instruction>(args,instrAdapter);
    }
    else {
        /* can be either... */
        /* ...BUT most likely an instruction... */
        InstructionAdapter instrAdapter;
        if ( p_lookupInstruction(name,&instrAdapter) ) {
            return make_unique<Instruction>(ArgumentList{},instrAdapter);
        }

        SubroutineAdapter subAdapter;
        if ( p_lookupSubroutine(name,&subAdapter) ) {
            return make_unique<Subroutine>(subAdapter);
        }

        throw ErrNotFound{ "cannot find: " + name };
    }
}


Context::UniqPtr RpnLib::createContext()
{
    return make_unique<ContextImpl>();
}


} /* namespace Rpn */


