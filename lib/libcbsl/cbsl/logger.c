#include "logger.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>

/* Note about implementation:
 * Although an RWlock might look more attractive, in practice it causes more
 * contention than a plain Mutex. The tests I made 
 * The OS/stdlib itself has a mutual exclusion mechanism attached to the FILE
 * stream, so that writes into the handle are serialized. If we let several
 * threds compete for access to the file stream the performance hit is slightly
 * bigger
 */

struct logger_priv {
  FILE *os;
};

static const int maxlen = 28;

void
logger_init(logctl_t *logger, FILE *s)
{
  logger->priv = malloc(sizeof(struct logger_priv));
  logger->priv->os = s;
}

void
logger_cleanup(logctl_t *logger)
{
  free(logger->priv);
}

char const *
logger_sev_name(severity_t sv)
{
  switch (sv)
    {
  case SEV_FATAL:
    return "FATAL";
  case SEV_CRITICAL:
    return "CRITICAL";
  case SEV_ERROR:
    return "ERROR";
  case SEV_WARNING:
    return "WARNING";
  case SEV_NOTICE:
    return "NOTICE";
  case SEV_INFO:
    return "INFO";
  case SEV_DEBUG:
    return "DEBUG";
  case SEV_TRACE:
    return "TRACE";
  default:
    return "???";
    }
}

void
logger_change_stream(logctl_t *logger, FILE *stream)
{
  logger->priv->os = stream;
}

/** DEPRECATED METHOD. DO NOT USE IT PLEASE!!!
 * \warning This method is unsafe and will be removed ASAP. Do not use it for
 * new applications.
 */
FILE *
logger_get_stream(logctl_t *logger)
{
  return logger->priv->os;
}

void
logger_print(logctl_t const *ctl,
             severity_t level,
             char const *file,
             int line,
             const char *fmt,
             ...)
{
  if (ctl == 0) {
    /* there is no logger control defined in our client, thus we cannot
     * print at all.
     */
    return;
  }

  if (level > ctl->min_severity) {
    return;
  }
  /* build the "here" string with up to maxlen bytes:
   * basically, just find an offset if the string is longer than
   * maxlen
   */

  char const *levelStr[] =
    {"     ", "FATAL", "CRIT ", "ERROR", "WARN ", "NOTE ", "INFO ", "DEBUG",
        "TRACE", "UMSKD", "*****", }; // unmasked

  int len;

  char hereStr[1024];

  len = snprintf(hereStr, 255, "%s: %d", file, line);

  int strOffset = 0;
  if (len > maxlen) {
    strOffset = len - maxlen;
    hereStr[strOffset + 0] = '.';
    hereStr[strOffset + 1] = '.';
    hereStr[strOffset + 2] = '.';
  }

  char fmtStr[256];
  int lvidx = level;
  if (level > SEV_TRACE) {
    lvidx = 10;
  }

  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);

  struct tm tm;
  localtime_r(&ts.tv_sec, &tm);

  snprintf(fmtStr, 255, "%02d-%02d:%02d:%02d.%06u %s %s %s",
           tm.tm_mday,
           tm.tm_hour,
           tm.tm_min,
           tm.tm_sec,
           (unsigned int)(ts.tv_nsec / 1000L),
           levelStr[lvidx],
           hereStr + strOffset,
           fmt);

  va_list args;
  va_start(args, fmt);
  vfprintf(ctl->priv->os, fmtStr, args);
#ifndef LOGGER_NO_FLUSH    
  fflush(ctl->priv->os);
#endif
  va_end(args);
}
