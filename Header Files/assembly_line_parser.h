#include <stdio.h>
#include <string.h>

typedef struct AssemblyLine
{
    char *label;
    char *instruction;
    char *operands;
    int *operandsNum;
} AssemblyLine;

struct AssemblyLine parseAssemblyLine(const char *line);

void printAssemblyLine(struct AssemblyLine *parsedLine);

void freeAssemblyLine(struct AssemblyLine *line);