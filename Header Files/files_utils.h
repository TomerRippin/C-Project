#include "utils.h"
#include "linked_list.h"

/**
 * @brief Replaces the extension of a file name with a new extension.
 * If searches for last '.' in the file name and removes it and everything after it.
 * If there's no extension in the original file name, it appends the new extension at the end.
 *
 * @param fileName The original file name.
 * @param newExtension The new extension to replace the old one.
 * @return char* The file name with the new extension.
 */
char *replaceFileNameExt(char *fileName, char *newExtension);

int createExternalsFile(LinkedList *symbolTable, char *originalFilePath);
int createEntriesFile(LinkedList *symbolTable, char *originalFilePath);