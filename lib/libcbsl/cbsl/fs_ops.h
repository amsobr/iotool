#ifndef FS_OPS_H
#define FS_OPS_H

/** \file 	fs_ops.h
 * \brief	Filesystem operations.
 * \details	This module implements common filesystem operations, 
 *			which make the life of the programmer easier and reduces the
 *			number of lines of code written to accomplish some operations.
 *			Implemented functions are:
 *				- file size - fs_size()
 * \author	António Oliveira - Withus, Lda (antonio.oliveira@withus.pt)
 */

#undef EXTERN
#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN extern
#endif
/** \enum	fs_retcode_t
 * \brief	Return codes for fs_op function calls.
 */
typedef enum fs_ret_code {
	FS_OK			= 0		, /**< No Error */
	FS_ERROR		= -1	, /**< Some Error, don't ask which :p */
	/** \brief 	Invalid permissions
	 * \details	Caller does not have enough permissions to perform
	 *			the operation.
	 */
	FS_ERR_PERM		= -2	,
	FS_ERR_TYPE		= -3	, /**< File given is of the wrong type */
	FS_ERR_PATH		= -4		, /**< Invalid path given */
} fs_retcode_t;

/** \brief	Calculate the size of a file
 * \details	Returns the size, in bytes, of file. File must be a regular
 *			file, or the result may be unpredictable.
 * \param file	Path to the file. Must be a regular file.
 * \return	Returns the size, in bytes, of file. If an error occurs,
 *			returns negative number, with the appropriate error code.
 */
EXTERN long int fs_size( char const *file );


/** \brief Slurp an entire file into a memory buffer.
 * \details	Slurps the whole contents of file into a buffer given by the
 *			caller. buf must be allocated by the caller and must ensure 
 *			that the whole file fits without a buffer overflow.
 * \param	buf	Pointer to the memory buffer where the whole contents of
 *				file will be read into.
 * \param	path	Path to the file to be read. The file is read in
 *					binary mode.
 * \param	maxLen	Maximum length to read. Set to zero to read the while
 *					the file has data to read. If set to a non-zero value,
 *					ensures that no more than maxLen bytes are read.
 * \return	Returns the number of bytes read. If some error occurs,
 *			returns a negative value, which is an error code.
 */
EXTERN long int fs_slurp( char *buf , char const *path , long int maxLen );

#undef EXTERN
#endif /* FS_OPS_H */
