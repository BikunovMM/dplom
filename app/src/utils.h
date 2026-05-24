#ifndef UTILS_H
#define UTILS_H

#include <cstdio>
#include <cstdarg>

#define ERRORS_LIST          \
    X(ALLOCATING_ERR, -1)    \
    X(UNKNOWN_FMT, -2)       \
    X(LIBAV_ERR, -3)         \
    X(NULL_ARGUMENT_ERR, -4)

enum Errors {
    #define X(name, val) name = val,
        ERRORS_LIST
    #undef X
};

void log_err(const char *format, ...);
void log_str(const char *format, ...);

inline const char *ERRORS_STR[] {
    #define X(name, val) #name,
        ERRORS_LIST
    #undef X
};

#endif /* UTILS_H */
