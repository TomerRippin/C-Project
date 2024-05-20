#ifndef ERRORS_H
#define ERRORS_H

#include "logger.h"

typedef enum ErrorCode
{
    SUCCESS = 0,
    GENERAL_ERROR = 1,
    ERROR_WRONG_ASSEMBLER_USAGE = 2,
    ERROR_FILE_NOT_FOUND = 3,
    ERROR_OPEN_FILE = 4,
    ERROR_UNSUPPORTED_CRLF_FORMAT = 5,
    ERROR_PARSE_DEFINE_DIRECTIVE = 6,
    ERROR_SYMBOL_ALREADY_EXIST = 7,
    ERROR_SYMBOL_NOT_VALID = 8,
    ERROR_SYMBOL_DECLARED_AS_ENTRY_AND_EXTERNAL = 9,
    ERROR_GIVEN_SYMBOL_NOT_EXIST = 10,
    ERROR_SYMBOL_WRONG_TYPE = 11,
    ERROR_UNKNOWN_INSTRUCTION = 12,
    ERROR_UNKNOWN_OPCODE = 13,
    ERROR_TOO_FEW_OPERANDS_GIVEN = 14,
    ERROR_OPERAND_IS_EMPTY = 15,
    ERROR_OPERAND_NOT_VALID = 16,
    ERROR_ADDRESSING_NOT_FOUND = 17,
    ERROR_MISSING_COMMA_BETWEEN_OPERANDS = 18,
    ERROR_EXTRA_COMMAS_BETWEEN_OPERANDS = 19,
    ERROR_EXTRA_TEXT_AFTER_OPERAND = 20,
    ERROR_ADDRESSING_TYPE_NOT_MATCHING = 21,
    ERROR_STRING_NOT_VALID = 22,
    ERROR_REGISTER_NOT_VALID = 23
} ErrorCode;

const char *getErrorMessage(ErrorCode error);

void printError(int lineNumber, ErrorCode errorCode);

#endif  /* ERRORS_H */