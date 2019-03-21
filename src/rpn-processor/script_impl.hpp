//
// Created by to on 18/03/19.
//

#ifndef IOTOOL_SCRIPT_IMPL_HPP
#define IOTOOL_SCRIPT_IMPL_HPP

#include <rpn-processor/script.hpp>
#include <rpn-processor/exception.hpp>
#include <rpn-processor/rpn_lib.hpp>

#include <sstream>

namespace Rpn
{

struct Step
{
    std::string instrName;
    std::vector<std::string> args;
    Instruction *handler;

    Step(std::string const &line)
    {
        std::istringstream is(line);
        std::getline(is, instrName , ' ');
        if (instrName.empty() ) {
            throw ErrInvalidSyntax();
        }

        std::string t;
        while ( std::getline(is, t,' ') ) {
            args.push_back(t);
        }

        handler = nullptr;
    }

    Step( Step const &o ) :
            instrName(o.instrName) ,
            handler(o.handler)
    {}

    virtual ~Step() = default;

    Step &operator=( Step const &other )
    {
        instrName = other.instrName;
        handler   = other.handler;
        return *this;
    }
};

class ScriptImpl : public Script
{
private:
    std::vector<Step> mySteps;

public:
    ScriptImpl( std::vector<std::string> const &script )
    {
        for ( std::string line : script ) {
            mySteps.push_back( Step(line) );
        }
    }

    virtual ~ScriptImpl() = default;

    /* arguments are ignored in a script, as these were already passed through and
     * compiled on construction of the script itself.
     */
    void execute(ContextPtr ctx ) override
    {
        for ( Step &step : mySteps ) {
            step.handler->execute(ctx,step.args);
        }
    }

    bool resolveDependencies(RpnLib &lib)
    {
        for ( Step &step : mySteps ) {
            Instruction *handler = lib.lookup(step.instrName);
                /* no resolver exists... easy to figure out... */
            if ( handler==nullptr ) return false;

            if ( handler->resolveDependencies(lib) ) {
                step.handler = handler;
            }
            else {
                /* we DID have a handler but one of its dependencies is likely missing... */
                return false;
            }
        }
        return true;
    };
};

}; /* namespace Rpn */


#endif //IOTOOL_SCRIPT_IMPL_HPP
