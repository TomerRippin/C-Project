#include "../HeaderFiles/logger.h"

void logger(LogLevel level, const char *format, ...)
{
    va_list args;
    va_start(args, format);

    switch (level)
    {
    case LOG_LEVEL_DEBUG:
        if (DEBUG_PRINT_LEVEL) {
            printf(COLOR_BLUE "[DEBUG]: " COLOR_RESET);
            vprintf(format, args);
            printf("\n");
        }
        break;
    case LOG_LEVEL_INFO:
        printf(COLOR_GREEN "[INFO]: " COLOR_RESET);
        vprintf(format, args);
        printf("\n");
        break;
    case LOG_LEVEL_WARNING:
        printf(COLOR_YELLOW "[WARNING]: " COLOR_RESET);
        vprintf(format, args);
        printf("\n");
        break;
    case LOG_LEVEL_ERROR:
        printf(COLOR_RED "[ERROR]: " COLOR_RESET);
        vprintf(format, args);
        printf("\n");
        break;
    }

    va_end(args);
}
