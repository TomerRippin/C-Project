#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"

/* Function to remove extra spaces and tabs from a string */
void removeExtraSpacesTabs(char *str);

/* Function to remove comments (lines starting with ';') from a string */
void removeComments(char *str);

/**
 * Removes extra spaces, tabs, and spaces after commas from a string.
 *
 * This function removes extra spaces, tabs, and spaces after commas from the given string.
 * Additionally, it ensures that only one space remains if spaces are found at the beginning of the string.
 *
 * @param str The string to be modified (in-place).
 */
int removeExtraSpacesTabsAndComments(FILE *inputFile, FILE *outputFile);

/* Function to check if a string represents a valid number. */
int isNumber(const char *str);

#endif /* UTILS_H */