#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "logger.h"

/* Removes extra spaces and tabs from a string */
void removeExtraSpacesTabs(char *str);

/* Removes comments (lines starting with ';') from a string */
void removeComments(char *str);

/**
 * @brief Removes extra spaces, tabs and comments from an input file and writes the cleaned lines to an output file.
 *
 * @param inputFile Pointer to the input file.
 * @param outputFile Pointer to the output file.
 * @return 1 after successfully processing the input file.
 */
int removeExtraSpacesTabsAndComments(FILE *inputFile, FILE *outputFile);

/**
 * Checks whether a given string represents a valid number.
 *
 * @param str The string to be checked.
 * @return 1 if the string represents a valid number, 0 otherwise.
 */
int isNumber(const char *str);

/**
 * @brief Duplicates a string.
 *
 * This function creates a new copy of the input string.
 * The returned string is allocated using malloc and should be freed by the caller.
 * NOTE: Sometimes this function is part of the standart library.
 *
 * @param source The string to be duplicated. If this parameter is NULL, the function returns NULL.
 * @return A pointer to the newly allocated string, or NULL if the allocation failed or the input was NULL.
 */
char *strdup(const char *source);

/**
 * This function counts the occurrences of a specific character in a string.
 *
 * @param str Pointer to the input string.
 * @param target The character to count occurrences of.
 *
 * @return The number of occurrences of the target character in the string.
 */
int countOccurrences(char *str, char target);

/**
 * @brief Converts an integer to a binary string representation, and returns the binary string.
 * If needed, the returned value will be padded with zeros according to len.
 *
 * @param num The integer to be converted.
 * @param len The length of the binary string representation.
 * @return A pointer to the binary string representation of the integer.
 *
 */
char *convertIntToBinary(int num, int len);

#endif /* UTILS_H */
