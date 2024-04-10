#include "../Header Files/first_pass.h"

/* TODO: rename this file */
/* TODO: delete all debug prints - starting with "DEBUG" */

const char *DIRECTIVES[NUM_DIRECTIVES] = {".data", ".string", ".extern", ".entry"};

int handleDefine(AssemblyLine *parsedLine, LinkedList *symbolTable)
{
    logger(LOG_LEVEL_DEBUG, "DEBUG - handleDefine");
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
    printf("DEBUG - handleDataDirective\n");
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

int handleStringDirective(AssemblyLine *parsedLine, LinkedList *symbolTable, int *binaryCodesTable, int *DC, BinaryCodesTable *binaryTableTry, char *line)
{
    printf("DEBUG - handleStringDirective\n");
    int stringLen = strlen(parsedLine->operands);
    int i;
    char *binaryCode[BINARY_CODE_LEN];
    memset(binaryCode, '\0', BINARY_CODE_LEN * sizeof(char *));

    if (isValidString(parsedLine->operands) == 0) {
        return ERROR_STRING_IS_NOT_VALID;
    }

    /* HEREEEEE NOT WORKING */

    for (i = 1; i < stringLen - 1; i++) {
        /* converts the ASCII value of the character to a binary string */
        strcpy(binaryCode, convertIntToBinary((int)parsedLine->operands[i], BINARY_CODE_LEN - 1));
        printf("DEBUG - Insert to binaryCodesTable: <%d>, at location: <%d>\n", parsedLine->operands[i], *DC);
        binaryCodesTable[*DC] = parsedLine->operands[i];
        logger(LOG_LEVEL_DEBUG, "insert to binaryTableTry: <%s> at location: <%d>", binaryCode, DC);
        insertToBinaryCodesTable(binaryTableTry, *DC, line, binaryCode);
        *DC = *DC + 1;
    }
    return SUCCESS;
}

int handleExternDirective(AssemblyLine *parsedLine, LinkedList *symbolTable, int *binaryCodesTable) {
    printf("DEBUG - handleExternDirective\n");
    if (parsedLine->label != NULL) {
        printf("WARNING: extern line contains label: <%s>", parsedLine->label);
    }

    /* TODO: validate label */
    /* TODO: handle multiple labeles */
    printf("DEBUG - Inserting to symbol table: <%s>, type: <%s>, at location: <NULL>\n", parsedLine->operands, SYMBOL_TYPE_EXTERNAL);
    /* TODO: wanted to insert NULL instead of 0 but it didnt work */
    insertToList(symbolTable, parsedLine->operands, SYMBOL_TYPE_EXTERNAL, 0);
    return SUCCESS;
}

int handleEntryDirective(AssemblyLine *parsedLine, LinkedList *symbolTable, int *binaryCodesTable) {
    printf("DEBUG - handleDataDirective\n");
    return SUCCESS;
}

int isDirectiveLine(AssemblyLine *parsedLine)
{
    int i;
    for (i = 0; i < NUM_DIRECTIVES; i++)
    {
        if (strcmp(parsedLine->instruction, DIRECTIVES[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int calculateL(int srcType, int dstType){
    int L = 1; /* you always have the base word */
    if (srcType == 4 && dstType == 4){
        return 1;
    }
    if (srcType == 0 || srcType == 3 || srcType == 4){
        L = L + 1;
    }
    if (dstType == 0 || dstType == 3 || srcType == 4){
        L = L + 1;
    }
    if (dstType == 2){
        L = L + 2;
    }
    if (srcType == 2){
        L = L + 2;
    }
    return L;
}

int firstPass(FILE *inputFile, LinkedList *symbolTable, int *binaryCodesTable)
{
    /* TODO: binaryCodesTable should hold decimalAdr and binaryMachineCode */
    int IC = 100; /* Insturctions Counter */
    int DC = 0;   /* Data Counter */
    char line[MAX_LINE_LEN];
    int isLabel = 0;
    int L;
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
                    handlerRetVal = handleStringDirective(&parsedLine, symbolTable, binaryCodesTable, &DC, binaryTableTry, line);
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
        else {
            printf("DEBUG - CODE LINE\n");
            if (isLabel)
            {
                printf("DEBUG - label found, insert to symbol table: <%s>, type: <%s>, at location: <%d>\n", parsedLine.label, SYMBOL_TYPE_CODE, IC);
                insertToList(symbolTable, parsedLine.label, SYMBOL_TYPE_CODE, IC);
                IC++;
            }
            printf("DEBUG - parsing operands \n");
            handlerRetVal = parseOperands(&parsedLine);
            if (handlerRetVal != SUCCESS)
            {
                printf("DEBUG - Could not parse operands\n");
                freeAssemblyLine(&parsedLine);
                return handlerRetVal;
            }
            printOperandsAfterParsing(&parsedLine);
            /* TODOL insert to binary table */
            L = calculateL(parsedLine.src->adrType, parsedLine.dst->adrType);
            printf("DEBUG - L = %d\n", L);
            IC = IC + L;
        }
        isLabel = 0;
        freeAssemblyLine(&parsedLine);
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

    freeBinaryCodesTable(binaryTableTry);

    /* TODO - free things */
    return SUCCESS;
}
