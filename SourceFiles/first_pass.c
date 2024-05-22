#include "../HeaderFiles/first_pass.h"

const char *DIRECTIVES[NUM_DIRECTIVES] = {".data", ".string", ".extern", ".entry"};

int handleDefine(AssemblyLine *parsedLine, SymbolTable *symbolTable)
{
    char *symbol = strtok(parsedLine->operands, "=");
    if (symbol == NULL)
    {
        return ERROR_PARSE_DEFINE_DIRECTIVE;
    }
    else if (searchSymbolNameInTable(symbolTable, symbol) != NULL)
    {
        return ERROR_SYMBOL_ALREADY_EXIST;
    }
    else
    {
        char *value = strtok(NULL, "=");
        if ((value == NULL) || (isNumber(value) != 1))
        {
            return ERROR_PARSE_DEFINE_DIRECTIVE;
        }
        else
        {
            insertToSymbolTable(symbolTable, symbol, SYMBOL_TYPE_MDEFINE, atoi(value));
            return SUCCESS;
        }
    }
}

int handleDataDirective(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryCodesTable *binaryCodesTable, int *DC)
{
    char *token = strtok(parsedLine->operands, ",");
    SymbolNode *searchResult;
    int value, errorCode;

    while (token != NULL)
    {
        if (isNumber(token)) {
            value = atoi(token);
        }
        else {
            if (isValidSymbol(token) != 1)
            {
                return ERROR_SYMBOL_NOT_VALID;
            }
            else
            {
                searchResult = searchSymbolNameTypeInTable(symbolTable, token, SYMBOL_TYPE_MDEFINE);
                if (searchResult == NULL)
                {
                    return ERROR_GIVEN_SYMBOL_NOT_EXIST;
                }
                else
                {
                    value = searchResult->symbolValue;
                }
            }
        }
        /* Insert the binary */
        errorCode = insertToBinaryCodesTable(binaryCodesTable, *DC, parsedLine, convertIntToBinary(value, BINARY_CODE_LEN), parsedLine->operands);
        if (errorCode != SUCCESS){
            return errorCode;
        }
        *DC = *DC + 1;

        token = strtok(NULL, ",");
    }
    return SUCCESS;
}

int handleStringDirective(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryCodesTable *binaryCodesTable, int *DC)
{
    int stringLen, i;
    char binaryCode[BINARY_CODE_LEN];
    char *intToBinaryRes;

    stringLen = strlen(parsedLine->operands);
    memset(binaryCode, '\0', sizeof(binaryCode));

    if (isValidString(parsedLine->operands) == 0) {
        return ERROR_STRING_NOT_VALID;
    }

    for (i = 1; i < stringLen -1; i++) {
        /* converts the ASCII value of the character to a binary string */
        intToBinaryRes = convertIntToBinary((int)parsedLine->operands[i], BINARY_CODE_LEN);
        strcpy(binaryCode, intToBinaryRes);
        insertToBinaryCodesTable(binaryCodesTable, *DC, parsedLine, binaryCode, &parsedLine->operands[i]);
        *DC = *DC + 1;
    }

    /* Null byte at the end of the string */
    /* Check if BINARY_CODE_LEN or BINARY_CODE_LEN -1 */
    insertToBinaryCodesTable(binaryCodesTable, *DC, parsedLine, convertIntToBinary(0, BINARY_CODE_LEN), &parsedLine->operands[i]);
    *DC = *DC + 1;

    return SUCCESS;
}

int handleExternDirective(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryCodesTable *binaryCodesTable)
{
    if (parsedLine->label != NULL) {
        logger(LOG_LEVEL_WARNING, "extern line contains label: <%s>", parsedLine->label);
    }

    /* SymbolValue is 0 doesn't have meaning */
    insertToSymbolTable(symbolTable, parsedLine->operands, SYMBOL_TYPE_EXTERNAL, 0);
    return SUCCESS;
}

int handleEntryDirective(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryCodesTable *binaryCodesTable)
{
    if (parsedLine->label != NULL) {
        logger(LOG_LEVEL_WARNING, "Entry line contains label: <%s>", parsedLine->label);
    }

    /* SymbolValue is 0 doesn't have meaning */
    insertToSymbolTable(symbolTable, parsedLine->operands, SYMBOL_TYPE_ENTRY, 0);
    return SUCCESS;
}

int handleCommandLine(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryCodesTable *binaryCodesTable, int *IC)
{
    int errorCode, L;

    if (parsedLine->label != NULL)
    {
        insertToSymbolTable(symbolTable, parsedLine->label, SYMBOL_TYPE_CODE, *IC);
    }
    errorCode = parseOperands(parsedLine);
    if (errorCode != SUCCESS)
    {
        return errorCode;
    }

    errorCode = handleOpcodeBinaryCode(parsedLine, binaryCodesTable, IC);
    if (errorCode != SUCCESS)
    {
        return errorCode;
    }

    if (parsedLine->operands != NULL)
    {
        errorCode = handleOperandsBinaryCode(parsedLine, binaryCodesTable, symbolTable, *IC + 1);  /* NOTE: this will still work even if operands is null */
        if (errorCode != SUCCESS)
        {
            if (errorCode == ERROR_GIVEN_SYMBOL_NOT_EXIST)
            {
                /* got an error in AddressType 1, ignore thie error and wait to second pass */
            }
            else
            {
                return errorCode;
            }
        }
    }

    L = calculateL(parsedLine->src->adrType, parsedLine->dst->adrType);
    *IC = *IC + L;

    return SUCCESS;
}

