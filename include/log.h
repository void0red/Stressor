//
// Created by void0red on 5/15/19.
//

#ifndef STRESSOR_LOG_H
#define STRESSOR_LOG_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <locale.h>
#include <string.h>

/*
 * logger place
 */
typedef enum {
    TONone, TOFILE, TOSTDOUT, TOSTDERR
} out_t;

/*
 * logger level
 */
typedef enum {
    None, INFO, WARNING, ERROR
} level_t;


/*
 * logger struct:
 * @param   path    char*       log file path
 * @param   out     out_t       logger place
 * @param   fd      FILE*       the file pointer
 * @param   write     void(*)(*)  write function
 */
typedef struct _logger {
    char *path;

    out_t out;

    FILE *fd;

    void (*write)(level_t level, char *text, int len);


} logger, *logger_ptr;


/*
 * log_init function:
 * description:     do init for the global logger
 *
 * @param   log     logger_ptr      pointer to the logger
 * @param   path    char*           log file path, can be NULL
 * @param   out     out_t           where to log
 */
extern void log_init(logger_ptr log, char *path, out_t out);


/*
 * log_destroy function:
 * description:     do destroy for the global logger
 *
 * @param   log     logger_ptr      pointer to the logger
 */
extern void log_destroy(logger_ptr log);


/*
 * write_log function:
 * description:     the global logger write function
 *
 * @param   level   level_t         the level of the message
 * @param   text    char*           the message pointer
 * @param   len     int             the length of the `text` string
 */
static void write_log(level_t level, char *text, int len);


/*
 * get_time function:
 * description:     get the local time (default: UTC+8)
 *
 * @param   buf     char*           the buffer for the time string
 * @param   max     int             the max length of the time string
 */
static void get_time(char *buf, int max);


/*
 * info_write | warning_write | error_write function:
 * description:     the info | warning | error | level write function
 *
 * @param   text    char*           the message pointer
 * @param   len     int             the length of the `text` string
 * @param   out     FILE*           the output stream
 */
static void info_write(char *text, int len, FILE *out);

static void warning_write(char *text, int len, FILE *out);

static void error_write(char *text, int len, FILE *out);


/*
 * Global_Logger variable
 * description:     the global logger(only) exported
 *
 * @member  write     void(*)(*)      write_log
 * @member  path    char*           NULL
 * @member  fd      FILE*           NULL
 * @member  out     out_t           TONONE
 */
extern logger Global_Logger;

#endif //STRESSOR_LOG_H