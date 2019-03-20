#ifndef INSTRUCTION_CATALOG_HPP
#define INSTRUCTION_CATALOG_HPP

#include <vector>
#include <string>
#include <mutex>

#include <rpn-processor/instruction.hpp>

namespace Rpn
{


class InstructionCatalog
{
private:
    std::mutex myMutex;
    std::vector<Instruction*> myCatalog;

public:

    InstructionCatalog() : 
    myMutex() ,
    myCatalog()
    {

    }

    /* NOTE: we should be delete'ing the resolvers here
     * but this catalog is made to be present for the whole runtime, so this should not be
     * a concern...
     */
    virtual ~InstructionCatalog() = default;

    InstructionCatalog( InstructionCatalog const & ) = delete;
    InstructionCatalog operator=( InstructionCatalog const &) = delete;

    Instruction *lookup( std::string const &name )
    {
        std::lock_guard<std::mutex> lockGuard(myMutex);
        for ( auto *resolver : myCatalog ) {
            if ( resolver->isCalled(name) ) {
                return resolver;
            }
        }
        return nullptr;
    }

    bool add( Instruction *instruction )
    {
        std::lock_guard<std::mutex> lockGuard(myMutex);
        for ( auto existing : myCatalog ) {
            if ( existing->isCalled(instruction->getName()) ) {
                return false;
            }
        }
        myCatalog.push_back(instruction);
        return true;
    }

};

}; /* namespace Rpn */


#endif /* !defined INSTRUCTION_CATALOG_HPP */
