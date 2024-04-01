#include <stdio.h>
#include <string.h>
#include "../Header Files/errors.h"
#include "../Header Files/constants.h"
#include "../Header Files/linked_list.h"

int handleDefine(char *line, LinkedList *symbolTable);

int handleDataInstruction(char *line, LinkedList *symbolTable, int isSymbol);

int getOpcodeOperandsNum(char *opcode);

int isInstructionLine(char *line);

struct AssemblyLine
{
    char *label;
    char *instruction;
    char *operands;
};

struct AssemblyLine parseAssemblyLine(const char *line);

void printAssemblyLine(struct AssemblyLine *parsedLine);

void freeAssemblyLine(struct AssemblyLine *line);

int firstPass(FILE *inputFile, FILE *outputFile);