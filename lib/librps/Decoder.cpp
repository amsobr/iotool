//
// Created by to on 17/03/19.
//

#include <algorithm>

#include <rps/Command.hpp>
#include <rps/Script.hpp>
#include <rps/Decoder.hpp>

using namespace std;

namespace rps
{

void Decoder::addCommand(AbstractCommandConstPtr command)
{
    for_each( myCommands.cbegin(), myCommands.cend() ,
            [&command] (auto const& c) {
                if ( command->getName()==c->getName() ) {
                    throw AlreadyExistsException{"command " + command->getName() + " already exists"};
                }
            } );
    myCommands.emplace_back(move(command));
}

void Decoder::addScript(ScriptConstPtr script)
{
    for_each( myScripts.cbegin(), myScripts.cend() ,
              [&script] (auto const& c) {
                  if ( script->getName()==c->getName() ) {
                      throw AlreadyExistsException{"script " + script->getName() + " already exists"};
                  }
              } );
    myScripts.emplace_back(move(script));

}

AbstractCommandConstPtr Decoder::findCommand(string const& name) const
{
    auto t  = find_if( myCommands.cbegin() , myCommands.cend() ,
        [&name](auto const& c) { return c->getName()==name; } );
    if ( t!=myCommands.end() ) {
        return *t;
    }
    return nullptr;
}

ScriptConstPtr Decoder::findScript(string const& name) const
{
    auto t  = find_if( myScripts.cbegin() , myScripts.cend() ,
                       [&name](auto const& c) { return c->getName()==name; } );
    if ( t!=myScripts.end() ) {
        return *t;
    }
    return nullptr;
}

#if 0

bool Resolver::loadAddons(std::string const &pathName )
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


void Resolver::loadAddon(std::string const &path)
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


Instruction::UniqPtr Resolver::p_compile(string const& s)
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

#endif

} /* namespace rps */


