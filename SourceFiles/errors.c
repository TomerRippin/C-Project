#include "../HeaderFiles/errors.h"

const char *getErrorMessage(ErrorCode error)
{
    switch (error)
    {
    case SUCCESS:
        return "Success";
    case GENERAL_ERROR:
        return "General error";
    case ERROR_MEMORY_ALLOC_FAILED:
        return "Memory allocation failed, exiting.";
    case ERROR_WRONG_ASSEMBLER_USAGE:
        return "Wrong assembler usage";
    case ERROR_FILE_NOT_FOUND:
        return "File not found";
    case ERROR_OPEN_FILE:
        return "Error opening file";
    case ERROR_UNSUPPORTED_CRLF_FORMAT:
        return "Unsupported CRLF format";
    case ERROR_PARSE_DEFINE_DIRECTIVE:
        return "Error parsing define directive";
    case ERROR_SYMBOL_ALREADY_EXIST:
        return "Symbol already exists";
    case ERROR_SYMBOL_NOT_VALID:
        return "Symbol not valid";
    case ERROR_SYMBOL_DECLARED_AS_ENTRY_AND_EXTERNAL:
        return "Symbol declared as entry and external";
    case ERROR_GIVEN_SYMBOL_NOT_EXIST:
        return "Given symbol does not exist";
    case ERROR_UNKNOWN_INSTRUCTION:
        return "Unknown instruction";
    case ERROR_UNKNOWN_OPCODE:
        return "Unknown opcode";
    case ERROR_TOO_FEW_OPERANDS_GIVEN:
        return "Too few operands given";
    case ERROR_OPERAND_IS_EMPTY:
        return "Operand is empty";
    case ERROR_OPERAND_NOT_VALID:
        return "Operand not valid";
    case ERROR_ADDRESSING_NOT_FOUND:
        return "Addressing not found";
    case ERROR_MISSING_COMMA_BETWEEN_OPERANDS:
        return "Missing comma between operands";
    case ERROR_EXTRA_COMMAS_BETWEEN_OPERANDS:
        return "Extra commas between operands";
    case ERROR_EXTRA_TEXT_AFTER_OPERAND:
        return "Extra text after operand";
    case ERROR_ADDRESSING_TYPE_NOT_MATCHING:
        return "Addressing type not matching";
    case ERROR_STRING_NOT_VALID:
        return "String not valid";
    case ERROR_REGISTER_NOT_VALID:
        return "Register not valid";
    case ERROR_ENTRY_NOT_DEFINED:
        return "Entry value is not defined";
    case ERROR_MEMORY_OVERFLOW:
        return "Exceeded memory limit";
    default:
        return "Unknown error";
    }
}

void printError(int lineNumber, ErrorCode errorCode)
{
    logger(LOG_LEVEL_ERROR, "Error in line: %d | \x1b[1m%s (%d)\x1b[0m", lineNumber, getErrorMessage(errorCode), errorCode);
}