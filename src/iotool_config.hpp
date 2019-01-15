#ifndef IOTOOL_CONFIG_HPP
#define IOTOOL_CONFIG_HPP

//#define IOTOOL_VERSION VER_MAJOR VER_MINOR VER_PATCH "-" VER_EXTRA

//#define IOTOOL_LOG_FILE_GENERAL "/var/log/iotool.log"
//#define IOTOOL_TCP_LISTEN_PORT 1337


namespace Iotool
{
    constexpr char const VERSION[]   = VER_MAJOR "." VER_MINOR "." VER_PATCH "-" VER_EXTRA;
    constexpr char const LOG_FILE[]  = "/var/log/iotool.log";
    constexpr unsigned int TCP_LISTEN_PORT = 1337;
};




#endif
