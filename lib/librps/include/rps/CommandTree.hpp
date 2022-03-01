//
// Created by to on 17/02/22.
//


#pragma once

#include <map>
#include <string>
#include <cctype>

#include "RpsFwd.hpp"
#include "CliPath.hpp"


namespace rps
{



class CommandTree
{
private:
    struct Node {
        Node const*     parent{ nullptr }; // nullptr for root node
        DecoderConstPtr decoder{ nullptr };
        std::map<std::string,Node> children{ };

        Node( Node const& p )   = default;
        Node( Node&& p )        = default;
        Node& operator=( Node const& o) = default;

        explicit Node( Node const* p=nullptr ) :
        parent{ p }
        {}

    };

    
    
public:
    /** @brief create a blank CliTree
     *
     * Creates an empty CliTree. The root node is automatically populated-
     */
    CommandTree();



    /** destroy a CliTree */
    ~CommandTree();


    /** @brief add a path to the CLI.
     *
     * All paths to add must be absolute. The CliTree automatically handles the
     * integration of the path to add with any pre-existing tree nodes that
     * may overlap with the given path.
     *
     * @param path The path to be added. Always counts from the root node
     */
    void addPath( CliPath const& path );


    /** @brief check if a path exists in the tree
     *
     * @param path The path to check. Is always treated as absolute
     * @return true if the path exists, otherwise returns false.
     */
    [[nodiscard]] bool pathExists( CliPath const& path ) const;


    /** @brief add a decoder on provide path
     *
     * Adds one command decoder at the specified path. The path is automatically
     * created if needed. Ownership is shared between the CliTree and the caller,
     * so that the caller can populate the decoded if needed after this call.
     * @param path The path where the decoder will be placed.
     * @param decoder The decoder to place at given path. This decoded will be
     *              used for this path and as a fallback lookup for any child
     *              node.
     */
    void addDecoder( CliPath const& path , DecoderConstPtr decoder );
    
    
    /**
     * @brief Add a command to the structure
     *
     * Adds the provided command on the location indicated by path. The path
     * and a matching decoder will be automatically created, as needed.
     * @param path The path where the command will be located
     * @param cmd The command to add
     */
    void addCommand( CliPath const& path , AbstractCommandConstPtr cmd );
    void addCommand( std::string const& path , AbstractCommandConstPtr cmd )
    {
        addCommand( CliPath{path} , std::move(cmd) );
    }
    
    /**
     * @brief Add a script to the structure
     *
     * Adds the script to the structure at the location specified by path.
     * The path and matching decoder will be automatically created, as needed.
     * @param path The location at which the script will be placed
     * @param script The script to be added
     */
    void addScript( CliPath const& path , ScriptConstPtr script );


    /** @brief lookup a command on a certain location.
     *
     * Looks up cmdName at the specified path.
     * If the command is not found at the provided path, it's parent is searched
     * instead, until the command if found, or the CliTree root is reached.
     *
     * @param path Initial path to look for. Will be searched first, and then
     *              its parent, until the command if found.
     * @param cmdName The name of the command to look for.
     * @return Returns a shared pointer to the command if found. If the command
     *          if not found on path nor any of its parents, returns nullptr
     * @throws NotFoundException - the provided path is invalid
     * @sa lookupScript
     */
    [[nodiscard]] AbstractCommandConstPtr lookupCmd(
                        CliPath const& path ,
                        std::string const& cmdName ) const;


    /** @brief lookup a script on a certain location.
     *
     * Looks up scriptName at the specified path. If the command is not found
     * at the provided path, it's parent is searched instead, until the script
     * is found, or the CliTree root is reached.
     *
     * @param path Initial path to look for. Will be searched first, and then
     *              its parent, until the command if found.
     * @param scriptName The name of the script to look for.
     * @return Returns a shared pointer to the command if found. If the command
     *          if not found on path nor any of its parents, returns nullptr
     * @throws NotFoundException - the provided path is invalid
     * @sa lookupCmd
     */
     [[nodiscard]] ScriptConstPtr lookupScript(
                        CliPath const& path ,
                        std::string const& scriptName ) const;
     
     /**
      * @brief Show contents of path
      *
      * Lists available commands and paths under a given path.
      * @param path The path for which to show available commands and paths
      * @return A tuple of three vectors: <br>
      *         The first vector contains the available commands <br>
      *         The second vector lists the available scripts<br>
      *         The third element lists the child paths
      */
     std::tuple< std::vector<AbstractCommandConstPtr>, std::vector<ScriptConstPtr>, std::vector<std::string> >
        showPath( CliPath const& path );


private:
    /**
     * @brief Go to node at specieid path location
     * @param path The path for which to find the corresponding node
     * @return The node mathinc path of nullptr if path does not exist
     */
    Node const* gotoPath( CliPath const& path );
    
    
    Node    myRootNode;
};


} /* namespace rps */
