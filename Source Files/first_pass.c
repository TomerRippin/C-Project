#include "../Header Files/first_pass.h"

/* TODO: rename this file */
/* TODO: delete all debug prints - starting with "DEBUG" */

const char *DIRECTIVES[NUM_DIRECTIVES] = {".data", ".string", ".extern", ".entry"};

const Opcode OPCODES[] = {
    {"mov", 2},
    {"cmp", 2},
    {"add", 2},
    {"sub", 2},
    {"lea", 2},
    {"not", 1},
    {"clr", 1},
    {"inc", 1},
    {"dec", 1},
    {"jmp", 1},
    {"bne", 1},
    {"red", 1},
    {"prn", 1},
    {"jsr", 1},
    {"rts", 0},
    {"hlt", 0}};

int handleDefine(AssemblyLine *parsedLine, LinkedList *symbolTable)
{
    printf("DEBUG - handleDefine\n");
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
        /* Check if token is a a label (not a number) and doesn't exist in symbolTable */
        /* TODO: maybe change to a function that checks if its a valid symbol */
        /* TODO: add search to the rest of the params in linked list also */
        if (isNumber(token) != 1) {
            searchResult = searchList(symbolTable, token);
            if (searchResult == NULL)
            {
                return ERROR_GIVEN_SYMBOL_NOT_EXIST;
            }
            else if (strcmp(searchResult->data, SYMBOL_TYPE_MDEFINE) != 0)
            {
                return ERROR_SYMBOL_WRONG_TYPE;
            }
            else
            {
                value = searchResult->lineNumber;
                printf("DEBUG - found a symbol: <%s> in the symbolTable, converting to value: <%d>\n", token, value);
            }
        } else {
            value = atoi(token);
        }

        printf("DEBUG - Insert to binaryCodesTable: <%d>, at location: <%d>\n", value, *DC);
        binaryCodesTable[*DC] = value;
        *DC = *DC + 1;

        token = strtok(NULL, ",");
    }
    return SUCCESS;
}

int handleStringDirective(AssemblyLine *parsedLine, LinkedList *symbolTable, int *binaryCodesTable, int *DC)
{
    printf("DEBUG - handleStringDirective\n");
    int stringLen = strlen(parsedLine->operands);
    int i;

    /* TODO: validate string - starts with "" */

    for (i = 1; i < stringLen - 1; i++) {
        printf("DEBUG - Insert to binaryCodesTable: <%d>, at location: <%d>\n", parsedLine->operands[i], *DC);
        binaryCodesTable[*DC] = parsedLine->operands[i];
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
    printf("STARTING parsing operands!123456 \n");
    printf("srctype = %d \n", srcType);
    printf("dst type = %d \n", dstType);
    int L = 1; /* you always have the base word */
    if (srcType == 4 && dstType == 4){
        return 1;
    }
    if (srcType >= 0 && srcType <= 3){
        L = L + 1;
    }
    if (dstType >= 0 && dstType <= 3){
        L = L + 1;
    }
    return L;
}

int firstPass(FILE *inputFile, LinkedList *symbolTable, int *binaryCodesTable)
{
    int IC = 100;  /* Insturctions Counter */
    int DC = 0;   /* Data Counter */
    char line[MAX_LINE_LEN];
    int isLabel = 0;
    AssemblyLine parsedLine;
    /* int operandsNum; */
    int handlerRetVal;

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
            /*
            else {
                printf("Inserting to symbol table: <%s>, type: <%s>, at location: <%d>", parsedLine.label, SYMBOL_TYPE_CODE, IC);
                insertToList(symbolTable, parsedLine.label, SYMBOL_TYPE_CODE, IC);
            }
            */
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
                    handlerRetVal = handleStringDirective(&parsedLine, symbolTable, binaryCodesTable, &DC);
                }
            }
            else if (strcmp(parsedLine.instruction, EXTERN_DIRECTIVE) == 0)
            {
                handlerRetVal = handleExternDirective(&parsedLine, symbolTable, binaryCodesTable);
            }
            else if (strcmp(parsedLine.instruction, ENTRY_DIRECTIVE) == 0)
            {
                handlerRetVal = handleEntryDirective(&parsedLine, symbolTable, binaryCodesTable);
            }
            else {
                /* TODO; not supposed to come here, but just in case */
                return GENERAL_ERROR;
            }

            if (handlerRetVal != SUCCESS)
            {
                return handlerRetVal;
            }
        }
        else {
            /*printf("DEBUG - CODE LINE - NOT HANDLED!\n");*/
            if (isLabel) {
                printf("DEBUG - label found, insert to symbol table: <%s>, type: <%s>, at location: <%d>\n", parsedLine.label, SYMBOL_TYPE_CODE, IC);
                insertToList(symbolTable, parsedLine.label, SYMBOL_TYPE_CODE, IC);
                IC++;
            }
            printf("STARTING parsing operands! \n");
            if (!parseOperands(&parsedLine)){
                printf("error");
            }
            printf("dst value = %s\n", parsedLine.dst->value);
            printf("src value = %s\n", parsedLine.src->value);
            int L = calculateL(parsedLine.src->type, parsedLine.dst->type);
            printf("############## \n L = %d \n\n", L);
            IC = IC + L;
            /*
            operandsNum = getOpcodeOperandsNum(parsedLine.instruction);
            if (operandsNum == -1)
            {
                return ERROR_OPCODE_NOT_FOUND;
            } 
            * else if (operandsNum != parsedLine.operandsNum) {
            *    return ERROR_TOO_FEW_OPERANDS_GIVEN;
            * }
            * */
            /* TODO - section 14-17 */
            isLabel = 0;
        }
        freeAssemblyLine(&parsedLine);
    }
    /* TODO - free things */
    return 0;
}