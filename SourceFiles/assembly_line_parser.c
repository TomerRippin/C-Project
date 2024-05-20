#include "../HeaderFiles/assembly_line_parser.h"

const Opcode OPCODES[] = {
    {0, "mov", 2},
    {1, "cmp", 2},
    {2, "add", 2},
    {3, "sub", 2},
    {4, "not", 1},
    {5, "clr", 1},
    {6, "lea", 2},
    {7, "inc", 1},
    {8, "dec", 1},
    {9, "jmp", 1},
    {10, "bne", 1},
    {11, "red", 1},
    {12, "prn", 1},
    {13, "jsr", 1},
    {14, "rts", 0},
    {15, "hlt", 0}};

const char *REGISTERS[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

struct AssemblyLine parseAssemblyLine(const char *line)
{    
    char *colonPos;
    char *spacePos;
    AssemblyLine result;
    
    result.label = NULL;
    result.instruction = NULL;
    result.operands = NULL;
    result.src = NULL;
    result.dst = NULL;

    colonPos = strchr(line, ':');
    if (colonPos != NULL)
    {
        /* --- Setting label --- */
        size_t labelLen = colonPos - line;
        result.label = (char *)malloc(labelLen + 1);
        strncpy(result.label, line, labelLen);
        result.label[labelLen] = '\0';

        /* Move the pointer past the colon and any whitespace */
        line = colonPos + 1;
        while (*line == ' ')
            line++;
    }

    /* --- Setting instruction ---*/
    spacePos = strchr(line, ' ');
    if (spacePos != NULL)
    {
        size_t instrLen = spacePos - line;
        result.instruction = (char *)malloc(instrLen + 1);
        strncpy(result.instruction, line, instrLen);
        result.instruction[instrLen] = '\0';

        /* Move the pointer past the space */
        line = spacePos + 1;
    }
    else
    {
        /* If there is no space, the entire line is the instruction and no operands, e.g: END: hlt */
        result.instruction = strdup(line);
        return result;
    }

    /* --- Setting operands --- */
    result.operands = strdup(line);

    return result;
}

void printAssemblyLine(AssemblyLine *parsedLine) {
    printf("--------------\n");
    if (parsedLine == NULL) {
        printf("AssemblyLine is NULL");
    }
    else {
        printf("Label: %s\n", parsedLine->label ? parsedLine->label : "(none)");
        printf("Instruction: %s\n", parsedLine->instruction ? parsedLine->instruction : "(none)");
        printf("Operands: %s\n", parsedLine->operands);
    }
    printf("--------------\n");
}

void printOperandsAfterParsing(AssemblyLine *parsedLine) {
    printf("--------------\n");
    if (parsedLine->src != NULL)
    {
        printf("Src Operand Adress Type: %d\n", parsedLine->src->adrType);
        printf("Src Operand Value: %s\n", parsedLine->src->value ? parsedLine->src->value : "(null)");
    }
    else
    {
        printf("Src Operand: (null)\n");
    }
    if (parsedLine->dst != NULL)
    {
        printf("Dst Operand Address Type: %d\n", parsedLine->dst->adrType);
        printf("Dst Operand Value: %s\n", parsedLine->dst->value ? parsedLine->dst->value : "(null)");
    }
    else
    {
        printf("Dst Operand: (null)\n");
    }
    printf("--------------\n");
}

void freeAssemblyLine(AssemblyLine *line)
{
    free(line->label);
    free(line->instruction);
    free(line->operands);
}

int isValidLabel(const char *label)
{
    int i;

    if (strlen(label) > MAX_LABEL_LEN)
    {
        return 0;
    }

    /* Check if the first character is an alphabetic letter */
    if (!isalpha(label[0]))
    {
        return 0;
    }

    /* Check the rest of the characters */
    for (i = 1; i < strlen(label); i++)
    {
        if (!isalnum(label[i]))
        {
            return 0;
        }
    }

    return 1;
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

int isCommandLine(AssemblyLine *parsedLine) {
    int i;
    for (i = 0; i < NUM_OPCODES; i++)
    {
        if (strcmp(parsedLine->instruction, OPCODES[i].name) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int isValidString(char *str) {
    if (str[0] != '"')
    {
        return 0;
    }
    if (str[strlen(str) - 1] != '"')
    {
        return 0;
    }
    return 1;
}

int isValidRegisterOperand(const char *operand)
{
    int i;
    for (i = 0; i < NUM_REGISTERS; i++)
    {
        if (strcmp(operand, REGISTERS[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int parseOperandAdressing(const char *operand, int *operandType)
{
    const char *labelEnd;
    char *label;
    char *index;

    if (*operand == '\0')
    {
        *operandType = -1;
        /* TODO: decide if return ERROR_OPERAND_IS_EMPTY; */
        return SUCCESS;
    }

    /* Check for immediate addressing */ 
    if (*operand == '#')
    {
        /* Check if the remaining characters are digits */ 
        if (isNumber(operand+=1))
        {
            *operandType = 0;
            return SUCCESS;
        }
        else
        {
            *operandType = 0;
            return SUCCESS;
        }
    }

    /* Check for register addressing */
    if (isValidRegisterOperand(operand))
    {
        *operandType = 3;
        return SUCCESS;
    }
    /* Check for straight addressing or index addressing */ 
    labelEnd = strchr(operand, '[');
    if (labelEnd != NULL)
    {
        const int labelLen = labelEnd - operand;
        /* Check if the label is followed by '[' and ']' */ 
        const char *indexStart = labelEnd + 1;
        const char *indexEnd = strchr(indexStart, ']');
        if (indexEnd != NULL && labelLen > 0 && indexEnd - indexStart > 0) {
            label = malloc(labelLen + 1);
            strncpy(label, operand, labelLen);
            label[labelLen] = '\0';
            index = malloc(indexEnd - indexStart + 1); /* +1 for the null-terminator */
            strncpy(index, labelEnd + 1, indexEnd - indexStart); /* +1 to exclude the '[' character */
            index[indexEnd - indexStart] = '\0';

            /* Validate label and validate index (number or a valid label) */
            if ((!isValidLabel(label)) || (!isNumber(index) && !isValidLabel(index)))
            {
                free(label);
                free(index);
                return ERROR_SYMBOL_NOT_VALID;
            }

            *operandType = 2;
            free(label);
            free(index);
            return SUCCESS;
        }
    }
    else
    {
        /* TODO: is it okay that this is here? */
        if (!isValidLabel(operand)) {
            return ERROR_SYMBOL_NOT_VALID;
        }
        *operandType = 1;
        return SUCCESS;
    }

    return ERROR_ADDRESSING_NOT_FOUND;
}

int getOpcodeCode(char *instruction) {
    int i = 1;
    if (instruction == NULL) {
        return -1;
    }

    for (i = 0; i <= NUM_OPCODES; i++) {
        if (strcmp(instruction, OPCODES[i].name) == 0) {
            return OPCODES[i].code;
        }
    }
    return -1;
}

int getOpcodeOperandsNumber(char *instruction) {
    int i = 1;
    if (instruction == NULL)
    {
        return -1;
    }

    for (i = 0; i <= NUM_OPCODES; i++)
    {
        if (strcmp(instruction, OPCODES[i].name) == 0)
        {
            return OPCODES[i].operandsNum;
        }
    }
    return -1;
}

int parseOperands(struct AssemblyLine *parsedLine)
{
    /* TODO: init memory */
    char *potSrc, *potDest;
    Operand *srcOperand, *destOperand;
    int opcodeOperandsNum, parseRetVal, opcodeCode;

    opcodeOperandsNum = getOpcodeOperandsNumber(parsedLine->instruction);
    /* Shouldn't be returned because it can only be if opcode is NULL and it checked earlier */
    if (opcodeOperandsNum == -1)
    {
        return ERROR_OPERAND_NOT_VALID;
    }

    potSrc = (char *)malloc(sizeof(char));
    potDest = (char *)malloc(sizeof(char));
    srcOperand = (Operand *)malloc(sizeof(Operand));
    destOperand = (Operand *)malloc(sizeof(Operand));

    /* if two operands */
    if (opcodeOperandsNum == 2)
    {
        int commaOccurences = countOccurrences(parsedLine->operands, ',');
        if (commaOccurences == 0)
        {
            return ERROR_MISSING_COMMA_BETWEEN_OPERANDS;
        }
        else if (commaOccurences > 1)
        {
            return ERROR_EXTRA_COMMAS_BETWEEN_OPERANDS;
        }
        else
        {
            /* Find the position of the comma and space separator */
            char *comma_pos = strstr(parsedLine->operands, ",");
            /* Copy the first operand (before the comma) */
            strncpy(potSrc, parsedLine->operands, comma_pos - parsedLine->operands);
            potSrc[(comma_pos - parsedLine->operands)] = '\0'; /* Null-terminate the string */
            /* Copy the second operand (after the comma and space) */
            strcpy(potDest, comma_pos + 1);
        }
    }

    /* if one operand */
    else if (opcodeOperandsNum == 1)
    {
        if (strchr(parsedLine->operands, ' ')) {
            return ERROR_EXTRA_TEXT_AFTER_OPERAND;
        }
        /* Only one argument, should be destination */
        strcpy(potDest, parsedLine->operands);
        /* The number of operands is 1, src operand is empty */
        potSrc = "";
    }

    /* if no operands */
    else if (opcodeOperandsNum == 0)
    {
        if (0)
        {   /* TODO: create the actual function extraText() */
            return ERROR_UNKNOWN_OPCODE;
        } 
        else {
            destOperand->adrType = -1;
            destOperand->value = '\0';
            srcOperand->adrType = -1;
            srcOperand->value = '\0';
            parsedLine->src = srcOperand;
            parsedLine->dst = destOperand;
        }
        return SUCCESS;
    }

    /* Check if potentioal arguments are correct */
    opcodeCode = getOpcodeCode(parsedLine->instruction);
    parseRetVal = parseOperandAdressing(potSrc, &(srcOperand->adrType));
    if (parseRetVal != SUCCESS)
    {
        return parseRetVal;
    }
    parseRetVal = parseOperandAdressing(potDest, &(destOperand->adrType));
    if (parseRetVal != SUCCESS)
    {
        return parseRetVal;
    }

    /*= parseOperandAdressing(potDest); */
    /* srcOperand->type = parseOperandAdressing(potSrc); */
    switch (opcodeCode)
        {
        /* MOV (0), ADD (2), SUB (3)  have dest instructions 1,2,3 and src instructions 0,1,2,3*/
        case 0: 
        case 2:
        case 3:
            if (!(destOperand->adrType >= 1 && destOperand->adrType <= 3) && (srcOperand->adrType >= 0 && srcOperand->adrType <=3 ))
            {
                return ERROR_ADDRESSING_TYPE_NOT_MATCHING;
            }
            break;
        
        /* CMP (1) has a dest instruction 0,1,2,3 and src instruction 0,1,2,3 */
        case 1:
            if (!(destOperand->adrType >= 0 && destOperand->adrType <= 3) && (srcOperand->adrType >= 0 && srcOperand->adrType <=3 ))
            {
                return ERROR_ADDRESSING_TYPE_NOT_MATCHING;
            }
            break;
        /* LEA (6) has dst instructions 1,2,3 and src instruction 1,2*/
        case 6:
            if (!(destOperand->adrType >= 1 && destOperand->adrType <= 3) && (srcOperand->adrType >= 1 && srcOperand->adrType <= 2))
            {
                return ERROR_ADDRESSING_TYPE_NOT_MATCHING;
            }
            break;
        /* NOT (4), CLR (5), INC(7), DEC(8), RED(11) have dst instruction 1, 2, 3 and no src instruction */
        case 4:
        case 5:
        case 7:
        case 8:
        case 11:
            if (!(destOperand->adrType >= 1 && destOperand->adrType <= 3))
            {
                return ERROR_ADDRESSING_TYPE_NOT_MATCHING;
            }
            break;
        /* JMP (9), BNE (10), JSR(13) have dst instruction 1,3 and no src instruction */
        case 9:
        case 10:
        case 13:
            if (!(destOperand->adrType == 1 || destOperand->adrType == 3))
            {
                return ERROR_ADDRESSING_TYPE_NOT_MATCHING;
            }
            break;

        /* PRN (12) has dst instruction 0,1,2,3 and no src instruction */
        case 12:
            if (!(destOperand->adrType >= 0 && destOperand->adrType <= 3))
            {
                return ERROR_ADDRESSING_TYPE_NOT_MATCHING;
            }
            break;
        /* RTS (14), HLT (15) have no dst and src instructions */
        case 14:
        case 15:

        /* Could not find instruction */
        case -1:
            return ERROR_UNKNOWN_OPCODE;
            break;
        default:
            break;
        }
        /* Succesfully inserted parsed operands, now save them in the parsed line */
        destOperand->value = potDest;
        srcOperand->value = potSrc;
        parsedLine->dst = destOperand;
        parsedLine->src = srcOperand;

        return SUCCESS;
}
