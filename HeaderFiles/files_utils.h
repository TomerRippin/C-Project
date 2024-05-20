#ifndef FILES_UTILS_H
#define FILES_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "errors.h"

/**
 * `fopen` wrapper for handling open file error effectively.
 *
 * @param fileName file path to open.
 * @param mode mode to open file (e.g: "r" for read or "w" for writing).
 * @return if succeeded - an open file object, or exiting with error code.
*/
FILE *openFile(const char *fileName, const char *mode);

/**
 * Detects if the line ending style is CRLF in a text file.
 *
 * @param file The input file to check.
 * @return 1 if CRLF (Windows) line endings are detected, 0 if LF (Unix) line endings are detected.
 */
int isCRLF(FILE *file);

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

/* Removes extra spaces in the end of the line */
void removeTrailingSpaces(char *line);

/**
 * @brief Removes extra spaces, tabs and comments from an input file and writes the cleaned lines to an output file.
 *
 * @param inputFile Pointer to the input file.
 * @param outputFile Pointer to the output file.
 *
 * @return int SUCCESS code.
 */
int cleanAssemblyFile(FILE *inputFile, FILE *outputFile);

#endif /* FILES_UTILS_H */