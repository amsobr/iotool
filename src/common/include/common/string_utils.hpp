//
// Created by to on 09/04/19.
//

#ifndef IOTOOL_STRING_UTILS_HPP
#define IOTOOL_STRING_UTILS_HPP

#include <vector>
#include <string>
#include <list>

namespace StringUtils
{

/**
 * \brief Split a string into tokens, given a separator
 * \details Splits str into tokens, given a separator string.
 *          When there are multiple consecutive occurences of separator
 *          in str, those will be merged depending on mergeSeparators.
 *
 *          The resulting tokens are placed into dest.
 * \note dest is not cleared prior to the splitting of str. The split
 *      tokens are push_back'ed into dest
 *
 * \param str String to be split
 * \param separator String holding the separator
 * \param dest Vector where resulting strings will be placed.
 * \param mergeSeparators If set to true, separators are merged.
 *                      If false, an empty string is saved whenever
 *                      two consecutive separators are found.
 * \return The number of tokens push_back'ed into dest
 */
size_t split(std::string const &str, std::string const &separator, std::vector<std::string> &dest , bool mergeSeparators=true )
{
    size_t curTokenPos(0);
    size_t const strLen(str.size());
    size_t const sepLen(separator.size());

    size_t numTokens(0);
    while( curTokenPos<strLen ) {
        size_t stt = str.find( separator , curTokenPos );
        if ( stt!=std::string::npos ) {
            /* if stt==curTokenPos, there are consecutive separators...
             */
            if ( stt!=curTokenPos ) {
                dest.push_back(str.substr(curTokenPos, stt-curTokenPos));
                numTokens++;
            }
            else {
                if ( !mergeSeparators ) {
                    dest.push_back("");
                    numTokens++;
                }

            }
            curTokenPos = stt + sepLen;
        }
        else {
            /* separator not found, so use all of the remaining string.
             * remember, we are at or behind the last character of str...
             */
            dest.push_back( str.substr(curTokenPos) );
            numTokens++;
            break;
        }
    }
    return numTokens;
}


};

#endif //IOTOOL_STRING_UTILS_HPP
