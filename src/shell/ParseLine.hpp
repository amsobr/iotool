//
// Created by to on 20/02/22.
//


#pragma once

#include <tuple>

#include <rps/ArgumentList.hpp>

std::tuple< std::string , rps::ArgumentList > parseLine( std::string const& line );
