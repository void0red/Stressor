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
    None, INFO, WARNING, ERROR, FORMAT
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
 */
extern void log_init(logger_ptr log, char *path);


/*
 * log_destroy function:
 * description:     do destroy for the global logger
 *
 * @param   log     logger_ptr      pointer to the logger
 */
extern void log_destroy(logger_ptr log);


/*
 * global_write function:
 * description:     the global logger write function
 *
 * @param   _logger logger_ptr      pointer to the logger
 * @param   level   level_t         the level of the message
 * @param   format  char*           format string
 * @param   text    char*           the message pointer
 * @param   len     int             the length of the `text` string
 */
static void global_write(logger_ptr _logger, level_t level, char *format, char *text, int len);

static void global_stdout_write(level_t level, char *text, int len);

static void global_file_write(level_t level, char *text, int len);


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


extern logger Global_File_Logger;
extern logger Global_Stdout_Logger;

#endif //STRESSOR_LOG_H