#include "pre_assembler.h"

void extractMacrosFromFile(FILE *inputFile, LinkedList *macrosList)
{
    char line[MAX_LINE_LEN];
    char macro[MAX_MACRO_LEN] = "";  /* TODO: maybe don't assume the macro size and use realloc? */ 
    int macroSection = 0;
    char macroName[MAX_LINE_LEN] = "";

    while (fgets(line, sizeof(line), inputFile) != NULL)
    {
        if (strstr(line, MACRO_START) != NULL)
        {
            macroSection = 1;
            strcpy(macroName, line);
        }
        else if (strstr(line, MACRO_END) != NULL)
        {
            macroSection = 0;
            strcpy(macro, "");
            insert_to_list(macrosList, macroName, macro, -1);  /* TODO: decide if give line number or not */
        }
        else if (macroSection)
        {
            strcat(macro, line);
        }
    }
}

void removeMacrosFromFile(FILE *inputFile, FILE *outputFile)
{
    char line[MAX_MACRO_LEN];
    int macroSection = 0;

    while (fgets(line, sizeof(line), inputFile) != NULL)
    {
        if (strstr(line, MACRO_START) != NULL)
        {
            macroSection = 1;
            continue;
        }
        else if (strstr(line, MACRO_START) != NULL)
        {
            macroSection = 0;
            continue;
        }

        if (!macroSection)
        {
            fprintf(outputFile, "%s", line);
        }
    }
}

void replaceMacrosInFile(FILE *inputFile, FILE *outputFile)
{
    LinkedList *macrosList;
    initialize_list(macrosList);

    extractMacrosFromFile(inputFile, &macrosList);

    fseek(inputFile, 0, SEEK_SET); /* Resets the file pointer to the beginning of the file */
    removeMacrosFromFile(inputFile, outputFile);

    /* Replaces the macro names with their content */
    char line[MAX_LINE_LEN];
    fseek(inputFile, 0, SEEK_SET); /* Resets the file pointer to the beginning of the file */
    int replaced = 0;
    ListNode *result;

    while (fgets(line, sizeof(line), inputFile) != NULL)
    {
        result = searchList(macrosList, line);
        if (result != NULL) {
            /* Replaces line with macro content */
            fprintf(outputFile, "%s", result->data);
        } else {
            /* Writes original line to output file */
            fprintf(outputFile, "%s", line);
        }
}
