#include "utils.h"

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
    vfprintf(stdout, format, args);

    fflush(stdout);
}
