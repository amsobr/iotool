//
// Created by to on 18/02/22.
//


#include <vector>
#include <string>
#include <cctype>
#include <sstream>

#include <common/Strings.hpp>

#include "Exception.hpp"

#pragma once

namespace rps
{

/** @brief A CLI path.
 *
 * A CLI path is a sequence of path components which represent a certain node
 * in the CLI's tree.
 *
 * A path is represented as:<br>
 * /N1/N2/N3
 *
 * Each node, Nx @em must contain only letters and numbers. The CLI path entries
 * are case sensitive.
 *
 * The following path: /one/two/3 is represented as:<br>
 * @code
 * {"/","one","two","3"}
 * @endcode
 * Note the presence of the first element, `"/"`, which indicates an absolute
 * path.
 *
 */
class CliPath
{
public:
    /** @create a path
     *
     * @param path
     */
    CliPath( std::string const& path )
    {
        if ( path.empty() ) {
            throw InvalidArgumentsException{"paths cannot be empty"};
        }
        
        if ( path[0]=='/' ) {
            myIsAbsolute    = true;
        }
        else {
            throw InvalidArgumentsException{"relative pats not supported (for now)"};
        }
        
        myPath  = Strings::split(path,"/");
        // drop first element - "" because path starts with the separator
        myPath.erase(myPath.begin());
        
        auto it{myPath.cbegin() };
        while (it!=myPath.cend() ) {
            for( char const&c : *it ) {
                if ( !isalnum(c) ) {
                    throw InvalidArgumentsException{"path components only accept letters and numbers"};
                }
            }
            ++it;
        }
    }
    
    ~CliPath()  = default;

    [[nodiscard]] bool isAbsolute() const
    {
        return myIsAbsolute;
    }


    [[nodiscard]] std::string toString() const
    {
        std::ostringstream oss;
        bool first  = true;
        for ( auto const&s : myPath ) {
            if ( first ) {
                first   = false;
            }
            else {
                oss << "/";
            }
            oss << s;
        }
        return oss.str();
    }

    [[nodiscard]] explicit operator std::string() const { return toString(); }

    [[nodiscard]] std::string at( int pos ) const { return myPath.at(pos); }
    [[nodiscard]] std::string operator[]( int pos ) const { return myPath[pos]; }

    /* support range based loops */
    [[nodiscard]] auto begin() const{ return myPath.begin(); }
    [[nodiscard]] auto end() const { return myPath.end(); }
    [[nodiscard]] auto cbegin() const { return myPath.cbegin(); }
    [[nodiscard]] auto cend() const { return myPath.cend(); }

private:
    std::vector<std::string> myPath;
    bool                     myIsAbsolute;
};

} /* namespace rps */
