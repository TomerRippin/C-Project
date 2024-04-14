#include "errors.h"
#include "utils.h"
#include "linked_list.h"
#include "assembly_line_parser.h"
#include "binary_codes_table.h"

int handleDefine(AssemblyLine *parsedLine, LinkedList *symbolTable);

int handleDataDirective(AssemblyLine *parsedLine, LinkedList *symbolTable, int *binaryCodesTable, int *DC);

int handleStringDirective(AssemblyLine *parsedLine, LinkedList *symbolTable, int *binaryCodesTable, int *DC, BinaryCodesTable *binaryTableTry);

int handleExternDirective(AssemblyLine *parsedLine, LinkedList *symbolTable, int *binaryCodesTable);

/* NOTE: This function does nothing and always returns true*/
int handleEntryDirective(AssemblyLine *parsedLine, LinkedList *symbolTable, int *binaryCodesTable);

int handleCodeLine(AssemblyLine *parsedLine, LinkedList *symbolTable, int *binaryCodesTable, int *IC, BinaryCodesTable *binaryTableTry);

int firstPass(FILE *inputFile, LinkedList *symbolTable, int *binaryCodesTable);