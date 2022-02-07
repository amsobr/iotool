#ifndef RPN_LIB_HPP
#define RPN_LIB_HPP

#include <vector>
#include <string>
#include <mutex>

#include "Subroutine.hpp"
#include "context.hpp"
#include "Instruction.hpp"
#include "SubroutineAdapter.h"


/**
 * @startuml
 *
 *
 * abstract class AbstractInstruction {
 * + AbstractInstruction( name )
 * + {abstract} getName(): string
 * + {abstract} execute( context )
 * }
 *
 * class Instruction {
 *  - myHandler : InstructionAdapter&
 *  - myArgs : vector<Args>
 *
 *  + Instruction( args , adapter )
 *  + execute( RpnContextPtr& )
 *  + getName()
 * }
 * note top of Instruction
 * One instance per compiled instruction.
 * Each instance pre-loaded with the arguments
 * parsed on compilation
 * end note
 *
 * class InstructionAdapter {
 *  + execute( context, args )
 *  + getName()
 *  - funcPtr
 *  - name
 * }
 *
 * note bottom of InstructionAdapter
 * * One adapter per registered instruction
 * * Shared among all instances using unique_ptr&
 * * contains pointer to handler function
 * end note
 *
 * note top of Subroutine
 * lines are compiled into subroutines when
 * they they match one of their names
 * end note
 * class Subroutine {
 *  + Subroutine( adapter )
 *  - adapter : SubroutineAdapter
 *  + execute( context )
 * }
 *
 * class SubroutineAdapter {
 * + execute( context )
 * + getName()
 * - name
 * - script: vector<AbstractInstruction>
 * }
 * note bottom of SubroutineAdapter
 * * subroutines execute by sequentially
 * running the instructions they are made of.
 * * Some of the instructions may correspond to
 * subroutines, too...
 * * each subroutine has an adapter, which contains
 * the script
 * * the adapter is shared among all instances
 * end note

 *
 *
 * Instruction --|> AbstractInstruction
 * Instruction " N " o-- " 1 " InstructionAdapter
 *
 * Subroutine --|> AbstractInstruction
 * Subroutine " N " o-- " 1 " SubroutineAdapter
 *
 *
 * @enduml
 */

namespace Rpn
{


class RpnLib
{
public:
    RpnLib& operator=(RpnLib const& o) = delete;
    RpnLib( RpnLib const& o ) = delete;

    virtual ~RpnLib() = default;


    static RpnLib& getInstance();


    /** @brief Compile line into an executable instruction.
     *
     * @param line the command, including arguments
     * @return Pointer to the instruction. or nullptr if the instruction is
     * not known.
     */
    AbstractInstruction::UniqPtr compile(std::string const& line);


    /**
     * Create a script with given name and list of instructions.
     * The script will be compiled and, in case of being well-formed, will be
     * added to the library.
     *
     * Dependencies are automatically resolved. However it may happen that there
     * are missing instructions to resolve.
     * @note
     * When a script is compiled, it is immediately added to the library and is
     * also returned, in the form of an instruction, to the caller. The returned
     * instruction may, or may not be fully resolved by the time it is returned.
     * If there are missing scripts to fully resolve the compiled script, as
     * they are added to the library, the returned script automatically resolves
     * and becomes able to execute.
     * @param name The name of the script
     * @param script The sequence of instructions of the script.
     * @return A compiled instruction mathing the newly created script.
     */
    AbstractInstruction::UniqPtr createSubroutine(std::string const& name, std::vector<std::string> const &script );

    bool loadAddons( std::string const& path );

    static Context::UniqPtr createContext();

protected:
    RpnLib();


private:
    void loadAddon(std::string const &path);

    void p_registerAdapter(InstructionAdapter adapter );
    void p_registerAdapter(SubroutineAdapter adapter );

    Instruction::UniqPtr p_compile( std::string const& s);

    [[nodiscard]] bool p_lookupInstruction( std::string const& name , InstructionAdapter* adapter=nullptr) const;
    [[nodiscard]] bool p_lookupSubroutine( std::string const& name , SubroutineAdapter* adapter=nullptr ) const;

    [[nodiscard]] bool p_exists( std::string const& name ) const
    {
        return p_lookupInstruction(name) || p_lookupSubroutine(name);
    }

    std::mutex myMutex;
    std::vector<InstructionAdapter>    myInstructionAdapters;
    std::vector<SubroutineAdapter>     myScriptAdapters;
};

/* class RpnLib */


} /* namespace Rpn */



#endif /* !defined RPN_LIB_HPP */
