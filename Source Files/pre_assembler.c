#include "../Header Files/pre_assembler.h"

void extractMacrosFromFile(FILE *inputFile, SymbolTable *macrosList)
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
            insertToList(macrosList, macroName, macroData, -1);
            /* TODO: decide if give line number or not */
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

/* TODO: maybe return int and many return options (different errors) */
int replaceMacrosInFile(FILE *inputFile, FILE *outputFile)
{
    SymbolTable *macrosList = createList();

    extractMacrosFromFile(inputFile, macrosList);

    fseek(inputFile, 0, SEEK_SET); /* Resets the file pointer to the beginning of the file */
    FILE *tempFile = tmpfile();
    if (tempFile == NULL)
    {
        printf("Error creating temporary file.\n");
        return -1;
    }
    removeMacrosFromFile(inputFile, tempFile);

    /* Replaces the macro names with their content */
    char line[MAX_LINE_LEN];
    fseek(tempFile, 0, SEEK_SET); /* Resets the file pointer to the beginning of the file */
    SymbolNode *result;
    int index = 0;

    while (fgets(line, sizeof(line), tempFile) != NULL)
    {
        result = searchList(macrosList, line); /* TODO: now not working because null bytes... */
        if (result != NULL) {
            /* Replaces line with macro content */
            fprintf(outputFile, "%s", result->symbolType);
        } else {
            /* Writes original line to output file */
            fprintf(outputFile, "%s", line);
        }
        index += 1;
    }

    fclose(tempFile);
    freeList(macrosList);
    free(macrosList);

    /* TODO: create a new file - .am */
    
    return 1;
}
