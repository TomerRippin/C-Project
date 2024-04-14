#include "../Header Files/first_pass.h"

/* TODO: rename this file */
/* TODO: delete all debug prints - starting with "DEBUG" */

const char *DIRECTIVES[NUM_DIRECTIVES] = {".data", ".string", ".extern", ".entry"};

int handleDefine(AssemblyLine *parsedLine, LinkedList *symbolTable)
{
    logger(LOG_LEVEL_DEBUG, "handleDefine");
    /* handles a line in format <label:> .define <symbol>=<value> */
    char *symbol = strtok(parsedLine->operands, "=");
    if (symbol == NULL)
    {
        return ERROR_PARSE_DEFINE_DIRECTIVE;
    }
    else if (searchList(symbolTable, symbol) != NULL)
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
            insertToList(symbolTable, symbol, SYMBOL_TYPE_MDEFINE, atoi(value));
            return SUCCESS;
        }
    }
}

int handleDataDirective(AssemblyLine *parsedLine, LinkedList *symbolTable, int *binaryCodesTable, int *DC)
{
    logger(LOG_LEVEL_DEBUG, "handleDataDirective");
    char *token = strtok(parsedLine->operands, ",");
    ListNode *searchResult;
    int value;

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
                searchResult = searchList(symbolTable, token);
                if (searchResult == NULL)
                {
                    return ERROR_GIVEN_SYMBOL_NOT_EXIST;
                }
                /* TODO: add search to the rest of the params in linked list also */
                else if (strcmp(searchResult->data, SYMBOL_TYPE_MDEFINE) != 0)
                {
                    return ERROR_SYMBOL_WRONG_TYPE;
                }
                else
                {
                    value = searchResult->lineNumber;
                    printf("DEBUG - found a symbol: <%s> in the symbolTable, converting to value: <%d>\n", token, value);
                }
            }
        }
        printf("DEBUG - Insert to binaryCodesTable: <%d>, at location: <%d>\n", value, *DC);
        binaryCodesTable[*DC] = value;
        *DC = *DC + 1;

        token = strtok(NULL, ",");
    }
    return SUCCESS;
}

int handleStringDirective(AssemblyLine *parsedLine, LinkedList *symbolTable, int *binaryCodesTable, int *DC, BinaryCodesTable *binaryTableTry)
{
    logger(LOG_LEVEL_DEBUG, "handleStringDirective");
    int stringLen = strlen(parsedLine->operands);
    int i;
    char binaryCode[BINARY_CODE_LEN];
    memset(binaryCode, '\0', sizeof(binaryCode));

    if (isValidString(parsedLine->operands) == 0) {
        return ERROR_STRING_IS_NOT_VALID;
    }

    for (i = 1; i < stringLen - 1; i++) {
        /* converts the ASCII value of the character to a binary string */
        char *res = convertIntToBinary((int)parsedLine->operands[i], BINARY_CODE_LEN - 1);

        strcpy(binaryCode, res);
        printf("DEBUG - Insert to binaryCodesTable: <%d>, at location: <%d>\n", parsedLine->operands[i], *DC);
        binaryCodesTable[*DC] = parsedLine->operands[i];
        logger(LOG_LEVEL_DEBUG, "insert to binaryTableTry: <%s> at location: <%d>", binaryCode, DC);
        insertToBinaryCodesTable(binaryTableTry, *DC, parsedLine, binaryCode);
        *DC = *DC + 1;
    }
    return SUCCESS;
}

int handleExternDirective(AssemblyLine *parsedLine, LinkedList *symbolTable, int *binaryCodesTable) {
    logger(LOG_LEVEL_DEBUG, "handleExternDirective");
    if (parsedLine->label != NULL) {
        printf("WARNING: extern line contains label: <%s>", parsedLine->label);
    }

    /* TODO: handle multiple labeles */
    printf("DEBUG - Inserting to symbol table: <%s>, type: <%s>, at location: <NULL>\n", parsedLine->operands, SYMBOL_TYPE_EXTERNAL);
    /* TODO: wanted to insert NULL instead of 0 but it didnt work */
    insertToList(symbolTable, parsedLine->operands, SYMBOL_TYPE_EXTERNAL, 0);
    return SUCCESS;
}

int handleEntryDirective(AssemblyLine *parsedLine, LinkedList *symbolTable, int *binaryCodesTable) {
    logger(LOG_LEVEL_DEBUG, "handleEntryDirective");
    return SUCCESS;
}

int calculateL(int srcType, int dstType){
    int L = 1; /* you always have the base word */
    if (srcType == -1 && dstType == -1)
    {
        return L;
    }
    else if (srcType == 4 && dstType == 4){
        return L;
    }
    else if (srcType == 0 || srcType == 3 || srcType == 4){
        L = L + 1;
    }
    else if (dstType == 0 || dstType == 3 || srcType == 4){
        L = L + 1;
    }
    else if (dstType == 2){
        L = L + 2;
    }
    else if (srcType == 2){
        L = L + 2;
    }
    return L;
}

