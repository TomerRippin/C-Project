#include "../Header Files/assembly_line_parser.h"

struct AssemblyLine parseAssemblyLine(const char *line) {
    AssemblyLine result;
    result.label = NULL;
    result.instruction = NULL;
    result.operands = NULL;
    result.operandsNum = NULL;

    char *colonPos = strchr(line, ':');
    if (colonPos != NULL)
    {
        /* --- Setting label --- */
        size_t labelLen = colonPos - line;
        result.label = (char *)malloc(labelLen + 1);
        strncpy(result.label, line, labelLen);
        result.label[labelLen] = '\0';

        line = colonPos + 1;
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

    /* --- Setting operands --- */
    result.operands = (*strdup(line));

    /* --- Setting operandsNum --- */
    /* if operands contains " - there is only one operand */
    if (strlen(result.operands) >= 2 && result.operands[0] == '"' && result.operands[strlen(result.operands)] == '"')
    {
        result.operandsNum = 1;
    }
    else {
        /* Count the number of commas to determine the number of parameters */
        for (int i = 0; result.operands; i++)
        {
            if (result.operands[i] == ',')
            {
                result.operandsNum++;
            }
        }
        return result;
    }

void printAssemblyLine(struct AssemblyLine *parsedLine) {
    printf("Label: %s\n", parsedLine->label ? parsedLine->label : "(none)");
    printf("Instruction: %s\n", parsedLine->instruction ? parsedLine->instruction : "(none)");
    printf("Operands: %s\n", parsedLine->operands);
}

void freeAssemblyLine(struct AssemblyLine *line) {
    free(line->label);
    free(line->instruction);
    free(line->operands);
}