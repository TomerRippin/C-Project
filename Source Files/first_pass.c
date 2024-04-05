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
                printf("DEBUG - found a symbol: <%s> in the symbolTable, converting to value: <%d>", token, value);
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

int getOpcodeOperandsNum(char *opcode)
{
    int i;
    for (i = 0; i < NUM_OPCODES; i++)
    {
        if (strcmp(opcode, OPCODES[i].name) == 0)
        {
            return OPCODES[i].operandsNum;
        }
    }
    return -1;
}

int getInstructionNumber(char *instruction){
    int i = 1;
    /* Return -1 if the string is NULL */
    if (instruction == NULL) {
        return -1;
    }

    /* Iterate through the list of known opcodes and compare the string with each opcode in the list */
    for (i = 0; i < 17; i++) { /* TODO: Create constatnt */
        if (strcmp(instruction, OPCODES[i].name) == 0) {
            return i; /* Return the index of the matching opcode */
        }
    }
    return -1; /* Return -1 if the string does not match any known opcodes */
}

int parseOperands(struct AssemblyLine *parsedLine){
    char *potSrc, *potDest;
    Operand *srcOperand, *destOperand;
    int opcodeOperandsNum = getOpcodeOperandsNum(parsedLine->instruction);
    /* Cannot find the operand */
    if (opcodeOperandsNum == -1){
        return ERROR_OPCODE_NOT_FOUND;
    }

    
    /* if two operands */
    if (opcodeOperandsNum == 2){
        int commaOccurences =  countOccurrences(parsedLine->operands, ',');
            if (commaOccurences == 0) {
            /* TODO: return "Missing comma between arguments" */
            return 0;
        }
        else if (commaOccurences > 1) {
            /* TODO: return "Extra commas between arguments" */
            return 0;
        }
        else {
            /** Find the position of the comma and space separator */
            char *comma_pos = strstr(parsedLine->operands, ", ");
            /** Copy the first operand (before the comma) */
            strncpy(potSrc, parsedLine->operands, comma_pos - parsedLine->operands);
            potSrc[comma_pos - parsedLine->operands] = '\0'; /** Null-terminate the string */
            
            /** Copy the second operand (after the comma and space) */
            strcpy(potDest, comma_pos + 2);
        }
    }
    /* if one operand */
    if (opcodeOperandsNum == 1) {
        if (strchr(parsedLine->operands, ' ')) {
            /* TODO: return error "Extra text after argument" */
            return 0;
        }
        /* Only one argument, should be destenation */
        strcpy(potDest, parsedLine->operands);
    }

    /* if no operands */

    if (opcodeOperandsNum == 0) {
            if (0) { /* TODO: create the actual function */
                return 0; /* TODO: return real ERROR CODE */
            } 
            else {
                /*parsedLine->src = parsedLine->dst = NULL;/*
            }
        /* Success, No more to check */
        return 1;
    }
    destOperand->type = parseOperandAdressing(potDest);
    srcOperand->type = parseOperandAdressing(potSrc);

    /* Check if potentioal arguments are correct */
    switch (getInstructionNumber(parsedLine->instruction))
        {

        /*
        0 is #number || #defined label -> Adds 1 line with the number itself.
        1 is label -> Adds 1 line of the adress of the label
        2 is index in array -> Adds 2 lines, the first is for the adress of the array, the second is for the index inside of it.
        3 is register -> adds 1 line with the number of the register. NOTICE! if both src and dest are registers the line will contain both.
        */
        /* MOV (1), ADD (3), SUB (4)  have dest instructions 1,2,3 and src instructions 0,1,2,3*/
        
        case 1: 
        case 3:
        case 4:
            if (!(destOperand->type >= 1 && destOperand->type <= 3) && (srcOperand->type >= 0 && srcOperand->type <=3 )){
                /* TODO: Add error code on "Bad addressing type for instruction"  */
                return 0;
            }
            /* code */
            break;
        
        /* CMP (2) has a dest instruction 0,1,2,3 and src instruction 0,1,2,3 */
        case 2:
            if (!(destOperand->type >= 0 && destOperand->type <= 3) && (srcOperand->type >= 0 && srcOperand->type <=3 )){
                /* TODO: Add error code on "Bad addressing type for instruction"  */
                return 0;
            }
            break;
        
        /* LEA (5) has dst instructions 1,2,3 and src instruction 1,2*/
        case 5:
            if (!(destOperand->type >= 1 && destOperand->type <= 3) && (srcOperand->type >= 1 && srcOperand->type <=2)){
                /* TODO: Add error code on "Bad addressing type for instruction"  */
                return 0;
            }
        /* NOT (6), CLR (7), INC(8), DEC(9), RED(12) have dst instruction 1,2,3 and no src instruction */
        case 6:
        case 7:
        case 8:
        case 9:
        case 12:
            if (!(destOperand->type >= 1 && destOperand->type <= 3)){
                /* TODO: Add error code on "Bad addressing type for instruction"  */
                return 0;
            }
            break;
        /* JMP (), BNE (), JSR() have dst instruction 1,3 and no src instruction */
        case 10:
        case 11:
        case 14:
            if (!(destOperand->type == 1 || destOperand->type == 3)){
                /* TODO: Add error code on "Bad addressing type for instruction"  */
                return 0;
            }
            break;

        /* PRN (13) has dst instruction 0,1,2,3 and no src instruction */
        case 13:
            if (!(destOperand->type >= 0 && destOperand->type <= 3)){
                /* TODO: Add error code on "Bad addressing type for instruction"  */
                return 0;
            }
        /* RTS (15), HLT (16) have no dst and src instructions */
        case 15:
        case 16:

        /* Could not find instruction */
        case -1:
            /* TODO: Return indicative ERROR */
            return -1;
        default:
            break;
        }
        /* Succesfully inserted parsed operands, now save them in the parsed line */
        destOperand->value = potDest;
        srcOperand->value = potSrc;
        /*parsedLine->dst = destOperand;
        parsedLine->src = srcOperand;*/
        free(potDest);
        free(potSrc);
        return 0;
}
}


void freeOperand(Operand *operand){
    free(operand->type);
    free(operand->value);
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
            printf("DEBUG - CODE LINE - NOT HANDLED!\n");
            if (isLabel) {
                printf("DEBUG - label found, insert to symbol table: <%s>, type: <%s>, at location: <%d>\n", parsedLine.label, SYMBOL_TYPE_CODE, IC);
                insertToList(symbolTable, parsedLine.label, SYMBOL_TYPE_CODE, IC);
                IC++;
            }
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