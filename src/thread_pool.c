//
// Created by void0red on 5/15/19.
//

#include "thread_pool.h"

static int job_queue_init(job_queue_ptr obj) {
    obj->length = 0;
    obj->front = NULL;
    obj->rear = NULL;
    pthread_mutex_init(&obj->mutex, NULL);

    return 0;
}

static void job_queue_clear(job_queue_ptr obj) {
    pthread_mutex_lock(&obj->mutex);
    job_ptr p = obj->front;
    while (obj->length) {
        job_ptr q = p;
        p = p->next;
        if (q) {
            free(q);
            obj->length -= 1;
        } else {
            break;
        }
    }
    pthread_mutex_unlock(&obj->mutex);
}

static void job_queue_push(job_queue_ptr obj, job_ptr new_job) {
    pthread_mutex_lock(&obj->mutex);
    new_job->next = NULL;

    if (obj->length != 0) {
        obj->rear->next = new_job;
    } else if (obj->length == MAX_JOB_SIZE) {
        pthread_mutex_unlock(&obj->mutex);
        return;
    } else {
        obj->front = new_job;
    }

    obj->rear = new_job;
    obj->length += 1;
    pthread_mutex_unlock(&obj->mutex);
}

static job_ptr job_queue_pull(job_queue_ptr obj) {
    pthread_mutex_lock(&obj->mutex);
    job_ptr job = obj->front;
    switch (obj->length) {
        case 0:
            break;
        case 1:
            obj->front = NULL;
            obj->rear = NULL;
            obj->length = 0;
            break;
        default:
            obj->front = job->next;
            obj->length -= 1;
    }
    pthread_mutex_unlock(&obj->mutex);
    return job;
}

static void job_queue_destroy(job_queue_ptr obj) {
    job_queue_clear(obj);
    free(obj);
}


static void *thread_do(pthread_t *thread) {

}

thread_pool_ptr thread_pool_init(int num) {
    if (num < 0)
        num = 0;
    thread_pool_ptr pool = (thread_pool_ptr) malloc(sizeof(thread_pool));
    if (pool == NULL) {
        return NULL;
    }
    pool->threads_map = 0;
    pool->threads_alive = 0;
    pool->threads_working = 0;

    if (job_queue_init(&pool->jobs)) {
        free(pool);
        return NULL;
    }

    pool->threads = (pthread_t **) malloc(sizeof(pthread_t *) * num);
    if (pool->threads == NULL) {
        free(pool);
        return NULL;
    }

    pthread_mutex_init(&pool->mutex, NULL);

    for (int i = 0; i < num; ++i) {
        pthread_create(pool->threads[i], NULL, (void *) thread_do, pool->threads[i]);
        pthread_detach(*(pool->threads[i]));

        pool->threads_map |= (1u << i);
    }


    while (pool->threads_alive != num) {

    }

    return pool;
}