int handleCodeLine(AssemblyLine *parsedLine, LinkedList *symbolTable, int *binaryCodesTable, int *IC, BinaryCodesTable *binaryTableTry)
{
    logger(LOG_LEVEL_DEBUG, "handleCodeLine");
    int parseRetVal;
    int L;
    if (parsedLine->label != NULL)
    {
        logger(LOG_LEVEL_DEBUG, "label found, insert to symbol table: <%s>, type: <%s>, at location: <%d>", parsedLine->label, SYMBOL_TYPE_CODE, *IC);
        insertToList(symbolTable, parsedLine->label, SYMBOL_TYPE_CODE, *IC);
        *IC = *IC + 1;
    }
    logger(LOG_LEVEL_DEBUG, "parsing operands");
    parseRetVal = parseOperands(parsedLine);
    if (parseRetVal != SUCCESS)
    {
        logger(LOG_LEVEL_DEBUG, "could not parse operands, return error: %d", parseRetVal);
        return parseRetVal;
    }
    printOperandsAfterParsing(parsedLine);
    /* TODOL insert to binary table */
    L = calculateL(parsedLine->src->adrType, parsedLine->dst->adrType);
    logger(LOG_LEVEL_DEBUG, "L = %d\n", L);
    /* TODO: calc binary code */
    logger(LOG_LEVEL_DEBUG, "insert to binaryTableTry: <\0> at location: <%d>", IC);
    insertToBinaryCodesTable(binaryTableTry, *IC, parsedLine, "\0");
    IC = IC + L;
    return SUCCESS;
}

int firstPass(FILE *inputFile, LinkedList *symbolTable, int *binaryCodesTable)
{
    /* TODO: binaryCodesTable should hold decimalAdr and binaryMachineCode */
    int IC = 100; /* Insturctions Counter */
    int DC = 0;   /* Data Counter */
    char line[MAX_LINE_LEN];
    int isLabel = 0;
    int entryCount = 0;
    int externCount = 0;
    AssemblyLine parsedLine;
    /* int operandsNum; */
    int handlerRetVal;
    BinaryCodesTable *binaryTableTry = createBinaryCodesTable();

    /* TODO: maybe check if line is too long */
    while (fgets(line, sizeof(line), inputFile) != NULL)
    {
        /* Remove the newline character at the end of the line */
        line[strcspn(line, "\n")] = '\0';

        printf("#####################################\nDEBUG - read line: %s\n", line);
        parsedLine = parseAssemblyLine(line);
        printAssemblyLine(&parsedLine);

        if (parsedLine.label != NULL)
        {
            isLabel = 1;
            if (searchList(symbolTable, parsedLine.label) != NULL)
            {
                return ERROR_SYMBOL_ALREADY_EXIST;
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
            printf("DEBUG - DIRECTIVE LINE!\n");
            if (strcmp(parsedLine.instruction, DATA_DIRECTIVE) == 0 || strcmp(parsedLine.instruction, STRING_DIRECTIVE) == 0)
            {
                if (isLabel) {
                    printf("DEBUG - label found, insert to symbol table: <%s>, type: <%s>, at location: <%d>\n", parsedLine.label, SYMBOL_TYPE_DATA, DC);
                    insertToList(symbolTable, parsedLine.label, SYMBOL_TYPE_DATA, DC);
                }
                if (strcmp(parsedLine.instruction, DATA_DIRECTIVE) == 0)
                {
                    handlerRetVal = handleDataDirective(&parsedLine, symbolTable, binaryCodesTable, &DC);
                }
                else
                {
                    handlerRetVal = handleStringDirective(&parsedLine, symbolTable, binaryCodesTable, &DC, binaryTableTry);

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
        /* TODO: validate its a code line - if not, throw exception */
        else {
            logger(LOG_LEVEL_DEBUG, "CODE LINE\n");
            handlerRetVal = handleCodeLine(&parsedLine, symbolTable, binaryCodesTable, &IC, binaryTableTry);
            if (handlerRetVal != SUCCESS)
            {
                freeAssemblyLine(&parsedLine);
                return handlerRetVal;
            }
        }
        isLabel = 0;
    }

    ListNode *current = symbolTable->head;
    while (current != NULL)
    {
        if (strcmp(current->data, SYMBOL_TYPE_DATA) == 0)
        {
            current->lineNumber = current->lineNumber + IC;
        }
        current = current->next;
    }  
    printBinaryList(binaryTableTry);

    /*freeBinaryCodesTable(binaryTableTry); */

    /* TODO - free things */
    return SUCCESS;
}
