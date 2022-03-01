
#pragma once

#include <map>

#include <common/PeripheralType.hpp>

#include <rps/AbstractCommand.hpp>


class PeripheralCmdProvider
{
public:

    PeripheralCmdProvider();
    ~PeripheralCmdProvider()    = default;

    /** @the index of all available commands, grouped by peripheral type
     *
     * This index is populated on startup and may be checked for all available
     * commands.
     *
     * @warning Do not change the contents of groups during application
     *          execution.
     */
    std::map< PeripheralType , std::vector<rps::AbstractCommandConstPtr> >  groups;
}; /* class PeripheralCmdProvider */


