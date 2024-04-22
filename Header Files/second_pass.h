#include "files_utils.h"
#include "first_pass.h"

int secondPass(FILE *inputFile, char *inputFileName, LinkedList *symbolTable, BinaryCodesTable *binaryCodesTable);

/**
 * @brief Handler for the entries in the file.
 *
 * The function creates a new file if an entry is detected in the original file.
 * @param filename - The name to call the new filename created.
 * @param symbolTable - The symbol table that was created after the two passes.
 *
 * @return SUCCESS code or ERROR code (symbol not exist or symbol wrong type).
 */
int handleEntryFile(char *filename, LinkedList *symbolTable);