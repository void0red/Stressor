//
// Created by void0red on 5/18/19.
//

#include "thread.h"

thread_pool *thread_pool_create(int threads_num) {
    if (threads_num < 0 || threads_num > THREAD_MAX_SIZE)
        return NULL;

    thread_pool *pool = (thread_pool *) malloc(sizeof(thread_pool));

#ifdef THREAD_LOG_STDOUT
    log_init(&Global_Stdout_Logger, NULL);
#endif

    pool->threads_num = 0;
    pool->head = pool->tail = NULL;
    pool->jobs_num = 0;

    pool->status = NORMAL;
    pool->running = 0;

    pool->threads = (pthread_t *) malloc(sizeof(pthread_t) * threads_num);

    pthread_mutex_init(&(pool->mutex), NULL);
    pthread_cond_init(&(pool->notify), NULL);

    for (int i = 0; i < threads_num; ++i) {
        pthread_create(&(pool->threads[i]), NULL, do_thread, pool);
        pool->threads_num += 1;
        pool->running += 1;
    }

    return pool;
}

int thread_pool_add(thread_pool *pool, void (*function)(void *), void *args) {
    if (pool == NULL || function == NULL)
        return -1;

    job_t *new_job = (job_t *) malloc(sizeof(job_t));
    new_job->function = function;
    new_job->args = args;
    new_job->next = NULL;

    pthread_mutex_lock(&(pool->mutex));
    if (pool->status == DIED) {
        pthread_mutex_unlock(&(pool->mutex));
        return -1;
    }
    if (pool->jobs_num == 0) {
        pool->head = pool->tail = new_job;
    } else {
        pool->tail->next = new_job;
        pool->tail = new_job;
    }
    pool->jobs_num += 1;
#ifdef THREAD_LOG_STDOUT
    Global_Stdout_Logger.write(INFO, "job added", 9);
#endif
    pthread_cond_signal(&(pool->notify));
    pthread_mutex_unlock(&(pool->mutex));
    return 0;
}

static int thread_pool_free(thread_pool *pool) {
    if (pool == NULL || pool->running > 0)
        return -1;
    if (pool->threads) {
        free(pool->threads);
    }
    pthread_mutex_destroy(&(pool->mutex));
    pthread_cond_destroy(&(pool->notify));
    free(pool);
    return 0;
}

int thread_pool_destroy(thread_pool *pool) {
    if (pool == NULL)
        return -1;

    pthread_mutex_lock(&(pool->mutex));

    if (pool->status == DIED) {
        pthread_mutex_unlock(&(pool->mutex));
        return -1;
    }

    pool->status = DIED;
#ifdef THREAD_LOG_STDOUT
    Global_Stdout_Logger.write(INFO, "pool dying", 10);
#endif
    pthread_cond_broadcast(&(pool->notify));
    pthread_mutex_unlock(&(pool->mutex));

    for (int i = 0; i < pool->threads_num; ++i) {
        pthread_join(pool->threads[i], NULL);
    }
#ifdef THREAD_LOG_STDOUT
    log_destroy(&Global_Stdout_Logger);
#endif

    return thread_pool_free(pool);
}

static void *do_thread(void *_pool) {
    thread_pool *pool = _pool;
    job_t job;
    while (1) {
        pthread_mutex_lock(&(pool->mutex));
        while ((pool->jobs_num == 0) && (pool->status != DIED)) {
            pthread_cond_wait(&(pool->notify), &(pool->mutex));
        }
        if (pool->status == DIED && pool->jobs_num == 0)
            break;

        job.function = pool->head->function;
        job.args = pool->head->args;
        job.next = NULL;

        job_t *p = pool->head;
        pool->head = pool->head->next;
        free(p);
        pool->jobs_num -= 1;
        pthread_mutex_unlock(&(pool->mutex));

        (*(job.function))(job.args);
#ifdef THREAD_LOG_STDOUT
        Global_Stdout_Logger.write(INFO, "job start", 9);
#endif
    }
    pool->running -= 1;
    pthread_mutex_unlock(&(pool->mutex));
    pthread_exit(NULL);
}
