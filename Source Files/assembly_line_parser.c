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
    /* printf("Num Operands: %d\n", *(parsedLine->operandsNum)); */
}

void freeAssemblyLine(AssemblyLine *line) {
    free(line->label);
    free(line->instruction);
    free(line->operands);
    /* free(line->operandsNum); */
}