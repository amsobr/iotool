/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2011, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at http://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/

/* this utility module is based on curl example:
 * ftpget.c
 * All credit goes to curl authors.
 */
#include <stdio.h>

#include <pti/logger.h>

#include <curl/curl.h>

#include "ftp_transfer.h"
#include "logctl.h"

/*
 * This is an example showing how to get a single file from an FTP server.
 * It delays the actual destination file creation until the first write
 * callback so that it won't create an empty file in case the remote file
 * doesn't exist or something else fails.
 */

struct FtpFile {
  const char *filename;
  FILE *stream;
};

static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
{
/*    LOG_PRINT( verb.omci , "Writing %d x %dB of file...\n" , nmemb , size ); */
  struct FtpFile *out=(struct FtpFile *)stream;
  if(out && !out->stream) {
    /* open file for writing */
    out->stream=fopen(out->filename, "wb");
    if(!out->stream)
      return -1; /* failure, can't open file to write */
  }
  return fwrite(buffer, size, nmemb, out->stream);
}


int ftp_get( char const *dest , FTP_FILE const *remFile )
{
    LOG_PRINT( verb.omci , "EXECUTING FTP TRANSFER...\n" );
  CURL *curl;
  struct FtpFile ftpfile={
    dest, /* name to store the file as if succesful */
    NULL
  };

  curl_global_init(CURL_GLOBAL_DEFAULT);

  curl = curl_easy_init();
  if(curl) {
    /*
     * You better replace the URL with one that works!
     */
    curl_easy_setopt(curl, CURLOPT_URL,
                     remFile->uri );
    /* Define our callback to get called when there's data to be written */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite);
    /* Set a pointer to our struct to pass to the callback */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ftpfile);
    char userpwd[128];
    snprintf( userpwd , sizeof(userpwd) , "%s:%s" , remFile->ftpUser , remFile->ftpPass );
    curl_easy_setopt(curl, CURLOPT_USERPWD , userpwd );
    /* Switch on full protocol/debug output */
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    CURLcode res = curl_easy_perform(curl);
    if ( res!=0 ) {
        LOG_PRINT( verb.omci , "CURL failed. res=%d\n" , res );
    }

    /* always cleanup */
    curl_easy_cleanup(curl);

    if(CURLE_OK != res) {
      /* we failed */
      LOG_PRINT( verb.omci , "curl told us %d\n", res);
      if ( res==CURLE_OPERATION_TIMEOUTED ) {
          return FTP_TRANSFER_TIMEOUT;
      }
      else {
          return -1;
      }
    }
  }

  if(ftpfile.stream)
    fclose(ftpfile.stream); /* close the local file */

  curl_global_cleanup();

  return 0;
}
