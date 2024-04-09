#ifndef FILES_UTILS_H
#define FILES_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

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
char *replaceFileNameExt(char *fileName, char *newExtension);

int createExternalsFile(LinkedList *symbolTable, char *originalFilePath);
int createEntriesFile(LinkedList *symbolTable, char *originalFilePath);

#endif /* FILES_UTILS_H */