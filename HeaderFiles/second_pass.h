#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "files_utils.h"
#include "first_pass.h"

int secondPass(FILE *inputFile, SymbolTable *symbolTable, BinaryCodesTable *binaryCodesTable);

/**
 * @brief Handler for the entries file.
 *
 * The function creates an entries file (.ent) that contains the entry symbols and their values.
 * If no entry symbols - then a file is not created.
 *
 * @param filename - The name to call the new filename created.
 * @param symbolTable - The symbol table that was created after the two passes.
 *
 * @return SUCCESS code or ERROR code (symbol not exist or symbol wrong type or error open file).
 */
int handleEntryFile(const char *filename, SymbolTable *symbolTable);

/**
 * @brief Handler for the extern file.
 *
 * The function creates an extern file (.ext) that contains the external symbols and their addresses .
 * If no external symbols - then a file is not created.
 *
 * @param filename - The name to call the new filename created.
 * @param symbolTable - The symbol table that was created after the two passes.
 *
 * @return SUCCESS code or ERROR code (symbol not exist or symbol wrong type or error open file).
 */
int handleExternFile(const char *filename, SymbolTable *symbolTable);

/**
 * @brief Creates the main output of the assembler - an object file (.ob).
 *
 * This function writes the instructions counter (IC) and data counter (DC) to the first line.
 * Then, it iterates over the binaryCodesTable, decodes each binary code and writes its address
 * and the decoded binary code to the file.
 *
 * @param filename - The name to call the new filename created.
 * @param binaryCodesTable - a BinaryCodesTable object contains all the binary codes of the original code.
 * @param IC - the instructions counter.
 * @param DC - the data counter.
 *
 * @return SUCCESS code or ERROR code (error open file).
 */
int createObjectFile(const char *filename, BinaryCodesTable *binaryCodesTable, int IC, int DC);

#endif /* SECOND_PASS_H */