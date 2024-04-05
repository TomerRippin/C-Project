#include "../Header Files/first_pass.h"

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