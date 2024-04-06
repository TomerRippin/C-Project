#include "../Header Files/first_pass.h"
#include "../Header Files/operands_parser.h"
#include <ctype.h>



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
    printf("DEBUG - operand 123 = %s \n", operand);
    char *ch;
    /* Check if the operand is empty
    TODO: return an Error Code  */ 
    if (*operand == '\0') {
        return -1;
    }
    printf("DEBUG - operand 1234 = %s \n", operand);
    /* Check for immediate addressing */ 
    if (*operand == '#') {
        printf("DEBUG - operand 12345 = %s \n", operand);
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
    printf("DEBUG - operand 1234 = %s \n", operand);
    /* Check for straight addressing or index addressing */ 
    const char *labelEnd = strchr(operand, '[');
    printf("DEBUG - operand 12345 = %s \n", operand);
    if (labelEnd != NULL) {
        /* Check if the label is followed by '[' and ']' */ 
        const char *indexStart = labelEnd + 1;
        const char *indexEnd = strchr(indexStart, ']');
        if (indexEnd != NULL && labelEnd - operand > 0 && indexEnd - indexStart > 0) {
            /* Check if the label contains only valid characters */ 
            if (!isAlphanumeric(operand)){
                return -1;
            }
            /* Check if the index contains only digits */ 
            for (*ch = indexStart; ch < indexEnd; ch++) {
                if (!isdigit(ch)) {
                    return -1; /* Invalid index  TODO: return an Error Code */ 
                }
            }
            return 2;
        }
    } else {
        /* Check if the operand contains only valid characters for a label */ 
        printf("DEBUG - operand 123456 = %s \n", operand);
        if (!isAlphanumeric(operand)) {
            printf("DEBUG - operand 1234567 = %s \n", operand);
            return -1; /* Invalid label  TODO: return an Error Code */ 
        }
        printf("DEBUG - operand 1234569 = %s \n", operand);
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
    printf("DEBUG - Number of occurences =  %d \n", count);
    return count;  /** Return the total count of occurrences */
}

int getInstructionNumber(char *instruction){
    printf("###########################\n");
    printf("DEBUG - Starting parsing instruction number \n");
    printf("DEBUG instruction <%s> \n", instruction);
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
    char *potSrc = (char*)malloc(sizeof(char));
    char *potDest = (char*)malloc(sizeof(char));
    Operand *srcOperand = (Operand*)malloc(sizeof(Operand));
    Operand *destOperand = (Operand*)malloc(sizeof(Operand));
    printf("DEBUG - Starting parsing operands \n");
    printf("DEBUG - operands = %s \n", parsedLine->operands);
    printf("DEBUG - instruction = %s \n", parsedLine->instruction);
    int opcodeOperandsNum = getInstructionOperandsNumber(parsedLine->instruction);
    printf("DEBUG - number of operands = %d \n", opcodeOperandsNum);
    
    /* Cannot find the operand */
    if (opcodeOperandsNum == -1){
        return ERROR_OPCODE_NOT_FOUND;
    }

    /* if two operands */
    if (opcodeOperandsNum == 2){
        printf("DEBUG - 2 operands!!\n #############\n");
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
            printf("IM HERE! \n");
            /** Find the position of the comma and space separator */
            char *comma_pos = strstr(parsedLine->operands, ",");
            /** Copy the first operand (before the comma) */
            strncpy(potSrc, parsedLine->operands, comma_pos - parsedLine->operands);
            printf("IM HERE! 123s \n");
            potSrc[(comma_pos - parsedLine->operands) + 1] = '\0'; /** Null-terminate the string */
            
            /** Copy the second operand (after the comma and space) */
            strcpy(potDest, comma_pos + 1);
            printf("DEBUG - potential src = %s \n", potSrc);
            
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
    printf("DEBUG - potential dest = %s \n", potDest);

    /* if no operands */

    if (opcodeOperandsNum == 0) {
            if (0) { /* TODO: create the actual function */
                return 0; /* TODO: return real ERROR CODE */
            } 
            else {
                parsedLine->src = parsedLine->dst = NULL;
            }
        /* Success, No more to check */
        return 1;
    }
    int type = parseOperandAdressing(potDest);
    printf("DEBUG - type = %d \n", type);

    /*srcOperand->type = parseOperandAdressing(potSrc);/*
    /* Check if potentioal arguments are correct */
    int num = getInstructionNumber(parsedLine->instruction);
    destOperand->type = type;
    printf("DEBUG - dest type = %d \n", destOperand->type);
    printf("DEBUG - num = %d \n", num);
    switch (num)
        {

        /*
        0 is #number || #defined label -> Adds 1 line with the number itself.
        1 is label -> Adds 1 line of the adress of the label
        2 is index in array -> Adds 2 lines, the first is for the adress of the array, the second is for the index inside of it.
        3 is register -> adds 1 line with the number of the register. NOTICE! if both src and dest are registers the line will contain both.
        */
        /* MOV (1), ADD (3), SUB (4)  have dest instructions 1,2,3 and src instructions 0,1,2,3*/
        
        case 0: 
        case 2:
        case 3:
            if (!(destOperand->type >= 1 && destOperand->type <= 3) && (srcOperand->type >= 0 && srcOperand->type <=3 )){
                /* TODO: Add error code on "Bad addressing type for instruction"  */
                return 0;
            }
            /* code */
            break;
        
        /* CMP (2) has a dest instruction 0,1,2,3 and src instruction 0,1,2,3 */
        case 1:
            if (!(destOperand->type >= 0 && destOperand->type <= 3) && (srcOperand->type >= 0 && srcOperand->type <=3 )){
                /* TODO: Add error code on "Bad addressing type for instruction"  */
                return 0;
            }
            break;
        
        /* LEA (5) has dst instructions 1,2,3 and src instruction 1,2*/
        case 4:
            if (!(destOperand->type >= 1 && destOperand->type <= 3) && (srcOperand->type >= 1 && srcOperand->type <=2)){
                /* TODO: Add error code on "Bad addressing type for instruction"  */
                return 0;
            }
            break;
        /* NOT (6), CLR (7), INC(8), DEC(9), RED(12) have dst instruction 1,2,3 and no src instruction */
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
        /* JMP (), BNE (), JSR() have dst instruction 1,3 and no src instruction */
        case 9:
        case 10:
        case 13:
            if (!(destOperand->type == 1 || destOperand->type == 3)){
                /* TODO: Add error code on "Bad addressing type for instruction"  */
                return 0;
            }
            break;

        /* PRN (13) has dst instruction 0,1,2,3 and no src instruction */
        case 12:
            if (!(destOperand->type >= 0 && destOperand->type <= 3)){
                /* TODO: Add error code on "Bad addressing type for instruction"  */
                return 0;
            }
            break;
        /* RTS (15), HLT (16) have no dst and src instructions */
        case 14:
        case 15:

        /* Could not find instruction */
        case -1:
            /* TODO: Return indicative ERROR */
            return 0;
        default:
            break;
        }
        /* Succesfully inserted parsed operands, now save them in the parsed line */
        printf("DEBUG - finished parsing operands \n");
        destOperand->value = potDest;
        printf("DEBUG - finished parsing operands 1  \n");
        srcOperand->value = potSrc;
        printf("DEBUG - finished parsing operands 2  \n");
        parsedLine->dst = destOperand;
        printf("DEBUG - finished parsing operands 3 \n");
        parsedLine->src = srcOperand;
        printf("DEBUG - finished parsing operands 4 \n");
        free(potDest);
        free(potSrc);
        return 1;
}


void freeOperand(Operand *operand){
    free(operand->type);
    free(operand->value);
}