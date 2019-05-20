//
// Created by void0red on 5/18/19.
//

#include "http_get.h"

static int http_write_callback(const void *ptr, size_t size, size_t n, void *stream) {
//    size_t real_size = size * n;
//
//    http_response *response = (http_response *) stream;
//    response->data = (char *) realloc(response->data, real_size + response->length);
//
//    memcpy(response->data, ptr, real_size);
//    response->length += real_size;

//    return real_size;
    return 0;
}

http_response *http_get(const char *url) {
    CURL *curl = curl_easy_init();

    http_response *response = (http_response *) malloc(sizeof(http_response));
    response->length = 0;

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http_write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

    int ok = curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response->status_code);

    response->ok = (ok != CURLE_ABORTED_BY_CALLBACK && response->status_code == 200) ? 1 : 0;

    curl_easy_cleanup(curl);

    return response;
}

void http_clean(http_response *response) {
    if (!response)
        return;
    free(response->data);
    free(response);
}