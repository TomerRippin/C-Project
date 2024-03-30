#ifndef ERRORS_H
#define ERRORS_H

typedef enum ErrorCode
{
    SUCCESS = 0,
    ERROR_FILE_NOT_FOUND,
    ERROR_PARSE_DEFINE_INSTRUCTION,
    ERROR_SYMBOL_ALREADY_EXIST,
    ERROR_OPCODE_NOT_FOUND
};

/* TODO: maybe we will want for debugging, to print the errors name, maybe delete */

/**
 * typedef struct Error
{
    ErrorCode code;
    const char *message;
};

Error error = {ERROR_FILE_NOT_FOUND, "File not found"};
printf("Error: %s, Message: %s\n", get_error_name(error.code), error.message);
*/

const char *get_error_name(ErrorCode code)
{
    switch (code)
    {
    case SUCCESS:
        return "SUCCESS";
    case ERROR_FILE_NOT_FOUND:
        return "ERROR_FILE_NOT_FOUND";
    case ERROR_PARSE_DEFINE_INSTRUCTION:
        return "ERROR_PARSE_DEFINE_INSTRUCTION";
    case ERROR_SYMBOL_ALREADY_EXIST:
        return "ERROR_SYMBOL_ALREADY_EXIST";
    case ERROR_OPCODE_NOT_FOUND:
        return "OPCODE_NOT_FOUND";
    default:
        return "UNKNOWN_ERROR";
    }
}

#endif // ERRORS_H