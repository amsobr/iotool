//
// Created by to on 17/03/19.
//

#pragma once

#include <memory>
#include <vector>
#include <string>
#include <tuple>
#include <cassert>

#include <common/Strings.hpp>

#include "NoCopy.hpp"
#include "ArgumentList.hpp"

namespace rps
{

class Script
{
public:
    NO_COPY(Script)

    typedef std::tuple<std::string,ArgumentList> Line;

    explicit Script( std::string name , std::vector<std::string> script ) :
    myName{ std::move(name) } ,
    myScript{ std::move(script) }
    {
        for ( auto const& line : myScript ) {
            auto tokens{ Strings::split(line," ") };
            assert(!tokens.empty());
            myLines.emplace_back(
                    std::make_tuple( tokens[0],ArgumentList{{tokens.begin()+1,tokens.end()}} )
                    );
        }
    }

    ~Script() = default;


    [[nodiscard]] std::string const& getName() const
    {
        return myName;
    }

    [[nodiscard]] std::vector<Line> getLines() const
    {
        return myLines;
    }



private:
    std::string                     myName;
    std::vector<std::string> const  myScript{};
    std::vector<Line>               myLines{};
};

typedef std::shared_ptr<Script> ScriptPtr;

}

