#include "../HeaderFiles/pre_assembler.h"

void extractMacrosFromFile(FILE *inputFile, MacrosList *macrosList)
{
    char line[MAX_LINE_LEN];
    char macroData[MAX_MACRO_LEN] = "";  /* TODO: maybe don't assume the macro size and use realloc? */ 
    int macroSection = 0;
    char macroName[MAX_LINE_LEN] = "";

    while (fgets(line, sizeof(line), inputFile) != NULL)
    {
        if (strncmp(line, MACRO_START, strlen(MACRO_START)) == 0)
        {
            macroSection = 1;
            strtok(line, " ");
            strcpy(macroName, strtok(NULL, " ")); /* Get the second word */
            /* TODO: copy the specific length of the name, without null bytes */
        }
        else if (strncmp(line, MACRO_END, strlen(MACRO_END)) == 0)
        {
            macroSection = 0;
            insertToMacrosList(macrosList, macroName, macroData);
        }
        else if (macroSection)
        {
            strcat(macroData, line);
        }
    }
}

void removeMacrosFromFile(FILE *inputFile, FILE *outputFile)
{
    char line[MAX_MACRO_LEN];
    int macroSection = 0;

    while (fgets(line, sizeof(line), inputFile) != NULL)
    {
        if (strncmp(line, MACRO_START, strlen(MACRO_START)) == 0)
        {
            macroSection = 1;
            continue;
        }
        else if (strncmp(line, MACRO_END, strlen(MACRO_END)) == 0)
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

int preAssembler(FILE *inputFile, FILE *outputFile)
{
    MacrosList *macrosList = createMacrosList();
    FILE *tempFile = tmpfile();
    char line[MAX_LINE_LEN];
    MacroNode *result;
    int index = 0;

    extractMacrosFromFile(inputFile, macrosList);

    fseek(inputFile, 0, SEEK_SET); /* Resets the file pointer to the beginning of the file */
    if (tempFile == NULL)
    {
        return ERROR_OPEN_FILE;
    }
    removeMacrosFromFile(inputFile, tempFile);

    /* Replaces the macro names with their content */
    fseek(tempFile, 0, SEEK_SET); /* Resets the file pointer to the beginning of the file */

    while (fgets(line, sizeof(line), tempFile) != NULL)
    {
        result = searchMacrosList(macrosList, line); /* TODO: now not working because null bytes... */
        if (result != NULL)
        {
            /* Replaces line with macro content */
            fprintf(outputFile, "%s", result->data);
        }
        else
        {
            /* Writes original line to output file */
            fprintf(outputFile, "%s", line);
        }
        index += 1;
    }

    fclose(tempFile);
    freeMacrosList(macrosList);
    
    return SUCCESS;
}
