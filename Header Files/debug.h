#ifndef DEBUG_H
#define DEBUG_H

#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...)                  \
    do                                         \
    {                                          \
        if (DEBUG)                             \
            fprintf(stderr, fmt, __VA_ARGS__); \
    } while (0)
#else
#define DEBUG_PRINT(fmt, ...) ((void)0)
#endif

#endif
