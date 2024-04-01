#include <stdio.h>
#include <string.h>
#include "../Header Files/errors.h"
#include "../Header Files/utils.h"
#include "../Header Files/constants.h"
#include "../Header Files/linked_list.h"
#include "../Header Files/assembly_line_parser.h"

int handleDefine(char *line, LinkedList *symbolTable);

int handleDataDirective(AssemblyLine *parsedLine, LinkedList *symbolTable, char *binaryCodesTable, int *DC);

int handleStringDirective(AssemblyLine *parsedLine, LinkedList *symbolTable, char *binaryCodesTable, int *DC);

int handleExternDirective(AssemblyLine *parsedLine, LinkedList *symbolTable, char *binaryCodesTable);

/* NOTE: This function does nothing and always returns true*/
int handleEntryDirective(AssemblyLine *parsedLine, LinkedList *symbolTable, char *binaryCodesTable);

int isDirectiveLine(char *line);

int getOpcodeOperandsNum(char *opcode);

int firstPass(FILE *inputFile, FILE *outputFile);