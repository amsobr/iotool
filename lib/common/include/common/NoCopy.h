
#pragma once

/** @brief Explicitly disable copy construction and assigment
 *
 * Place in public section of the class declaration as shown:
 * @code
 * class T {
 * public:
 *     NO_COPY(T)
 * };
 * @endcode
 *
 * No semicolon is needed.
 */

#define NO_COPY(T) \
    T( T const& o) = delete; \
    T& operator=( T const& rhs ) = delete; \

