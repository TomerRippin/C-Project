#include "errors.h"
#include "utils.h"
#include "binary_code.h"
#include "symbol_table.h"
#include "assembly_line_parser.h"
#include "binary_codes_table.h"

/* handles a line in format <label:> .define <symbol>=<value> */
int handleDefine(AssemblyLine *parsedLine, SymbolTable *symbolTable);

int handleDataDirective(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryCodesTable *binaryCodesTable, int *DC);

int handleStringDirective(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryCodesTable *binaryCodesTable, int *DC);

int handleExternDirective(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryCodesTable *binaryCodesTable);

int handleEntryDirective(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryCodesTable *binaryCodesTable);

int handleCommandLine(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryCodesTable *binaryCodesTable, int *IC);

int firstPass(FILE *inputFile, SymbolTable *symbolTable, BinaryCodesTable *binaryCodesTable);