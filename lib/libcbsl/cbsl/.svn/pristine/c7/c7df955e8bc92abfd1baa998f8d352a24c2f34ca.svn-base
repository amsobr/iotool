#ifndef FIFO_H
#define FIFO_H

#include <stddef.h>

/** \brief N-order fifo module
 * \details This module implements an N-order FIFO. An N-order FIFO can hold
 * up to (2^N) - 1 elements. Being N-order, the math to maintain its internal
 * points can be made very efficient.
 */

#ifdef __cplusplus
#define FIFO_EXTERN extern "C"
#else
#define FIFO_EXTERN extern
#endif

typedef struct nth_order_fifo NORDER_FIFO;

typedef enum {
    FIFO_OK                 = 0  ,
    FIFO_ERR_BAD_PARAM      = -1 ,
    FIFO_ERR_ALLOC          = -2 ,
    FIFO_ERR_NOSPACE        = -3 ,
} NORDER_FIFO_STATUS;


/** \brief Create a fifo object.
 * \param fifo  Pointer to location where the pointer to the new fifo will be
 *              saved into.
 * \param order Order of the fifo. The capacity of the new fifo is a power
 *              of two with exponent \a order.
 * \return Returns FIFO_OK (0) on success or some other error.
 */
FIFO_EXTERN int fifo_create( NORDER_FIFO **fifo , unsigned int order );

/** \brief Destroy a fifo object
 */
FIFO_EXTERN void fifo_destroy( NORDER_FIFO *fifo );


FIFO_EXTERN unsigned int fifo_get_order( NORDER_FIFO const *fifo );

/** \brief Enqueue data into the FIFO
 * \param fifo Pointer to fifo object
 * \param buf Pointer to location holding data to copy into the \a fifo
 * \param len Size of data to enqueue.
 * \return If the FIFO had enough capacity, \a len is returned, otherwise
 * returns the number of bytes copied from \a buf. In case of error returns a
 * negative value.
 */
FIFO_EXTERN ssize_t fifo_enqueue( NORDER_FIFO *fifo , void const *buf , size_t len );

/** \brief Read from FIFO
 * \details Reads at most \a maxLen bytes from the FIFO into \a buf
 * \param fifo Pointer to fifo object
 * \param buf Pointer to location where data will be stored.
 * \warning The caller must enure enough space in \a buf to hold \a maxLen bytes.
 * \param maxLen Maximum ammount of bytes to save into \a buf
 * \return Returns the number of bytes  copied into \a buf or an error code( <0 )
 */
FIFO_EXTERN size_t fifo_dequeue( NORDER_FIFO *fifo , void *buf , size_t maxLen );


/** \brief Get the number of bytes currently held by fifo 
 */
FIFO_EXTERN size_t fifo_size( NORDER_FIFO const *fifo );

/** \brief Get the capacity of the fifo. 
 */
FIFO_EXTERN size_t fifo_capacity( NORDER_FIFO const *fifo );

/** \brief Check if a fifo is empty */
FIFO_EXTERN int fifo_empty( NORDER_FIFO const *fifo );

/** \brief Clear all data in the FIFO. FIFO becomes empty
 */
FIFO_EXTERN void fifo_clear( NORDER_FIFO *fifo );


#undef FIFO_EXTERN

#endif /* !defined FIFO_H */
