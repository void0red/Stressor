//
// Created by void0red on 5/18/19.
//

#ifndef STRESSOR_THREAD_H
#define STRESSOR_THREAD_H

#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define THREAD_LOG_STDOUT

#ifdef THREAD_LOG_STDOUT

#include "log.h"

#endif

#define THREAD_MAX_SIZE 128


typedef enum {
    NORMAL, DIED
} status_t;

typedef struct _job {
    void (*function)(void *);

    void *args;
    struct _job *next;
} job_t;

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t notify;

    pthread_t *threads;
    int threads_num;

    int jobs_num;
    job_t *head;
    job_t *tail;

    int running;
    status_t status;

} thread_pool;

static void *do_thread(void *_pool);

static int thread_pool_free(thread_pool *pool);

extern thread_pool *thread_pool_create(int threads_num);

extern int thread_pool_destroy(thread_pool *pool);

extern int thread_pool_add(thread_pool *pool, void (*function)(void *), void *args);


#endif //STRESSOR_THREAD_H
