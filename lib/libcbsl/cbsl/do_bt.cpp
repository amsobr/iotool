
#include <stdlib.h>
#include <execinfo.h>
#include <stdio.h>
#include <string.h>


#include <pti/thread.h>

//static char bt_buf[20][1<<16];

static int const BT_SZ  = 100;
void do_bt( /*int id ,*/ char const *extra_info )
{
    void *bt[BT_SZ];
    pid_t tid   = Pti::Thread::getTid();
    
    int bt_len  = backtrace( bt , BT_SZ );
    char **bt_lines = backtrace_symbols( bt , bt_len );
    char buf[1<<16];
    int pos = 0;
    if ( 0!=extra_info ) {
        pos += sprintf( buf , "\n====== NEW BACKTRACE ====\nTID=%d [%s]\n", tid , extra_info );
    }
    else {
        pos += sprintf( buf , "\n====== NEW BACKTRACE ====\nTID=%d\n", tid );
    }
    for ( int i=0 ; i<bt_len ; ++i ) {
        pos += sprintf( buf+pos , "#%02d: %s\n" , i , bt_lines[i] );
    }
    fprintf( stderr , "%s\n" , buf );
    free(bt_lines);
}

void bt_dump()
{
    fprintf( stderr , "\n\n======== LAST BACKTRACES OF EACH THREAD ========\n" );
//     for ( int i=0 ; i<20 ; ++i ) {
//         fprintf( stderr , "%s\n", &bt_buf[i][0] );
//     }
}


void bt_get_caller( char *caller , size_t maxlen )
{
    /* we must descend 3 levels to get up to the caller of our own caller.
     * Our backtrace should be something like:
     * 0: our selves
     * 1: our caller
     * 2: our caller's caller <--- we're looking for this one
     */
    void *bt[3];
//     pid_t tid   = Pti::Thread::getTid();
    
    int bt_len  = backtrace( bt , 3 );
    char **bt_lines = backtrace_symbols( bt , bt_len );
    if ( bt_len>=3 ) {
        strncpy( caller , bt_lines[2] , maxlen );
    }
    else {
        caller[0]   = '\0';
    }
    free(bt_lines);
}

