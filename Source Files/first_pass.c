#include <stdio.h>
#include <string.h>
#include "../Header Files/constants.h"
#include "../Header Files/linked_list.h"

void handleDefine(char *line, LinkedList *symbolTable)
{
    printf("handleDefine");
    /* TODO */
}

void handleInstruction(char *line, LinkedList *symbolTable)
{
    printf("handleInstruction");
    /* TODO */
}

int isInstructionLine(char *line)
{
    for (int i = 0; i < NUM_INSTRUCTIONS; i++)
    {
        if (strncmp(line, INSTRUCTIONS[i], strlen(INSTRUCTIONS[i])) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int isSymbol(char *line) {
    printf("isSymbol");
    /* TODO */
    return 0;
}

int firstPass(FILE *inputFile, FILE *outputFile)
{
    int IC = 0; /* Insturctions Counter */
    int DC = 0; /* Data Counter */
    char line[MAX_LINE_LEN];
    LinkedList *symbolTable = createList();
    int symbolLine = 0;

    while (fgets(line, sizeof(line), inputFile) != NULL)
    {
        if (strncmp(line, DEFINE_DIRECTIVE, strlen(DEFINE_DIRECTIVE)) == 0)
        {
            handleDefine(*line, symbolTable);
            continue;
        }
        if (isSymbol(*line)) {
            symbolLine = 1;
        }
        if (isInstructionLine(*line)) {
            handleInstruction(*line, symbolTable);
            continue;
        }
    }
}