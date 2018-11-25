#if !defined( TIME_CHECK_H )
#define TIME_CHECK_H

#include <time.h>
#include <string.h>

enum {
    MAX_CHECKPOINTS__   = 16 ,
    MAX_DESC_LEN__      = 64 ,
};

typedef struct {
    unsigned int    numEntries;
    struct {
        struct timespec t;
        char            desc[MAX_DESC_LEN__];
    } checkpoint[MAX_CHECKPOINTS__];
} TIME_CHECK;


/** \brief Initialize a time check structure. 0 entries. All fields blank.
 * \param tc Pointer to the structure to be initialized.
 */
static inline void time_check_init( TIME_CHECK *tc )
{
    memset( tc , 0 , sizeof(TIME_CHECK) );
}


/** \brief Start collecting time information
 * \param tc    Pointer to the structure.
 * \param desc  Optional string that may be used to describe the checkpoint.
 *              If not used, set to 0
 */
static inline void time_check_start( TIME_CHECK *tc , char const *desc )
{
    tc->numEntries  = 1;
    clock_gettime( CLOCK_MONOTONIC , &tc->checkpoint[0].t );
    if ( desc ) {
        strncpy( &tc->checkpoint[0].desc[0] , desc , MAX_DESC_LEN__ );
    }
    else {
        tc->checkpoint[0].desc[0]   = '\0';
    }
}


static inline void time_check_add( TIME_CHECK *tc , char const *desc )
{
    if ( tc->numEntries<MAX_CHECKPOINTS__ ) {
        clock_gettime( CLOCK_MONOTONIC , &tc->checkpoint[tc->numEntries].t );
        if ( desc ) {
            strncpy( &tc->checkpoint[tc->numEntries].desc[0] , desc , MAX_DESC_LEN__ );
        }
        else {
            tc->checkpoint[tc->numEntries].desc[0]  = '\0';
        }
    }
    ++tc->numEntries;
}


static inline void time_check_report( TIME_CHECK *tc , FILE *s )
{
    fprintf( s , "TIME REPORT:\n"
                 "+-------+----------+-----------+\n"
                 "| ENTRY | REL TIME | DIFF LAST | DESC (optional)\n"
                 "+-------+----------+-----------+\n"
    );
    struct timespec tIni;
    struct timespec tLast;
    for ( unsigned int i=0 ; i<tc->numEntries ; ++i ) {
        unsigned long int relUs , diffUs;
        if ( i==0 ) {
            tIni.tv_sec     = tc->checkpoint[0].t.tv_sec;
            tIni.tv_nsec    = tc->checkpoint[0].t.tv_nsec;
            relUs           = 0;
            diffUs          = 0;
        }
        else {
            relUs           = (tc->checkpoint[i].t.tv_sec-tIni.tv_sec)*1000000
                                + (tc->checkpoint[i].t.tv_nsec-tIni.tv_nsec)/1000;
            diffUs          = (tc->checkpoint[i].t.tv_sec-tLast.tv_sec)*1000000
                                + (tc->checkpoint[i].t.tv_nsec-tLast.tv_nsec)/1000;
        }
        fprintf( s , "| % 5d | % 8ld | % 8ld | %s\n", i , relUs , diffUs , tc->checkpoint[i].desc );
        
        tLast.tv_sec        = tc->checkpoint[i].t.tv_sec;
        tLast.tv_nsec       = tc->checkpoint[i].t.tv_nsec;
    }
}

#endif /* !defined( TIME_CHECK_H ) */
