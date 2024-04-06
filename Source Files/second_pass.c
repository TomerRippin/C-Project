#include "../Header Files/second_pass.h"

int secondPass(FILE *inputFile, char *inputFileName, LinkedList *symbolTable, int *binaryCodesTable)
{
    int IC = 0;
    char line[MAX_LINE_LEN];
    AssemblyLine parsedLine;

    /* TODO: maybe dont go over all the lines, and just go over the symbolTable + binaryCodesTable */
    while (fgets(line, sizeof(line), inputFile) != NULL) {
        /* Remove the newline character at the end of the line */
        line[strcspn(line, "\n")] = '\0';

        parsedLine = parseAssemblyLine(line);

        if (parsedLine.label) {
            continue;
            /* TODO: not sure if the meaning is to skip this line or skip the label */
        }
        else if (strcmp(parsedLine.instruction, DATA_DIRECTIVE) == 0 || 
            strcmp(parsedLine.instruction, STRING_DIRECTIVE) == 0 || 
            strcmp(parsedLine.instruction, EXTERN_DIRECTIVE) == 0) 
        {
            continue;
        }
        else if (strcmp(parsedLine.instruction, ENTRY_DIRECTIVE) == 0) {
            /* TODO: line 6 */
            continue;
        }
        else {
            /* TODO: section 7-8 */
            continue;
        }
    }

    /* Print the extern labels and their addresses to the '.externals' output file */
    /* Print the entry labels and their addresses to the '.entries' output file */
    /* Free all the allocated memory and resources used during the second pass */

    return SUCCESS;
}