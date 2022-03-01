//
// Created by to on 11/02/22.
//


#pragma once

#include <cassert>
#include <memory>
#include <any>
#include <string>
#include <iostream>
#include <sstream>
#include <functional>

#include <Poco/Path.h>

#include "Stack.hpp"
#include "Decoder.hpp"
#include "CliPath.hpp"
#include "CommandTree.hpp"
#include "StreamAdapter.hpp"
#include "AbstractCommand.hpp"

namespace rps
{

class Context
{
public:
    /**
     * @brief Create a new Context
     * @param stream IO streams adapter. The Context takes ownership of the adapter
     *          and no further usage of ios is allowed after the constructor
     *          returns.
     * @param cliTree The command tree of the CLI
     * @param priv Private data, if any
     */
    Context(std::shared_ptr<StreamAdapter> stream ,
            CommandTreeConstPtr cliTree ,
            std::any priv=std::any{} ) :
        stream{std::move(stream) } ,
        myCliTree{ std::move(cliTree) } ,
        myCurPath{ {"/"} } ,
        myPrivData{ std::move(priv) }
    {
    }

    ~Context() = default;

    void changePath( CliPath const& newPath )
    {
        if ( myCliTree->pathExists(newPath) ) {
            myCurPath   = newPath;
            stream->writeLine(newPath.toString() + " >" );
        }
        else {
            stream->writeLine(Poco::format("path not found: '%s'\n", newPath.toString()) );
        }
    }
    
    [[nodiscard]] CliPath getCurPath() const
    {
        return myCurPath;
    }
    
    
    /**
     * @brief Get the current working path as a string
     *
     * Gets the current working path as a string. This is a short hand for:
     * @code
     *     ctx->getCurPath().toString();
     * @endcode
     * @return
     */
    [[nodiscard]] std::string getCwd() const
    {
        return myCurPath.toString();
    }

    AbstractCommandConstPtr findCommand(std::string const& command )
    {
        return myCliTree->lookupCmd(myCurPath,command);
    }
    
    /**
     * @brief list commands, scripts and paths
     *
     * See CommandTree::showPath() for more information
     * @return a tiple with 3 arrays: commands, scripts and paths
     */
    [[nodiscard]] auto listAll() const
    {
        return myCliTree->showPath(myCurPath);
    }
    

    Stack           stack;
    std::shared_ptr<StreamAdapter> const stream;

private:
    CommandTreeConstPtr myCliTree;
    CliPath         myCurPath;

    std::any        myPrivData;

    [[nodiscard]] std::string getWorkingPath() const
    {
        return myCurPath.toString();
    }
};


typedef std::shared_ptr<Context> ContextPtr;


} /* namespace rps */
