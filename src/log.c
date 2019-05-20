//
// Created by void0red on 5/15/19.
//

#include "log.h"

logger Global_File_Logger = {
        .out = TOFILE,
        .fd = NULL,
        .write = global_file_write,
        .path = NULL,
};

logger Global_Stdout_Logger = {
        .out = TOSTDOUT,
        .fd = NULL,
        .write = global_stdout_write,
        .path = NULL,
};

void log_init(logger_ptr log, char *path) {
    log->path = path;

    switch (log->out) {
        default:
        case TONone:
            break;
        case TOFILE:
            if (log->path == NULL)
                log->fd = stdout;
            else
                log->fd = fopen(path, "w+");
            break;
        case TOSTDOUT:
            log->fd = stdout;
            break;
        case TOSTDERR:
            log->fd = stderr;
            break;
    }
    setlocale(LC_ALL, "zh_CN.UTF-8");
    log->write(None, "\n\n[global logger]\n\n", 24);

}

void log_destroy(logger_ptr log) {
    if (log->out == TOFILE) {
        fclose(log->fd);
    } else {
        fflush(log->fd);
    }
}

static void get_time(char *buf, int max) {
    if (buf == NULL)
        return;
    time_t now = time(NULL);

    int len = strftime(buf, max, "%D %T", localtime(&now));
    if (len == 0) {
        buf[0] = '\0';
    }
}

inline void global_write(logger_ptr logger, level_t level, char *format, char *text, int len) {

    if (text == NULL || len <= 0)
        return;

    char buf[len + 1];
    strncpy(buf, text, len);
    buf[len] = '\0';

    switch (level) {
        case INFO:
            info_write(buf, len, logger->fd);
            break;
        case WARNING:
            warning_write(buf, len, logger->fd);
            break;
        case ERROR:
            error_write(buf, len, logger->fd);
            break;
        case FORMAT:
            fprintf(logger->fd, format, buf);
        default:
            fprintf(logger->fd, "%s", buf);
            break;
    }
}

static void global_file_write(level_t level, char *text, int len) {
    global_write(&Global_File_Logger, level, NULL, text, len);
}

static void global_stdout_write(level_t level, char *text, int len) {
    global_write(&Global_Stdout_Logger, level, NULL, text, len);
}

static void info_write(char *text, int len, FILE *out) {
    char now[64];
    get_time(now, 64);
    fprintf(out, "[INFO](%s): %s\n", now, text);
}

static void warning_write(char *text, int len, FILE *out) {
    char now[64];
    get_time(now, 64);
    fprintf(out, "[WARNING](%s): %s\n", now, text);
}

static void error_write(char *text, int len, FILE *out) {
    char now[64];
    get_time(now, 64);
    fprintf(out, "[ERROR](%s): %s\n", now, text);
}