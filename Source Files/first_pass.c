#include "../Header Files/first_pass.h"

/* TODO: rename this file */

int handleDefine(char *line, LinkedList *symbolTable)
{
    printf("handleDefine");
    char symbol[MAX_LINE_LEN];
    int value;
    if (sscanf(line, ".define %80s = %d", symbol, &(value)))
    {
        if (searchList(symbolTable, *symbol) != NULL) {
            return ERROR_SYMBOL_ALREADY_EXIST;
        } else {
            insertToList(symbolTable, symbol, SYMBOL_TYPE_MDEFINE, value);
            return SUCCESS;
        }
    }
    else {
        return ERROR_PARSE_DEFINE_INSTRUCTION;
    }
}

int handleDataDirective(AssemblyLine *parsedLine, LinkedList *symbolTable, char *binaryCodesTable, int *DC)
{
    printf("handleDataDirective");
    char *token = strtok(parsedLine->operands, ",");
    ListNode *searchResult;
    int value;

    while (token != NULL)
    {
        /* Check if token is a a label (not a number) and doesn't exist in symbolTable */
        /* TODO: maybe change to a function that checks if its a valid symbol */
        /* TODO: add search to the rest of the params in linked list also */
        removeSpacesAndTabsFromString(token);
        if (sscanf(token, "%d") != 1) {
            searchResult = searchList(symbolTable, token);
            if (searchResult == NULL) {
                /* TODO: validate the symbol is mdefine type*/
                return ERROR_GIVEN_SYMBOL_NOT_EXIST;
            } else if (searchResult->data != "mdefine") {
                return ERROR_SYMBOL_WRONG_TYPE;
            } else {
                value = searchResult->line_number;
            }
        } else {
            value = atoi(token);
        }

        DEBUG_PRINT("Inserting to codes table: <%s>, at location: <%d>", value, *DC);
        binaryCodesTable[*DC] = value;
        *DC++;

        token = strtok(NULL, ",");
    }
    return SUCCESS;
}

int handleStringDirective(AssemblyLine *parsedLine, LinkedList *symbolTable, char *binaryCodesTable, int *DC) {
    printf("handleStringDirective");
    int stringLen = strlen(parsedLine->operands);
    int i;

    /* TODO: validate string - starts with "" */

    for (i = 0; i < stringLen - 1; i++) {
        DEBUG_PRINT("Inserting to codes table: <%s>, at location: <%d>", parsedLine->operands[i], *DC);
        binaryCodesTable[*DC] = parsedLine->operands[i];
        *DC++;
    }
    return SUCCESS;
}

int handleExternDirective(AssemblyLine *parsedLine, LinkedList *symbolTable, char *binaryCodesTable) {
    printf("handleExternDirective");
    if (parsedLine->label != NULL) {
        DEBUG_PRINT("WARNING: extern line contains label: <%s>", parsedLine->label);
    }

    /* TODO: validate label */
    /* TODO: handle multiple labales */
    DEBUG_PRINT("Inserting to symbol table: <%s>, type: <%s>, at location: <NULL>", parsedLine->operands, SYMBOL_TYPE_EXTERNAL);
    insertToList(symbolTable, parsedLine->operands, SYMBOL_TYPE_EXTERNAL, NULL);
    return SUCCESS;
}

int handleEntryDirective(AssemblyLine *parsedLine, LinkedList *symbolTable, char *binaryCodesTable) {
    return SUCCESS;
}

int isDirectiveLine(char *line)
{
    for (int i = 0; i < NUM_DIRECTIVES; i++)
    {
        if (strstr(line, DIRECTIVES[i]) != NULL)
        {
            return 1;
        }
    }
    return 0;
}

int getOpcodeOperandsNum(char *opcode)
{
    for (int i = 0; i < NUM_OPCODES; i++)
    {
        if (opcode == OPCODES[i].name)
        {
            return OPCODES[i].operandsNum;
        }
    }
    return -1;
}

int firstPass(FILE *inputFile, FILE *outputFile)
{
    int IC = 100;  /* Insturctions Counter */
    int DC = 0;   /* Data Counter */
    char line[MAX_LINE_LEN];
    LinkedList *symbolTable = createList();
    int isLabel = 0;
    AssemblyLine parsedLine;
    int operandsNum;
    int handlerRetVal;
    char binaryCodesTable[MAX_LINES_NUM];

    /* TODO: maybe check if line is too long */
    while (fgets(line, sizeof(line), inputFile) != NULL)
    {
        DEBUG_PRINT("read line: %s>", line);
        parsedLine = parseAssemblyLine(line);
        if (parsedLine.label != NULL) 
        {
            isLabel = 1;
            if (searchList(symbolTable, parsedLine.label))
            {
                return ERROR_SYMBOL_ALREADY_EXIST;
            }
        }

        if (isDirectiveLine(*line))
        {
            if (parsedLine.instruction == DEFINE_DIRECTIVE)
            {
                handlerRetVal = handleDefine(*line, symbolTable);
            }

            if (parsedLine.instruction == DATA_DIRECTIVE || parsedLine.instruction == STRING_DIRECTIVE)
            {
                if (isLabel) {
                    DEBUG_PRINT("Inserting to symbol table: <%s>, type: <%s>, at location: <%d>", parsedLine.label, SYMBOL_TYPE_DATA, DC);
                    insertToList(symbolTable, parsedLine.label, SYMBOL_TYPE_DATA, DC);
                }
                if (parsedLine.instruction == DATA_DIRECTIVE)
                {
                    handlerRetVal = handleDataDirective(&parsedLine, symbolTable, *binaryCodesTable, &DC);
                }
                else
                {
                    handlerRetVal = handleStringDirective(&parsedLine, symbolTable, *binaryCodesTable, &DC);
                }
            }
            else if (parsedLine.instruction == EXTERN_DIRECTIVE) {
                handlerRetVal = handleExternDirective(&parsedLine, symbolTable, *binaryCodesTable);
            }
            else if (parsedLine.instruction == ENTRY_DIRECTIVE) {
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
            if (isLabel) {
                DEBUG_PRINT("Inserting to symbol table: <%s>, type: <%s>, at location: <%d>", parsedLine.label, SYMBOL_TYPE_CODE, IC);
                insertToList(symbolTable, parsedLine.label, SYMBOL_TYPE_CODE, IC);
            }
            operandsNum = getOpcodeOperandsNum(parsedLine.instruction);
            if (operandsNum == -1)
            {
                return ERROR_OPCODE_NOT_FOUND;
            } else if (operandsNum != parsedLine.operandsNum) {
                return ERROR_TOO_FEW_OPERANDS_GIVEN;
            }
            /* TODO - section 14-17 */
        }
        freeAssemblyLine(&parsedLine);
    }

    /* TODO - free things */
}