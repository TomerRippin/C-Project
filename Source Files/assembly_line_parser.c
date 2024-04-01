#include "../Header Files/assembly_line_parser.h"

struct AssemblyLine parseAssemblyLine(const char *line)
{
    struct AssemblyLine result;
    result.label = NULL;
    result.instruction = NULL;
    result.operands = NULL;
    result.operandsNum = NULL;

    // Find the first occurrence of ':'
    char *colonPos = strchr(line, ':');
    if (colonPos != NULL)
    {
        // Extract the label
        size_t labelLen = colonPos - line;
        result.label = (char *)malloc(labelLen + 1);
        strncpy(result.label, line, labelLen);
        result.label[labelLen] = '\0';

        // Move the pointer past the colon
        line = colonPos + 1;
    }

    // Find the instruction (e.g., .data, .string, MOV, jmp, etc.)
    char *spacePos = strchr(line, ' ');
    if (spacePos != NULL)
    {
        size_t instrLen = spacePos - line;
        result.instruction = (char *)malloc(instrLen + 1);
        strncpy(result.instruction, line, instrLen);
        result.instruction[instrLen] = '\0';

        // Move the pointer past the space
        line = spacePos + 1;
    }

    // The remaining part is the operands
    result.operands = removeSpacesAndTabsFromString(*strdup(line));

    // Count the number of commas to determine the number of parameters
    for (int i = 0; result.operands; i++)
    {
        if (result.operands[i] == ',')
        {
            result.operandsNum++;
        }
    }

    return result;
}

void printAssemblyLine(struct AssemblyLine *parsedLine)
{
    printf("Label: %s\n", parsedLine->label ? parsedLine->label : "(none)");
    printf("Instruction: %s\n", parsedLine->instruction ? parsedLine->instruction : "(none)");
    printf("Operands: %s\n", parsedLine->operands);
}

void freeAssemblyLine(struct AssemblyLine *line)
{
    free(line->label);
    free(line->instruction);
    free(line->operands);
}