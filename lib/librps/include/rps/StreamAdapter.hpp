#pragma once

#include <memory>
#include <string>

#include "NoCopy.hpp"

namespace rps
{


class StreamAdapter
{
public:
    NO_COPY(StreamAdapter)

    StreamAdapter()  = default;
    virtual ~StreamAdapter();


    [[nodiscard]] virtual std::string readLine() = 0;
    [[nodiscard]] virtual bool isEof() const = 0;
    virtual char readChar() = 0;
    virtual void writeLine(std::string const &s) = 0;
    virtual void putc(char c) = 0;

}; /* class StreamAdapter */

typedef std::shared_ptr<StreamAdapter> StreamAdapterPtr;


} /* namespace rps */
