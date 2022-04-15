//
// Created by to on 17/02/22.
//

#include <cassert>

#include <rps/Exception.hpp>
#include <rps/Decoder.hpp>
#include <rps/CommandTree.hpp>

#include <rps/BuiltinCommands.hpp>

namespace rps
{


CommandTree::CommandTree() :
    myRootNode{ }
{
    /* root node is initialized */
}

CommandTree::~CommandTree() = default;



/** @brief ensure the path is absolute
 *
 * Ensures the path is absolute. throws an exeption in case it isn't.
 * @param p the path to check
 */
static inline void ckAbsPath( CliPath const& p )
{
    if ( !p.isAbsolute() ) {
        throw InvalidArgumentsException{"path must be absolute"};
    }
}


void CommandTree::addPath(CliPath const& path)
{
    ckAbsPath(path);

    Node* curNode   = &myRootNode;
    for ( auto const& tok : path ) {
        if ( !curNode->children.contains(tok) ) {
            curNode->children[tok]  = Node{curNode};
        }
        curNode = &curNode->children[tok];
    }
}

bool CommandTree::pathExists(CliPath const& path) const
{
    ckAbsPath(path);

    Node const* curNode   = &myRootNode;
    for ( auto const& tok : path ) {
        if ( !curNode->children.contains(tok) ) {
            return false;
        }
        curNode = &curNode->children.at(tok);
    }
    return true;
}

void CommandTree::addDecoder(CliPath const& path, DecoderConstPtr decoder)
{
    ckAbsPath(path);
    
    
    Node* curNode   = &myRootNode;
    
    for ( auto const& tok : path ) {
        if ( !curNode->children.contains(tok) ) {
            curNode->children[tok]  = Node{curNode};
        }
        curNode = &curNode->children[tok];
    }
    assert(curNode!=nullptr);
    curNode->decoder    = std::move(decoder);
}

AbstractCommandConstPtr CommandTree::lookupCmd(CliPath const& path, std::string const& cmdName) const
{
    ckAbsPath(path);

    Node const* curNode   = &myRootNode;
    for ( auto const& tok : path ) {
        if ( !curNode->children.contains(tok) ) {
            throw NotFoundException{"path was not found"};
        }
        curNode = &curNode->children.at(tok);
    }

    /* lookup command in current node
     * descend into parent if:
     *  - no decoder
     *  - OR command not found
     *
     *  STOP when ROOT was searched without a match
     */
    while ( curNode!=nullptr ) {
        if ( curNode->decoder!=nullptr ) {
            auto cmd    = curNode->decoder->findCommand(cmdName);
            if ( cmd!=nullptr ) {
                return cmd;
            }
        }
        curNode = curNode->parent;
    }
    
    return nullptr;
}

ScriptConstPtr CommandTree::lookupScript(CliPath const& path, std::string const& scriptName) const
{
    ckAbsPath(path);

    Node const* curNode   = &myRootNode;
    for ( auto const& tok : path ) {
        if ( !curNode->children.contains(tok) ) {
            throw NotFoundException{"path was not found"};
        }
        curNode = &curNode->children.at(tok);
    }

    /* lookup command in current node
     * descend into parent if:
     *  - no decoder
     *  - OR command not found
     *
     *  STOP when ROOT was searched without a match
     */
    while ( curNode!=nullptr ) {
        if ( curNode->decoder!=nullptr ) {
            ScriptConstPtr cmd  = curNode->decoder->findScript(scriptName);
            if ( cmd!=nullptr ) {
                return cmd;
            }
        }
        curNode = curNode->parent;
    }
    return nullptr;
}

void CommandTree::addCommand(CliPath const& path, AbstractCommandConstPtr cmd)
{
    ckAbsPath(path);
    
    Node* curNode   = &myRootNode;
    for ( auto const& n: path ) {
        if ( !curNode->children.contains(n) ) {
            curNode->children[n]    = Node{curNode};
        }
        curNode = &curNode->children[n];
    }
    if ( curNode->decoder==nullptr ) {
        curNode->decoder    = std::make_shared<Decoder>();
    }
    curNode->decoder->addCommand(std::move(cmd));
}

void CommandTree::addScript(CliPath const& path, ScriptConstPtr script)
{
    ckAbsPath(path);
    
    Node* curNode   = &myRootNode;
    for ( auto const& n: path ) {
        if ( !curNode->children.contains(n) ) {
            curNode->children[n]    = Node{curNode};
        }
        curNode = &curNode->children[n];
    }
    if ( curNode->decoder==nullptr ) {
        curNode->decoder    = std::make_shared<Decoder>();
    }
    curNode->decoder->addScript(std::move(script));
}

std::tuple<
    std::vector<AbstractCommandConstPtr>,
    std::vector<ScriptConstPtr>,
    std::vector<std::string> > CommandTree::showPath(CliPath const& path)
{
    auto* node  = gotoPath(path);
    if ( node==nullptr ) {
        return {};
    }
    
    std::vector<std::string> children;
    for ( auto const& c: node->children ) {
        children.push_back(c.first);
    }
    
    std::vector<AbstractCommandConstPtr> cmds;
    std::vector<ScriptConstPtr> scripts;
    
    while ( node!=nullptr ) {
        if ( node->decoder!=nullptr ) {
            auto c  = node->decoder->listCmds();
            auto s  = node->decoder->listScriptS();
            cmds.insert(end(cmds),begin(c),end(c));
            scripts.insert(end(scripts),begin(s),end(s));
        }
        node    = node->parent;
    }
    
    return { cmds,scripts ,children };
}

CommandTree::Node const* CommandTree::gotoPath(CliPath const& path)
{
    Node* curNode   = &myRootNode;
    for ( auto const& p : path ) {
        if ( curNode->children.contains(p) ) {
            curNode = &curNode->children.at(p);
        }
        else {
            return nullptr;
        }
    }
    return curNode;
}

} /* namespace rps */
