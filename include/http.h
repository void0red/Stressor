//
// Created by void0red on 5/15/19.
//

/*
 * only implement of the GET request
 */

#ifndef STRESSOR_HTTP_H
#define STRESSOR_HTTP_H

#include <curl/curl.h>

/*
 * http version
 */
typedef enum _http_version {
    HTTP_1_1, HTTP_2_0
} http_version;


/*
 * http request method
 * GET only
 */
typedef enum _http_method {
    HTTP_None, HTTP_GET
} http_method;


/*
 * tuple struct:
 * description:     implement of name-value tuple like map
 *
 * @member  name    const char*     name
 * @member  value   const char*     value
 * @member  next    struct _tuple*  next tuple pointer
 */
typedef struct _tuple {
    const char *name;
    const char *value;
    struct _tuple *next;
} tuple;


/*
 * http_header struct:
 * description:     http header message
 *
 * @member  data    tuple*          tuple data
 */
typedef struct _http_header {
    tuple *data;
} http_header;


/*
 * http_request struct:
 * description:     http request
 *
 * @member  request_method      http_method     http request method
 * @member  ulr                 const char*     request location
 * @member  version             http_version    the version of http
 * @member  headers             http_header     request headers
 */
typedef struct _http_request {
    http_method request_method;
    const char *url;
    http_version version;
    http_header headers;
} http_request;


/*
 * http_response struct:
 * description:     http response
 *
 * @member  version             http_version    the version of http
 * @member  status_code         int             status code
 * @member  status_message      const char*     the message of the status
 * @member  headers             http_header     response headers
 */
typedef struct _http_response {
    http_version version;
    int status_code;
    const char *status_message;
    http_header headers;
} http_response;

typedef struct _connection {
    http_request request;
    http_response response;
} connection;

void http_init();

void http_destroy();


# endif //STRESSOR_HTTP_H
