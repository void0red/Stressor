//
// Created by void0red on 5/15/19.
//

#include "log.h"

logger Global_Logger = {
        .write = write_log,
        .path = NULL,
        .fd = NULL,
        .out = TONone
};

void log_init(logger_ptr log, char *path, out_t out) {
    log->path = path;
    log->out = out;

    switch (out) {
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

void write_log(level_t level, char *text, int len) {

    if (text == NULL || len <= 0)
        return;

    char buf[len + 1];
    strncpy(buf, text, len);
    buf[len - 1] = '\0';

    switch (level) {
        case INFO:
            info_write(buf, len, Global_Logger.fd);
            break;
        case WARNING:
            warning_write(buf, len, Global_Logger.fd);
            break;
        case ERROR:
            error_write(buf, len, Global_Logger.fd);
            break;
        default:
            fprintf(Global_Logger.fd, "%s", buf);
            break;
    }
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