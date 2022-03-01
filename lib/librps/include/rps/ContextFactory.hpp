//
// Created by to on 21/02/22.
//


#pragma once

#include <memory>

#include "Context.hpp"

namespace rps
{

class ContextFactory
{
public:
    explicit ContextFactory( CommandTreeConstPtr cmdTree ) :
    myCmdTree{ std::move(cmdTree) }
    {
    }
    
    ~ContextFactory()   = default;
    
    
    /**
     * @brief Create a Context using given IO adapter
     * @param stream The IO stream adapter. Ownership will be transferred to the
     *              newly built context. <em>No further use of ios is allowed
     *              after the call. </em>
     * @return A new Context
     */
    ContextPtr create( std::shared_ptr<StreamAdapter> stream )
    {
        return std::make_shared<Context>(std::move(stream) , myCmdTree );
    }
    
private:
    CommandTreeConstPtr myCmdTree;
};


typedef std::shared_ptr<ContextFactory> ContextFactoryPtr;

} /* namespace rps */

