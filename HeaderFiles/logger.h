#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdarg.h>

#define DEBUG_PRINT_LEVEL 0  /* 1 - means debug prints will be printed */

typedef enum
{
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR
} LogLevel;

#define COLOR_RESET "\x1b[0m"
#define COLOR_RED "\x1b[1;31m"
#define COLOR_GREEN "\x1b[1;32m"
#define COLOR_YELLOW "\x1b[1;33m"
#define COLOR_BLUE "\x1b[1;34m"

void logger(LogLevel level, const char *format, ...);

#endif  /* LOGGER_H */
