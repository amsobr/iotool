#ifndef IOTOOL_CONFIG_HPP
#define IOTOOL_CONFIG_HPP

//#define IOTOOL_VERSION VER_MAJOR VER_MINOR VER_PATCH "-" VER_EXTRA

//#define IOTOOL_LOG_FILE_GENERAL "/var/log/iotool.log"
//#define IOTOOL_TCP_LISTEN_PORT 1337


namespace Iotool
{
    constexpr char const VERSION[]   = VER_MAJOR "." VER_MINOR "." VER_PATCH "-" VER_EXTRA;
    constexpr char const LOG_FILE[]  = "/var/log/iotool/iotool.log";
    constexpr unsigned int TCP_LISTEN_PORT = 1337;
    constexpr unsigned int TCP_DATA_SERVER_PORT = 1338;
    constexpr char const SCHEDULERS_PATH[] = "schedulers";
    constexpr char const TRANSFORM_JOBS_PATH[] = "transform_jobs";
    constexpr char const RPNLIB_ADDONS_PATH[] = "rpnlib_addons";
    constexpr char const OUTPUT_CHANNELS_PATH[] = "output_channels";
};




#endif
