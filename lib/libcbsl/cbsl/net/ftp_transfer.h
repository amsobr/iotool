#ifndef FTP_TRANSFER_H
#define FTP_TRANSFER_H

#ifdef __cplusplus
# define EXTERN_C extern "C"
#else
# define EXTERN_C extern
#endif

typedef struct ftp_file {
    /** URI that points to the file.\n
     * Should be in the form ftp://ip_address/path/to/file.xyz
     */
    char    uri[256];
    char    ftpUser[64];
    char    ftpPass[64];
} FTP_FILE;

enum {
    FTP_TRANSFER_OK         = 0 ,
    FTP_TRANSFER_TIMEOUT    = 1 ,
};

EXTERN_C int ftp_get( char const *dest , FTP_FILE const *remFile );


#undef EXTERN_C
#endif /* FTP_TRANSFER_H */
