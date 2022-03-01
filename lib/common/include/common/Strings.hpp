
#pragma once

#include <cstring>
#include <string>
#include <vector>


class Strings
{
public:
    /**
     * Get a human readable description of an error number.
     * @param e the error number. Usually taken from errno.h and most standard
     *      library calls
     * @return A string corresponding to the error number, as would be returned
     * by strerror()
     */
    static std::string fromErrno( int e )
    {
        char str[128];
        [[maybe_unused]] char* s;
        s = strerror_r(e,str,sizeof(str));
        return std::string{s};
    }

    /**
     * Split a string into a vector of tokens.
     * @param in The input string
     * @param delimiter The delimiter to use for splitting.
     * @param mergeDelimiters @li true - consecutive separators are merged and no string
     *                  is extracted.
     *                  @li false - consecutive separators are not merge, and an
     *                  empty string will be generated between each 2 adjacent
     *                  separators.
     * @return A vector with the tokens extracted from the input string.
     */
    static std::vector<std::string> split(std::string const& in , std::string const& delimiter , bool mergeDelimiters=true );


    /**
     * @brief Find and replace in a string.
     *
     * @details Finds all occurences of a substring in the input string and
     * replaces them using the provided replacement.
     *
     * The input string is modified in place. In case the caller wants to
     * preserve the original string, it must provide a copy of the original.
     * @param in The input string.
     * @param from substring to be replaced.
     * @param to replacement substring
     * @return The number of replacements that were performed. If zero, the input
     *      string remains untouched.
     */
    static int replace(std::string* in , std::string const& from , std::string const& to );


    /**
     * Check if a string ends with a given ending
     * @param in The string
     * @param ending The ending to look for
     * @return true if @a in ends with @a ending, otherwise false
     */
    static bool endsWith( std::string const&in , std::string const& ending );

}; /* class Strings */

