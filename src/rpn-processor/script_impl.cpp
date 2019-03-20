//
// Created by to on 18/03/19.
//

#include "script_impl.hpp"



//
// Created by to on 18/03/19.
//

#ifndef IOTOOL_SCRIPT_IMPL_HPP
#define IOTOOL_SCRIPT_IMPL_HPP

#include <rpn-processor/script.hpp>

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
            throw InvalidSyntaxException();
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
        Impl( std::vector<std::string> &script )
        {
            for ( std::string line : script ) {
                mySteps.push_back( Step(line) );
            }
        }

        virtual ~Impl() = default;

        /* arguments are ignored in a script, as these were already passed through and
         * compiled on construction of the script itself.
         */
        void execute(ContextPtr ctx , std::vector<std::string> const &) override
        {
            for ( Step &step : mySteps ) {
                step.handler->execute(ctx,step.args);
            }
        }

        Instruction *resolve( InstructionCatalog &catalog )
        {
            for ( Step &step : mySteps ) {
                InstructionResolver *resolver = catalog.lookup(step.instrName);
                /* no resolver exists... easy to figure out... */
                if ( resolver==nullptr ) return nullptr;

                step.handler    = resolver->resolve(catalog);
                /* we DID have a resolver but one of its dependencies is likely missing... */
                if ( step.handler==nullptr ) return nullptr;
            }
            return this;
        }



    };

    Subroutine::Subroutine( std::string const &name , std::vector<std::string> &script ) :
            InstructionResolver(name) ,
            impl( new Impl(script) )
    {
    }

    Subroutine::~Subroutine()
    {
        delete impl;
    }


    Instruction *Subroutine::resolve(InstructionCatalog &catalog)
    {
        return impl->resolve(catalog);
    }



};

}; /* namespace Rpn */


#endif //IOTOOL_SCRIPT_IMPL_HPP

