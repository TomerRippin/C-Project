#include "../Header Files/second_pass.h"

int secondPass(FILE *inputFile, char *inputFileName, LinkedList *symbolTable, int *binaryCodesTable)
{
    /* int IC = 0; */
    char line[MAX_LINE_LEN];
    AssemblyLine parsedLine;
    ListNode *searchResult;
    int handlerRetVal;

    /* TODO: maybe dont go over all the lines, and just go over the symbolTable + binaryCodesTable */
    while (fgets(line, sizeof(line), inputFile) != NULL)
    {
        /* Remove the newline character at the end of the line */
        line[strcspn(line, "\n")] = '\0';

        parsedLine = parseAssemblyLine(line);
        printAssemblyLine(&parsedLine);

        if (parsedLine.label) {
            /* TODO: not sure if the meaning is to skip this line or skip the label */
            continue;
        }
        else if (strcmp(parsedLine.instruction, DATA_DIRECTIVE) == 0 ||
                 strcmp(parsedLine.instruction, STRING_DIRECTIVE) == 0 ||
                 strcmp(parsedLine.instruction, EXTERN_DIRECTIVE) == 0 ||
                 strcmp(parsedLine.instruction, DEFINE_DIRECTIVE) == 0)  /* TODO: what to od with define? */
        {
            logger(LOG_LEVEL_DEBUG, "string/data/extern - do nothing");
            continue;
        }
        else if (strcmp(parsedLine.instruction, ENTRY_DIRECTIVE) == 0) {
            logger(LOG_LEVEL_DEBUG, "entry - do line 6");
            if (parsedLine.label) {
                logger(LOG_LEVEL_WARNING, "a label is declared in an entry line");
            }
            searchResult = searchList(symbolTable, parsedLine.operands);
            if (searchResult == NULL) {
                return ERROR_GIVEN_SYMBOL_NOT_EXIST;
            }
            else if (strcmp(searchResult->data, SYMBOL_TYPE_EXTERNAL) == 0)  /* TODO: test this error */
            {
                return ERROR_LABEL_DECLARED_AS_ENTRY_AND_EXTERNAL;
            }
            else {
                /* TODO: line 6 - update entry symbol */
                printf("DEBUG - line 6 - TODO");
            }
        }
        else {
            /* TODO: section 7-8 */
            printf("DEBUG - section 7-8\n");
            handlerRetVal = parseOperands(&parsedLine);
            printOperandsAfterParsing(&parsedLine);
            printf("DEBUG - %d\n", handlerRetVal);
        }
    }

    /* Print the extern labels and their addresses to the '.externals' output file */
    /* Print the entry labels and their addresses to the '.entries' output file */
    /* Free all the allocated memory and resources used during the second pass */

    return SUCCESS;
}