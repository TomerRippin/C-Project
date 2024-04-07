#include "../Header Files/assembly_line_parser.h"

struct AssemblyLine parseAssemblyLine(const char *line) {
    AssemblyLine result;
    result.label = NULL;
    result.instruction = NULL;
    result.operands = NULL;
    /* result.operandsNum = NULL; */

    char *colonPos = strchr(line, ':');
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
    char *spacePos = strchr(line, ' ');
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

    /** --- Setting operandsNum --- 
    if operands contains " - there is only one operand *
    if (strlen(result.operands) >= 2 && result.operands[0] == '"' && result.operands[strlen(result.operands)] == '"')
    {
        result.operandsNum = (int *)1;
    }
    else if (strtok(result.operands, "=") != NULL)
    {
        result.operandsNum = (int *)1;
    }
    else {
        * Count the number of commas to determine the number of parameters *
        int i;
        for (i = 0; i < sizeof(result.operands); i++)
        {
            if (result.operands[i] == ',')
            {
                result.operandsNum++;
            }
        }
    }
    */
    return result;
}

void printAssemblyLine(AssemblyLine *parsedLine) {
    printf("--------------\n");
    printf("Label: %s\n", parsedLine->label ? parsedLine->label : "(none)");
    printf("Instruction: %s\n", parsedLine->instruction ? parsedLine->instruction : "(none)");
    printf("Operands: %s\n", parsedLine->operands);
    printf("--------------\n");
    /*printf("src type: %d, src value: %s", parsedLine->src->type, parsedLine->src->value);
    printf("dst type: %d, dst value: %s", parsedLine->dst->type, parsedLine->dst->value);*/
    /* printf("Num Operands: %d\n", *(parsedLine->operandsNum)); */
}

void freeAssemblyLine(AssemblyLine *line) {
    free(line->label);
    free(line->instruction);
    free(line->operands);
    /* free(line->operandsNum); */
}


int isAlphanumeric(const char *str) {
  if (str == NULL) {  
    return 0;
  }

  while (*str != '\0') {  
    if (!isalnum(*str)) {  
      return 0;  
    }
    str++; 
  }

  return 1; 
}

int parseOperandAdressing(const char *operand)
{
    /* Check if the operand is empty
    TODO: return an Error Code  */ 
    if (*operand == '\0') {
        return -1;
    }
    /* Check for immediate addressing */ 
    if (*operand == '#') {
        /* Check if the remaining characters are digits */ 
        if(isNumber(operand+=1))
            return 0;
        else
            return -1;
    }

    /* Check for register addressing */ 
    if (operand[0] == 'r' && isdigit(operand[1]) && operand[2] == '\0') {
        return 3;
    }
    /* Check for straight addressing or index addressing */ 
    const char *labelEnd = strchr(operand, '[');
    if (labelEnd != NULL) {
        /* Check if the label is followed by '[' and ']' */ 
        const char *indexStart = labelEnd + 1;
        const char *indexEnd = strchr(indexStart, ']');
        if (indexEnd != NULL && labelEnd - operand > 0 && indexEnd - indexStart > 0) {
            /* TODO: Check if the label contains only valid characters */ 
            /* TODO: Check if the index contains only digits its hard because it might have a define in it like LIST[sz] */ 
            return 2;
        }
    } else {
        /* Check if the operand contains only valid characters for a label */ 
        if (!isAlphanumeric(operand)) {
            return -1; /* Invalid label  TODO: return an Error Code */ 
        }
        return 1; /* Straight adressing */
    }

    return -1; /* No valid addressing type found  TODO: return an Error Code */ 
}