int firstPass(FILE *inputFile, SymbolTable *symbolTable, BinaryCodesTable *binaryCodesTable, int *IC, int *DC)
{
    char line[MAX_LINE_LEN];
    int isSymbol, entryCount, externCount, hasError, errorCode, lineNumber;
    AssemblyLine parsedLine;
    SymbolNode *current, *searchResult;
    isSymbol = entryCount = externCount = hasError = 0;
    errorCode = SUCCESS;

    /* TODO: maybe check if line is too long */
    while (fgets(line, sizeof(line), inputFile) != NULL)
    {
        lineNumber++;

        /* Remove the newline character at the end of the line */
        line[strcspn(line, "\n")] = '\0';

        logger(LOG_LEVEL_DEBUG, "%d IC: %d DC: %d read line: %s", lineNumber, *IC, *DC, line);

        if (isEmptyLine(line) || isCommentedLine(line)) {
            continue;
        }

        parsedLine = parseAssemblyLine(line);

        if (parsedLine.label != NULL)
        {
            isSymbol = 1;
            if (searchSymbolNameInTable(symbolTable, parsedLine.label) != NULL)
            {
                logger(LOG_LEVEL_WARNING, "Symbol: `%s` already exists, continue", parsedLine.label);
            }
            else if (isValidSymbol(parsedLine.label) != 1)
            {
                printError(lineNumber, ERROR_SYMBOL_NOT_VALID);
                hasError = 1;
                continue;
            }
        }

        if (strcmp(parsedLine.instruction, DEFINE_DIRECTIVE) == 0)
        {
            errorCode = handleDefine(&parsedLine, symbolTable);
        }
        else if (isDirectiveLine(&parsedLine))
        {
            if (strcmp(parsedLine.instruction, DATA_DIRECTIVE) == 0 || strcmp(parsedLine.instruction, STRING_DIRECTIVE) == 0)
            {
                if (isSymbol) {
                    searchResult = searchSymbolNameTypeInTable(symbolTable, parsedLine.label, SYMBOL_TYPE_DATA);
                    if (searchResult != NULL)
                    {
                        errorCode = ERROR_SYMBOL_ALREADY_EXIST;
                    }
                    else
                    {
                        insertToSymbolTable(symbolTable, parsedLine.label, SYMBOL_TYPE_DATA, *DC);
                    }
                }
                if (strcmp(parsedLine.instruction, DATA_DIRECTIVE) == 0)
                {
                    errorCode = handleDataDirective(&parsedLine, symbolTable, binaryCodesTable, DC);
                }
                else
                {
                    errorCode = handleStringDirective(&parsedLine, symbolTable, binaryCodesTable, DC);
                }
            }
            else if (strcmp(parsedLine.instruction, EXTERN_DIRECTIVE) == 0)
            {
                errorCode = handleExternDirective(&parsedLine, symbolTable, binaryCodesTable);
                externCount += 1;
            }
            else if (strcmp(parsedLine.instruction, ENTRY_DIRECTIVE) == 0)
            {
                errorCode = handleEntryDirective(&parsedLine, symbolTable, binaryCodesTable);
                entryCount += 1;
            }
            else {
                /* Not supposed to come here, but just in case */
                errorCode = ERROR_UNKNOWN_INSTRUCTION;
            }

            if (errorCode != SUCCESS) {
                printError(lineNumber, errorCode);
            } 
        }
        
        else if (isCommandLine(&parsedLine)) {
            errorCode = handleCommandLine(&parsedLine, symbolTable, binaryCodesTable, IC);
        }

        else {
            /* Unknown instruction type in line */
            errorCode = ERROR_UNKNOWN_INSTRUCTION;
        }
        isSymbol = 0;

        /* Log the Error of the line */
        if (errorCode != SUCCESS)
        {
            printError(lineNumber, errorCode);
            hasError = 1;
        }

        if ((*IC + *DC) > MAX_MEMORY_WORDS) {
            logger(LOG_LEVEL_ERROR, "\x1b[1m%s (%d) ", getErrorMessage(ERROR_MEMORY_OVERFLOW), ERROR_MEMORY_OVERFLOW);
            return ERROR_MEMORY_OVERFLOW;
        }
    }

    current = symbolTable->head;
    while (current != NULL)
    {
        if (strcmp(current->symbolType, SYMBOL_TYPE_DATA) == 0)
        {
            current->symbolValue = current->symbolValue + *IC;
        }
        current = current->next;
    }

    freeAssemblyLine(&parsedLine);

    return hasError;
}
