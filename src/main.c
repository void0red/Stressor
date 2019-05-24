#include <stdio.h>
#include <sys/stat.h>
#include "thread.h"
#include "http_get.h"
#include "log.h"


static void ping(void *url) {
    http_response *response = http_get(url);
    if (response->ok) {
        Global_File_Logger.write(INFO, "ok", 2);
    } else {
        Global_File_Logger.write(ERROR, "error", 5);
    }
    http_clean(response);
}

int main(int argc, char *argv[]) {
    char *url = NULL;
    char *log_path = NULL;
    int times = 100;
    int thread_nums = 64;
    switch (argc) {
        default:
            fprintf(stderr, "Usage: %s url [log path] [ping times] [thread nums]\n", argv[0]);
            return 0;
        case 5:
            thread_nums = strtol(argv[4], NULL, 10);
        case 4:
            times = strtol(argv[3], NULL, 10);
        case 3:
            log_path = argv[2];
        case 2:
            url = argv[1];
            break;
    }
    if (log_path != NULL) {
        log_init(&Global_File_Logger, log_path);
    } else {
        log_init(&Global_File_Logger, "./log.txt");
    }
    thread_pool *pool = thread_pool_create(thread_nums);
    for (int i = 0; i < times; ++i) {
        thread_pool_add(pool, ping, url);
    }
    thread_pool_destroy(pool);

    return 0;
}