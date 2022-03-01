
#pragma once

#include <vector>
#include <string>
#include <mutex>


#include "RpsFwd.hpp"

namespace rps
{

class Decoder;
typedef std::shared_ptr<Decoder> DecoderPtr;

class Decoder
{
public:
    /** Create a blank decoder
     */
    Decoder()   = default;


    /** @brief destroy a decoder
     */
    ~Decoder() = default;


    /** @brief Register a command
     *
     * @note While it is possible to have one command and one script with the
     *      same, such practice is <em>highly discouraged</em>. Future versions
     *      may add restrictions that will break applications which abuse this
     *      limitation.
     *
     * @param command The command to register.
     * @throw AlreadyExistsException - an instruction already exists with the same name
     */
    void addCommand(AbstractCommandConstPtr command );


    /* Register a script
     *
     * @note While it is possible to have one command and one script with the
     *      same, such practice is <em>highly discouraged</em>. Future versions
     *      may add restrictions that will break applications which abuse this
     *      limitation.
     *
     * @param script The script to be registered. It is @em strongly discouraged
     *              to register scripts with the same name as existing
     *              instructions.
     */
    void addScript(ScriptConstPtr script );


    /** @brief look up a command
     *
     * @param name The name of the command to lookup
     * @return pointer to the command, if found. Otherwise returns nullptr
     */
    [[nodiscard]] AbstractCommandConstPtr findCommand(std::string const& name) const;


    /** @brief look up a script
     *
     * @param name The name of the script to lookup
     * @return pointer to the command, if found. Otherwise returns nullptr
     */
    [[nodiscard]] ScriptConstPtr findScript(std::string const& name) const;
    
    
    /**
     * @brief list all commands provided by the decoder
     * @return An array containing all commands supported by the decoder
     */
    [[nodiscard]] std::vector<AbstractCommandConstPtr> listCmds() const
    {
        return myCommands;
    }
    
    
    /**
     * @brief list all scripts provided by the decoded
     * @return An array containing all commands supported by the decoder;
     */
    [[nodiscard]] std::vector<ScriptConstPtr> listScriptS() const
    {
        return myScripts;
    }


private:
    std::vector<AbstractCommandConstPtr>    myCommands;
    std::vector<ScriptConstPtr>             myScripts;
};

typedef std::shared_ptr<Decoder> ResolverPtr;

/* class RpnLib */


} /* namespace rps */

