#include "../HeaderFiles/first_pass.h"

/* TODO: rename this file */
/* TODO: delete all debug prints - starting with "DEBUG" */

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
            printf("DEBUG - Inserting to symbol table: <%s>, type: <%s>, at location: <%d>\n", symbol, SYMBOL_TYPE_MDEFINE, atoi(value));
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
            if (isValidLabel(token) != 1)
            {
                return ERROR_LABEL_NOT_VALID;
            }
            else
            {
                searchResult = searchSymbolNameInTable(symbolTable, token);
                if (searchResult == NULL)
                {
                    return ERROR_GIVEN_SYMBOL_NOT_EXIST;
                }
                /* TODO: add search to the rest of the params in linked list also */
                else if (strcmp(searchResult->symbolType, SYMBOL_TYPE_MDEFINE) != 0)
                {
                    return ERROR_SYMBOL_WRONG_TYPE;
                }
                else
                {
                    value = searchResult->symbolValue;
                    printf("DEBUG - found a symbol: <%s> in the symbolTable, converting to value: <%d>\n", token, value);
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
        return ERROR_STRING_IS_NOT_VALID;
    }

    for (i = 1; i < stringLen -1; i++) {
        /* converts the ASCII value of the character to a binary string */
        intToBinaryRes = convertIntToBinary((int)parsedLine->operands[i], BINARY_CODE_LEN);
        strcpy(binaryCode, intToBinaryRes);
        logger(LOG_LEVEL_DEBUG, "insert to binaryCodesTable: <%c>-<%s> at location: <%d>", parsedLine->operands[i], binaryCode, *DC);
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

    /* TODO: handle multiple labeles */
    logger(LOG_LEVEL_DEBUG, "Inserting to symbol table: <%s>, type: <%s>, at location: <NULL>\n", parsedLine->operands, SYMBOL_TYPE_EXTERNAL);
    /* TODO: wanted to insert NULL instead of 0 but it didnt work */
    insertToSymbolTable(symbolTable, parsedLine->operands, SYMBOL_TYPE_EXTERNAL, 0);
    return SUCCESS;
}

int handleEntryDirective(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryCodesTable *binaryCodesTable)
{
    if (parsedLine->label != NULL) {
        logger(LOG_LEVEL_WARNING, "entry line contains label: <%s>", parsedLine->label);
    }

    /* TODO: handle multiple labeles */
    logger(LOG_LEVEL_DEBUG, "Inserting to symbol table: <%s>, type: <%s>, at location: <NULL>\n", parsedLine->operands, SYMBOL_TYPE_ENTRY);
    /* TODO: wanted to insert NULL instead of 0 but it didnt work */
    insertToSymbolTable(symbolTable, parsedLine->operands, SYMBOL_TYPE_ENTRY, 0);
    return SUCCESS;
}

int handleCommandLine(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryCodesTable *binaryCodesTable, int *IC)
{
    int errorCode, L;

    if (parsedLine->label != NULL)
    {
        logger(LOG_LEVEL_DEBUG, "label found, insert to symbol table: <%s>, type: <%s>, at location: <%d>", parsedLine->label, SYMBOL_TYPE_CODE, *IC);
        insertToSymbolTable(symbolTable, parsedLine->label, SYMBOL_TYPE_CODE, *IC);
    }
    logger(LOG_LEVEL_DEBUG, "parsing operands");
    errorCode = parseOperands(parsedLine);
    if (errorCode != SUCCESS)
    {
        return errorCode;
    }
    /* printOperandsAfterParsing(parsedLine); */

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

    /* TODO: why is needed, maybe after each to binary code this updates */
    L = calculateL(parsedLine->src->adrType, parsedLine->dst->adrType);
    logger(LOG_LEVEL_DEBUG, "calculated L = %d", L);
    *IC = *IC + L;

    return SUCCESS;
}

int firstPass(FILE *inputFile, SymbolTable *symbolTable, BinaryCodesTable *binaryCodesTable)
{
    /* TODO: binaryCodesTable should hold decimalAdr and binaryMachineCode */
    int IC = BASE_INSTRUCTIONS_COUNTER; /* Insturctions Counter */
    int DC = 0;   /* Data Counter */
    char line[MAX_LINE_LEN];
    int isLabel = 0;
    int entryCount = 0;
    int externCount = 0;
    AssemblyLine parsedLine;
    int errorCode = SUCCESS;
    int hasError = 0;
    SymbolNode *current;
    int lineNumber;

    /* TODO: maybe check if line is too long */
    while (fgets(line, sizeof(line), inputFile) != NULL)
    {
        lineNumber++;
        /* Remove the newline character at the end of the line */
        line[strcspn(line, "\n")] = '\0';

        /* printf("#####################################\n"); */
        logger(LOG_LEVEL_DEBUG, "read line: %s", line);
        parsedLine = parseAssemblyLine(line);
        /* printAssemblyLine(&parsedLine); */

        if (parsedLine.label != NULL)
        {
            isLabel = 1;
            if (searchSymbolNameInTable(symbolTable, parsedLine.label) != NULL)
            {
                /* TODO think if this is legal to not do anything */
                logger(LOG_LEVEL_WARNING, "symbol already exist");
                /* return ERROR_SYMBOL_ALREADY_EXIST; */
            }
            else if (isValidLabel(parsedLine.label) != 1)
            {
                logger(LOG_LEVEL_ERROR, "Error in line %d, error code: %d", lineNumber, ERROR_LABEL_NOT_VALID);
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
            logger(LOG_LEVEL_INFO, "DIRECTIVE LINE");
            if (strcmp(parsedLine.instruction, DATA_DIRECTIVE) == 0 || strcmp(parsedLine.instruction, STRING_DIRECTIVE) == 0)
            {
                if (isLabel) {
                    logger(LOG_LEVEL_DEBUG, "label found, insert to symbol table: <%s>, type: <%s>, at location: <%d>", parsedLine.label, SYMBOL_TYPE_DATA, DC);
                    insertToSymbolTable(symbolTable, parsedLine.label, SYMBOL_TYPE_DATA, DC);
                }
                if (strcmp(parsedLine.instruction, DATA_DIRECTIVE) == 0)
                {
                    errorCode = handleDataDirective(&parsedLine, symbolTable, binaryCodesTable, &DC);
                }
                else
                {
                    errorCode = handleStringDirective(&parsedLine, symbolTable, binaryCodesTable, &DC);
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
                /* TODO; not supposed to come here, but just in case */
                freeAssemblyLine(&parsedLine);
                logger(LOG_LEVEL_ERROR, "Error while freeing the line %d", lineNumber);
            }
            if (errorCode != SUCCESS){
                logger(LOG_LEVEL_ERROR, "Error in line %d, error code: %d", lineNumber, errorCode);

            } 
        }
        
        else if (isCommandLine(&parsedLine)) {
            logger(LOG_LEVEL_INFO, "COMMAND LINE");
            errorCode = handleCommandLine(&parsedLine, symbolTable, binaryCodesTable, &IC);
        }

        else {
            logger(LOG_LEVEL_ERROR, "unknown instruction type");
            freeAssemblyLine(&parsedLine);
            errorCode = ERROR_UNKNOWN_INSTRUCTION;
        }
        isLabel = 0;

        /* Log the Error of the line */
        if (errorCode != SUCCESS)
        {
            freeAssemblyLine(&parsedLine);
            logger(LOG_LEVEL_ERROR, "Found Error in line %d, Error code: %d", lineNumber, errorCode);
            hasError = 1;
        }
    }

    current = symbolTable->head;
    while (current != NULL)
    {
        if (strcmp(current->symbolType, SYMBOL_TYPE_DATA) == 0)
        {
            current->symbolValue = current->symbolValue + IC;
        }
        current = current->next;
    }

    /* TODO - free things */
    return hasError;
}
