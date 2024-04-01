#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct AssemblyLine
{
    char *label;
    char *instruction;
    char *operands;
    int *operandsNum;
} AssemblyLine;

struct AssemblyLine parseAssemblyLine(const char *line);

void printAssemblyLine(AssemblyLine *parsedLine);

void freeAssemblyLine(AssemblyLine *line);

/* Idea - validations */
int isValidLabel(char *str);
int isValidString(char *str);
