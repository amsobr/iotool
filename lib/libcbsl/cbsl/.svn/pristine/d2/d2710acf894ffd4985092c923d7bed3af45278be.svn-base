
#include <stdlib.h>
#include <stdint.h>

#include "norder_fifo.h"

struct nth_order_fifo {
    unsigned int order;
    size_t       rawSz;
    unsigned int mask;
    unsigned int wrPos;
    unsigned int rdPos;
    uint8_t     *buf;
};

static inline size_t p_used_bytes( NORDER_FIFO const *f )
{
    return (f->wrPos + f->rawSz - f->rdPos)&f->mask;
}


int fifo_create( NORDER_FIFO **fifo , unsigned int order )
{
    if ( order<2 ) {
        return FIFO_ERR_BAD_PARAM;
    }
    
    if ( order>13 ) { /* allow FIFOS up to 2048B in size */
        return FIFO_ERR_BAD_PARAM;
    }
    
    *fifo               = malloc( sizeof(NORDER_FIFO) );
    if ( *fifo==0 ) {
        return FIFO_ERR_ALLOC;
    }
    (*fifo)->order      = order;
    (*fifo)->rawSz      = 1<<order;
    (*fifo)->mask       = (*fifo)->rawSz - 1;
    (*fifo)->buf        = (uint8_t*)malloc( (*fifo)->rawSz );
    if ( (*fifo)->buf==0 ) {
        free( *fifo );
        return FIFO_ERR_ALLOC;
    }
    (*fifo)->wrPos      = 0;
    (*fifo)->rdPos      = 0;
    return FIFO_OK;
}


void fifo_destroy( NORDER_FIFO *fifo )
{
    free( fifo->buf );
    free( fifo );
}


unsigned int fifo_get_order( NORDER_FIFO const *fifo )
{
    return fifo->order;
}


ssize_t fifo_enqueue( NORDER_FIFO *fifo , void const *buf , size_t len )
{
    size_t avail    = fifo->rawSz - p_used_bytes(fifo) - 1;

    if ( avail==0 ) {
        return 0;
    }

    if ( avail<len ) {
        return FIFO_ERR_NOSPACE;
    }
    
    uint8_t *dst        = (uint8_t *)fifo->buf;
    uint8_t const *src  = (uint8_t *)buf;
    
    for ( size_t i=0 ; i<len ; ++i ) {
        dst[fifo->wrPos]    = src[i];
        fifo->wrPos         = (fifo->wrPos + 1)&fifo->mask;
    }
    
    return len;
}

size_t fifo_dequeue( NORDER_FIFO *fifo , void *buf , size_t maxLen )
{
    size_t avail    = p_used_bytes( fifo );
    size_t copyLen  = avail<maxLen ? avail : maxLen;
    
    uint8_t *dst    = (uint8_t *)buf;
    uint8_t *src    = (uint8_t *)fifo->buf;
    for ( size_t i=0 ; i<copyLen ; ++i ) {
        dst[i]      = src[fifo->rdPos];
        fifo->rdPos = (fifo->rdPos + 1)&fifo->mask;
    }
    return copyLen;
}


size_t fifo_size( NORDER_FIFO const *fifo )
{
    return p_used_bytes(fifo);
}


size_t fifo_capacity( NORDER_FIFO const *fifo )
{
    return fifo->rawSz - 1;
}


int fifo_empty( NORDER_FIFO const *fifo )
{
    if ( fifo->rdPos==fifo->wrPos ) {
        return 1;
    }
    return 0;
}


void fifo_clear( NORDER_FIFO *fifo )
{
    fifo->wrPos = 0;
    fifo->rdPos = 0;
}
