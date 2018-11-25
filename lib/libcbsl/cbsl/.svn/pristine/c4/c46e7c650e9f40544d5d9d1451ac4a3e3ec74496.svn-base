
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>

#include "fs_ops.h"


long int fs_size( char const *file )
{
	struct stat file_stats;
	
	int ret = stat( file , &file_stats );
	if ( ret!=0 ) {
        int code = errno;
        fprintf( stderr , "Failed stat() on %s. errno=%d\n", file, code );
		return FS_ERROR;
	}
	return (long int)file_stats.st_size;
}


long int fs_slurp( char *buf , char const *path , long int maxLen )
{
	if ( 0==maxLen ) {
		maxLen	= fs_size( path );
		fprintf( stderr , "maxLen is zero. Adjusted to file size: %ld\n", maxLen );
	}
	
	FILE *fp	= fopen( path , "rb" );
	if ( 0 == fp ) {
		fprintf( stderr ,"Unable to open %s in read-binary mode.\n", path );
		return FS_ERR_PATH;
	}
	
	long int readSz	= fread( buf , 1 , maxLen , fp );
	fclose( fp );
	return readSz;
}
