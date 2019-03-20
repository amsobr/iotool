#ifndef RPN_LIB_HPP
#define RPN_LIB_HPP

#include <vector>
#include <string>

#include "script.hpp"
#include "context.hpp"


namespace Rpn
{

class InstructionCatalog;

class RpnLib
{
private:
    InstructionCatalog *myCatalog;

    void loadAddon(std::string const &path);

public:

    RpnLib();

    virtual ~RpnLib();

    void init();

    /**
     * @brief Load addons from a path
     * @param pathName
     * @return bool on success. In case of an error, throws Rpn::Exception
     */
    bool loadAddons( std::string const &pathName );

    /**
     * Call after init() and all calls to loadAddons(), in order
     * to resolve dependencies
     * @return true in case all dependencies are met, ie, all symbols resolve to an Instruction.
     * Otherwise returns false.
     */
    bool resolveDependencies();

    /** @brief Find an instruction by name
     *
     * @param name Name of the instruction to lookup
     * @return Pointer to the instruction with given name. nullptr if none is found.
     */
    Instruction *lookup( std::string name );


    /** @brief compile an RPN script into executable form
     *
     * @param script Array with the instructions.
     * @return A runnable script, or nullptr if compilation fails
     */

    ScriptPtr compile( std::vector<std::string> const &script );


};

/* class RpnLib */


}; /* namespace Rpn */



#endif /* !defined RPN_LIB_HPP */
