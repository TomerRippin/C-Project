#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "symbol_table.h"

/**
 * @brief Reads a file and saves all the macros in it to a given linked list.
 *
 * This function reads an input file line by line. When it finds a line with \MACRO_START\,
 * it starts recording lines until it encounters a line with \MACRO_END\.
 * Then, it appends the recorded lines (the macro) to the linked list.
 *
 * @param inputFile A pointer to the input file.
 * @param macrosList A pointer to the linked list where the macros will be stored.
 */
void extractMacrosFromFile(FILE *inputFile, SymbolTable *macrosList);

/**
 * @brief Reads a file, calls extractMacrosFromFile and remove all the macros from the file.
 *
 * This function reads an input file line by line. When it finds a line with \MACRO_START\,
 * it starts skipping lines until it encounters a line with \MACRO_END\.
 * Then, it writes the remaining lines (the macro) to the linked list.
 *
 * @param inputFile A pointer to the input file.
 * @param outputFile A pointer to the output file where the new content will be written.
 */
void removeMacrosFromFile(FILE *inputFile, FILE *outputFile);

/**
 * @brief Removes macros definitions from file and replaces the macro names with the macros contents.
 *
 * This function first calls extractMacrosFromFile to create a linked list of macros from the input file.
 * Then, it calls removeMacrosFromFile to remove the macro sections from the file.
 * Finally, it reads the input file and replaces lines with macro names with macro content and returns 
 * the new file.
 *
 * @param inputFile A pointer to the input file.
 * @param outputFile A pointer to the output file where the new content will be written.
 */
int replaceMacrosInFile(FILE *inputFile, FILE *outputFile);
