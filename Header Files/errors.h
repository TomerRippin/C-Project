#ifndef ERRORS_H
#define ERRORS_H

typedef enum ErrorCode
{
    SUCCESS = 0,
    GENERAL_ERROR = 1,
    ERROR_FILE_NOT_FOUND = 2,
    ERROR_PARSE_DEFINE_DIRECTIVE = 3,
    ERROR_SYMBOL_ALREADY_EXIST = 4,
    ERROR_GIVEN_SYMBOL_NOT_EXIST = 5,
    ERROR_SYMBOL_WRONG_TYPE = 6,
    ERROR_OPCODE_NOT_FOUND = 7,
    ERROR_TOO_FEW_OPERANDS_GIVEN = 8,
} ErrorCode;

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

/* 
const char getErrorName(ErrorCode code)
{
    switch (code)
    {
        case SUCCESS:
            return "SUCCESS";
        case GENERAL_ERROR:
            return "GENERAL_ERROR";
        case ERROR_FILE_NOT_FOUND:
            return "ERROR_FILE_NOT_FOUND";
        case ERROR_PARSE_DEFINE_DIRECTIVE:
            return "ERROR_PARSE_DEFINE_DIRECTIVE";
        case ERROR_SYMBOL_ALREADY_EXIST:
            return "ERROR_SYMBOL_ALREADY_EXIST";
        case ERROR_GIVEN_SYMBOL_NOT_EXIST:
            return "ERROR_GIVEN_SYMBOL_NOT_EXIST";
        case ERROR_SYMBOL_WRONG_TYPE:
            return "ERROR_SYMBOL_WRONG_TYPE";
        case ERROR_OPCODE_NOT_FOUND:
            return "ERROR_OPCODE_NOT_FOUND";
        default:
            return "UNKNOWN_ERROR";
    }
}
*/

#endif  /* ERRORS_H */