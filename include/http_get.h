//
// Created by void0red on 5/18/19.
//

#ifndef STRESSOR_HTTP_GET_H
#define STRESSOR_HTTP_GET_H

#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>


/*
 * http_response struct:
 *
 * @member      status_code     int         http response status code
 * @member      data            char*       response body data pointer
 * @member      length          int         response body data length
 * @member      ok              int         response status success && status code equal 200
 */
typedef struct _http_response {
    int status_code;
    char *data;
    int length;
    int ok;
} http_response;


/*
 * http_write_callback function:
 * description:     for curl to get response callback
 *
 * @param       ptr         const void*ptr      callback data pointer
 * @param       size        size_t              callback data size per packet
 * @param       n           size_t              callback numbers of packets
 * @param       stream      void*               callback output stream
 * @return      \           int                 real size of the data
 */
static int http_write_callback(const void *ptr, size_t size, size_t n, void *stream);


/*
 * http_get function:
 * description:     extern http get function
 *
 * @param       url         const char*         url param
 * @return      \           http_response       http response object
 */
http_response *http_get(const char *url);


/*
 * http_clean function:
 * description:     clean the http response object
 *
 * @param       response    http_response       http_response object need to clean
*/
void http_clean(http_response *response);

#endif //STRESSOR_HTTP_GET_H
