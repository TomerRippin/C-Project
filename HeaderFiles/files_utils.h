#ifndef FILES_UTILS_H
#define FILES_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "errors.h"


/**
 * @brief Replaces the extension of a file name with a new extension.
 * If searches for last '.' in the file name and removes it and everything after it.
 * If there's no extension in the original file name, it appends the new extension at the end.
 * If the extension dosn't starts with a '.' - it adds a '.' to the extension.
 *
 * @param fileName The original file name.
 * @param newExtension The new extension to replace the old one.
 * @return char* The file name with the new extension.
 */
char *replaceFileNameExt(const char *fileName, char *newExtension);

/* Removes extra spaces and tabs from a string */
void removeExtraSpacesTabs(char *str);

/* Removes comments (lines starting with ';') from a string */
void removeComments(char *str);

/**
 * @brief Removes extra spaces, tabs and comments from an input file and writes the cleaned lines to an output file.
 *
 * @param inputFile Pointer to the input file.
 * @param outputFile Pointer to the output file.
 *
 * @return int SUCCESS code.
 * TODO: check alll edge cases + maybe return error code?
 */
int cleanAssemblyFile(FILE *inputFile, FILE *outputFile);

#endif /* FILES_UTILS_H */