int countOccurrences(char *str, char target) {
    int count = 0;  /** Initialize a counter for occurrences */

    /** Iterate through the string until the end ('\0') is reached */
    while (*str != '\0') {
        /** If the current character matches the target character, increment count */
        if (*str == target) {
            count++;
        }
        /** Move to the next character in the string */
        str++;
    }
    return count;  /** Return the total count of occurrences */
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

int getInstructionOperandsNumber(char *instruction){
    printf("DEBUG - Starting parsing operand number \n");
    printf("DEBUG instruction <%s> \n", instruction);
    int i = 1;
    /* Return -1 if the string is NULL */
    if (instruction == NULL) {
        return -1;
    }

    /* Iterate through the list of known opcodes and compare the string with each opcode in the list */
    for (i = 0; i < 17; i++) { /* TODO: Create constatnt */
        if (strcmp(instruction, OPCODES[i].name) == 0) {
            return OPCODES[i].operandsNum; /* Return the index of the matching opcode */
        }
    }
    return -1; /* Return -1 if the string does not match any known opcodes */
}

int parseOperands(struct AssemblyLine *parsedLine){
    /* TODO: init memory */
    char *potSrc = (char*)malloc(sizeof(char));
    char *potDest = (char*)malloc(sizeof(char));
    Operand *srcOperand = (Operand*)malloc(sizeof(Operand));
    Operand *destOperand = (Operand*)malloc(sizeof(Operand));
    int opcodeOperandsNum = getInstructionOperandsNumber(parsedLine->instruction);
    
    /* Cannot find the operand */
    if (opcodeOperandsNum == -1){
        return 0;
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
            char *comma_pos = strstr(parsedLine->operands, ",");
            /** Copy the first operand (before the comma) */
            strncpy(potSrc, parsedLine->operands, comma_pos - parsedLine->operands);
            potSrc[(comma_pos - parsedLine->operands) + 1] = '\0'; /** Null-terminate the string */
            
            /** Copy the second operand (after the comma and space) */
            strcpy(potDest, comma_pos + 1);
            
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
            if (0) { /* TODO: create the actual function extraText() */
                return 0; /* TODO: return real ERROR CODE */
            } 
            else {
                /* TODO: think what to do with this */
                destOperand->type = -1;
                destOperand->value = '\0';
                srcOperand->type = -1;
                srcOperand->value = '\0';
                parsedLine->src = srcOperand;
                parsedLine->dst = destOperand;
            }
        /* Success, No more to check */
        return 1;
    }

    /* Check if potentioal arguments are correct */
    int instructionNumber = getInstructionNumber(parsedLine->instruction);
    destOperand->type = parseOperandAdressing(potDest);
    srcOperand->type = parseOperandAdressing(potSrc);
    switch (instructionNumber)
        {
        /* MOV (0), ADD (2), SUB (3)  have dest instructions 1,2,3 and src instructions 0,1,2,3*/
        case 0: 
        case 2:
        case 3:
            if (!(destOperand->type >= 1 && destOperand->type <= 3) && (srcOperand->type >= 0 && srcOperand->type <=3 )){
                /* TODO: Add error code on "Bad addressing type for instruction"  */
                return 0;
            }
            break;
        
        /* CMP (1) has a dest instruction 0,1,2,3 and src instruction 0,1,2,3 */
        case 1:
            if (!(destOperand->type >= 0 && destOperand->type <= 3) && (srcOperand->type >= 0 && srcOperand->type <=3 )){
                /* TODO: Add error code on "Bad addressing type for instruction"  */
                return 0;
            }
            break;
        
        /* LEA (4) has dst instructions 1,2,3 and src instruction 1,2*/
        case 4:
            if (!(destOperand->type >= 1 && destOperand->type <= 3) && (srcOperand->type >= 1 && srcOperand->type <=2)){
                /* TODO: Add error code on "Bad addressing type for instruction"  */
                return 0;
            }
            break;
        /* NOT (5), CLR (6), INC(7), DEC(8), RED(11) have dst instruction 1,2,3 and no src instruction */
        case 5:
        case 6:
        case 7:
        case 8:
        case 11:
            if (!(destOperand->type >= 1 && destOperand->type <= 3)){
                /* TODO: Add error code on "Bad addressing type for instruction"  */
                return 0;
            }
            break;
        /* JMP (9), BNE (10), JSR(13) have dst instruction 1,3 and no src instruction */
        case 9:
        case 10:
        case 13:
            if (!(destOperand->type == 1 || destOperand->type == 3)){
                /* TODO: Add error code on "Bad addressing type for instruction"  */
                return 0;
            }
            break;

        /* PRN (12) has dst instruction 0,1,2,3 and no src instruction */
        case 12:
            if (!(destOperand->type >= 0 && destOperand->type <= 3)){
                /* TODO: Add error code on "Bad addressing type for instruction"  */
                return 0;
            }
            break;
        /* RTS (14), HLT (15) have no dst and src instructions */
        case 14:
        case 15:

        /* Could not find instruction */
        case -1:
            /* TODO: Return indicative ERROR */
            return 0;
            break;
        default:
            break;
        }
        /* Succesfully inserted parsed operands, now save them in the parsed line */
        destOperand->value = potDest;
        srcOperand->value = potSrc;
        parsedLine->dst = destOperand;
        parsedLine->src = srcOperand;
        return 1;
}
