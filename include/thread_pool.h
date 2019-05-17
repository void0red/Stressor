//
// Created by void0red on 5/15/19.
//

#ifndef STRESSOR_THREAD_POOL_H
#define STRESSOR_THREAD_POOL_H

#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <limits.h>

#define MAX_THREAD_SIZE (8 * sizeof(uinsigned int))
#define MAX_JOB_SIZE UINT_MAX

/*
 * job struct:
 * @param   function    void(*)(*)      the real job function pointer
 * @param   args        void*           real job function param pointer
 * @param   next        struct _job*    the next job pointer
 * @param   start_time  time_t          job start time
 */
typedef struct _job {
    void (*function)(void *args);

    void *args;

    struct _job *next;

    time_t start_time;

} job, *job_ptr;


/*
 * job_queue struct:
 * @param   mutex       pthread_mutex_t     mutex locker
 * @param   front       job_ptr             job pointer pointed to queue head
 * @param   rear        job_ptr             job pointer pointed to queue tail
 * @param   length      unsigned int        the length of job queue
 */
typedef struct _job_queue {
    pthread_mutex_t mutex;
    job_ptr front;
    job_ptr rear;
    unsigned int length;
} job_queue, *job_queue_ptr;


/*
 * thread_pool struct:
 * @param   threads         pthread_t**         pointer to the real thread pointer
 * @param   threads_map     unsigned int        bit map to the threads
 * @param   threads_alive   unsigned int        alive threads
 * @param   threads_working unsigned int        working threads
 * @param   mutex           pthread_mutex_t     mutex locker
 * @param   jobs            job_queue           the job queue
 */
typedef struct _thread_pool {
    pthread_t **threads;
    unsigned int threads_map;
    unsigned int threads_alive;
    unsigned int threads_working;
    pthread_mutex_t mutex;
    job_queue jobs;
} thread_pool, *thread_pool_ptr;


static int job_queue_init(job_queue_ptr obj);

static void job_queue_clear(job_queue_ptr obj);

static void job_queue_push(job_queue_ptr obj, job_ptr new_job);

static job_ptr job_queue_pull(job_queue_ptr obj);

static void job_queue_destroy(job_queue_ptr obj);

static void *thread_do(pthread_t *thread);

thread_pool_ptr thread_pool_init(int num);


#endif //STRESSOR_THREAD_POOL_H
