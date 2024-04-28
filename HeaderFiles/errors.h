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
    ERROR_OPERAND_IS_EMPTY = 9,
    ERROR_OPERAND_NOT_VALID = 10,
    ERROR_LABEL_NOT_VALID = 11,
    ERROR_ADDRESSING_NOT_FOUND = 12,
    ERROR_MISSING_COMMA_BETWEEN_ARGUMENTS = 13,
    ERROR_EXTRA_COMMAS_BETWEEN_ARGUMENTS = 14,
    ERROR_EXTRA_TEXT_AFTER_ARGUMENT = 15,
    ERROR_ADDRESSING_TYPE_NOT_MATCHING = 16,
    ERROR_LABEL_DECLARED_AS_ENTRY_AND_EXTERNAL = 17,
    ERROR_STRING_IS_NOT_VALID = 18,
    ERROR_UNKNOWN_INSTRUCTION = 19,
    ERROR_NOT_VALID_REGISTER = 20,
    ERROR_INDEX_NOT_NUMBER_AND_NOT_DEFINED = 21,
    ERROR_INDEX_NOT_DEFINE_OR_NUMBER = 22,
    ERROR_OPEN_FILE = 23
} ErrorCode;

/* TODO: better order in ErrorCode (by logic)  */
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