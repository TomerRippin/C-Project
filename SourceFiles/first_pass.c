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
            insertToSymbolTable(symbolTable, symbol, SYMBOL_TYPE_MDEFINE, atoi(value));
            return SUCCESS;
        }
    }
}

int handleDataDirective(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryCodesTable *binaryCodesTable, int *DC)
{
    char *token = strtok(parsedLine->operands, ",");
    SymbolNode *searchResult;
    int value, handlerRetVal;

    while (token != NULL)
    {
        if (isNumber(token))
        {
            value = atoi(token);
        }
        else
        {
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
                }
            }
        }
        handlerRetVal = insertToBinaryCodesTable(binaryCodesTable, *DC, parsedLine, convertIntToBinary(value, BINARY_CODE_LEN), parsedLine->operands);
        if (handlerRetVal != SUCCESS){
            return handlerRetVal;
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
        printf("WARNING: entry line contains label: <%s>", parsedLine->label);
    }

    /* TODO: handle multiple labeles */
    /* TODO: wanted to insert NULL instead of 0 but it didnt work */
    insertToSymbolTable(symbolTable, parsedLine->operands, SYMBOL_TYPE_ENTRY, 0);
    return SUCCESS;
}

int handleCommandLine(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryCodesTable *binaryCodesTable, int *IC)
{
    int funcsRetVal, L;

    if (parsedLine->label != NULL)
    {
        logger(LOG_LEVEL_DEBUG, "label found, insert to symbol table: <%s>, type: <%s>, at location: <%d>", parsedLine->label, SYMBOL_TYPE_CODE, *IC);
        insertToSymbolTable(symbolTable, parsedLine->label, SYMBOL_TYPE_CODE, *IC);
    }
    logger(LOG_LEVEL_DEBUG, "parsing operands");
    funcsRetVal = parseOperands(parsedLine);
    if (funcsRetVal != SUCCESS)
    {
        logger(LOG_LEVEL_ERROR, "got an error in 'parseOperands': %d", funcsRetVal);
        return funcsRetVal;
    }
    printOperandsAfterParsing(parsedLine);

    funcsRetVal = handleOpcodeBinaryCode(parsedLine, binaryCodesTable, IC);
    if (funcsRetVal != SUCCESS)
    {
        logger(LOG_LEVEL_ERROR, "got an error in 'handleOpcodeBinaryCode': %d", funcsRetVal);
        return funcsRetVal;
    }

    if (parsedLine->operands != NULL)
    {
        funcsRetVal = handleOperandsBinaryCode(parsedLine, binaryCodesTable, symbolTable, *IC + 1);  /* NOTE: this will still work even if operands is null */
        if (funcsRetVal != SUCCESS)
        {
            if (funcsRetVal != ERROR_GIVEN_SYMBOL_NOT_EXIST)
            {
                logger(LOG_LEVEL_ERROR, "got an error in 'handleOperandsBinaryCode': %d", funcsRetVal);
                return funcsRetVal;
            }
            /* ERROR_GIVEN_SYMBOL_NOT_EXIST - ignore thie error and wait to second pass */
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
    int IC = BASE_INSTRUCTIONS_COUNTER; /* Insturctions Counter */
    int DC = 0;   /* Data Counter */
    char line[MAX_LINE_LEN];
    int isLabel = 0;
    int entryCount = 0;
    int externCount = 0;
    AssemblyLine parsedLine;
    int handlerRetVal;
    SymbolNode *current;

    /* TODO: maybe check if line is too long */
    while (fgets(line, sizeof(line), inputFile) != NULL)
    {
        /* Remove the newline character at the end of the line */
        line[strcspn(line, "\n")] = '\0';

        printf("#####################################\n");
        logger(LOG_LEVEL_DEBUG, "read line: %s", line);
        parsedLine = parseAssemblyLine(line);
        printAssemblyLine(&parsedLine);

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
                return ERROR_LABEL_NOT_VALID;
            }
        }

        if (strcmp(parsedLine.instruction, DEFINE_DIRECTIVE) == 0)
        {
            handlerRetVal = handleDefine(&parsedLine, symbolTable);
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
                    handlerRetVal = handleDataDirective(&parsedLine, symbolTable, binaryCodesTable, &DC);
                }
                else
                {
                    handlerRetVal = handleStringDirective(&parsedLine, symbolTable, binaryCodesTable, &DC);
                }
            }
            else if (strcmp(parsedLine.instruction, EXTERN_DIRECTIVE) == 0)
            {
                handlerRetVal = handleExternDirective(&parsedLine, symbolTable, binaryCodesTable);
                externCount += 1;
            }
            else if (strcmp(parsedLine.instruction, ENTRY_DIRECTIVE) == 0)
            {
                handlerRetVal = handleEntryDirective(&parsedLine, symbolTable, binaryCodesTable);
                entryCount += 1;
            }
            else {
                /* TODO; not supposed to come here, but just in case */
                freeAssemblyLine(&parsedLine);
                return GENERAL_ERROR;
            }

            if (handlerRetVal != SUCCESS)
            {
                freeAssemblyLine(&parsedLine);
                return handlerRetVal;
            }
        }
        
        else if (isCommandLine(&parsedLine)) {
            logger(LOG_LEVEL_INFO, "COMMAND LINE");
            handlerRetVal = handleCommandLine(&parsedLine, symbolTable, binaryCodesTable, &IC);
            if (handlerRetVal != SUCCESS)
            {
                freeAssemblyLine(&parsedLine);
                return handlerRetVal;
            }
        }

        else {
            logger(LOG_LEVEL_ERROR, "unknown instruction type");
            freeAssemblyLine(&parsedLine);
            return ERROR_UNKNOWN_INSTRUCTION;
        }
        isLabel = 0;
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

    /*freeBinaryCodesTable(binaryTableTry); */

    /* TODO - free things */
    return SUCCESS;
}
