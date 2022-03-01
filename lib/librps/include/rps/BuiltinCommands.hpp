//
// Created by to on 18/03/19.
//

#pragma once

#include <vector>
#include <string>
#include <rps/Context.hpp>
#include <rps/ArgumentList.hpp>
#include <rps/Command.hpp>




namespace rps
{

std::vector<AbstractCommandConstPtr> getBuiltinCommands();

} /* namespace rps */



