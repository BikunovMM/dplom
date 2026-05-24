#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdarg.h>

void log_err(const char *format, ...);
void log_str(const char *format, ...);

#ifdef UTILS_IMPLEMENTATION

void log_err(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    vfprintf(stderr, format, args);
    fflush(stderr);
}

void log_str(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    vfprintf(stderr, format, args);
    fflush(stdout);
}

#endif /* UTILS_IMPLEMENTATION */

#endif /* UTILS_H */