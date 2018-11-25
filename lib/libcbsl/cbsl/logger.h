/** \file 	logger.h
 * \brief	Module for log facilities
 * \details	This class belongs to the Pti library
 *			This helps keeping all the code organized, and to separate
 *			common bits from device specific bits.
 *			The only assumption made here is that we have a full linux
 *			system to work on top of. Other than that, any assumprion
 *			about the platform on whose top this will be run is
 *			absolutelly forbidden.
 * \note	This code should be reusable in \em any device \b without 
 *			\b any \b modifications. Any change that might avoid this 
 *			belongs somewhere else, instead of this module.
 * \author	Antonio Oliveira (antonio.oliveira@withus.pt)
 */


#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C  extern
#endif

typedef enum {
    SEV_FORCE = 0   ,
    SEV_FATAL       ,
    SEV_CRITICAL    ,
    SEV_ERROR       ,
    SEV_WARNING     ,
    SEV_NOTICE      ,
    SEV_INFO        ,
    SEV_DEBUG       ,
    SEV_TRACE
} severity_t;


typedef struct {
    severity_t min_severity;
    struct logger_priv *priv;
} logctl_t;



/** \brief Initialize the logger
 */
EXTERN_C void logger_init( logctl_t *logger , FILE *s );


/** \brief Cleanup a logger's internal data */
EXTERN_C void logger_cleanup( logctl_t *logger );

EXTERN_C char const *logger_sev_name( severity_t sv );


void log_trace(     logctl_t const *ctl , char const *fmt , ... ) __attribute__((format(printf,2,3)));
void log_debug(     logctl_t const *ctl , char const *fmt , ... ) __attribute__((format(printf,2,3)));
void log_info(      logctl_t const *ctl , char const *fmt , ... ) __attribute__((format(printf,2,3)));
void log_notice(    logctl_t const *ctl , char const *fmt , ... ) __attribute__((format(printf,2,3)));
void log_warning(   logctl_t const *ctl , char const *fmt , ... ) __attribute__((format(printf,2,3)));
void log_error(     logctl_t const *ctl , char const *fmt , ... ) __attribute__((format(printf,2,3)));
void log_critical(  logctl_t const *ctl , char const *fmt , ... ) __attribute__((format(printf,2,3)));
void log_fatal(     logctl_t const *ctl , char const *fmt , ... ) __attribute__((format(printf,2,3)));
void log_always(    logctl_t const *ctl , char const *fmt , ... ) __attribute__((format(printf,2,3)));



#define log_trace( ctl , fmt , args... ) logger_print( ctl , SEV_TRACE , __FILE__ , __LINE__ , fmt , ##args )
#define log_debug( ctl , fmt , args... ) logger_print( ctl , SEV_DEBUG , __FILE__ , __LINE__ , fmt , ##args )
#define log_info(  ctl , fmt , args... ) logger_print( ctl , SEV_INFO  , __FILE__ , __LINE__ , fmt , ##args )
#define log_notice(ctl , fmt , args... ) logger_print( ctl , SEV_NOTICE, __FILE__ , __LINE__ , fmt , ##args )
#define log_warning(ctl, fmt , args... ) logger_print( ctl , SEV_WARNING,__FILE__ , __LINE__ , fmt , ##args )
#define log_error( ctl , fmt , args... ) logger_print( ctl , SEV_ERROR , __FILE__ , __LINE__ , fmt , ##args )
#define log_critical(ctl,fmt , args... ) logger_print( ctl , SEV_CRITICAL,__FILE__, __LINE__ , fmt , ##args )
#define log_fatal( ctl , fmt , args... ) logger_print( ctl , SEV_FATAL , __FILE__ , __LINE__ , fmt , ##args )
#define log_always( ctl , fmt , args... ) logger_print( ctl , SEV_FORCE , __FILE__ , __LINE__ , fmt , ##args )
    

static inline int FATAL_ENABLED(    logctl_t const *ctl ) { return ctl->min_severity>=SEV_FATAL;     }
static inline int CRITICAL_ENABLED( logctl_t const *ctl ) { return ctl->min_severity>=SEV_CRITICAL;  }
static inline int ERROR_ENABLED(    logctl_t const *ctl ) { return ctl->min_severity>=SEV_ERROR;     }
static inline int WARNING_ENABLED(  logctl_t const *ctl ) { return ctl->min_severity>=SEV_WARNING;   }
static inline int NOTICE_ENABLED(   logctl_t const *ctl ) { return ctl->min_severity>=SEV_NOTICE;    }
static inline int INFO_ENABLED(     logctl_t const *ctl ) { return ctl->min_severity>=SEV_INFO;      }
static inline int DEBUG_ENABLED(    logctl_t const *ctl ) { return ctl->min_severity>=SEV_DEBUG;     }
static inline int TRACE_ENABLED(    logctl_t const *ctl ) { return ctl->min_severity>=SEV_TRACE;     }

/** \brief Redirect the log stream to a new stream.
 */
EXTERN_C void logger_change_stream( logctl_t *logger , FILE *s );
EXTERN_C FILE *logger_get_stream(logctl_t *logger);


EXTERN_C void logger_print(
                    logctl_t const *ctl 
                    , severity_t level
                    , char const *file
                    , int         line
                    , char const *fmt
                    , ... 
                        ) __attribute__((format(printf,5,6)));

#undef EXTERN_C

#endif /* !defined(LOGGER_H) */
