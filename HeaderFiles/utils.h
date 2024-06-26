#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "constants.h"
#include "logger.h"
#include "errors.h"

/**
 * Checks whether a given string represents a valid number.
 * Tries to convert each char to a number and also checks that the number's length is less then \MAX_NUM_LEN\.
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
 * This function calculates the number L that represents how many words of information in total the instruction needs.
 * It based on the srcType and dstType of the operands.
 *
 * @param srcType The adressing type of the src Operand. 
 * @param dstType The adressing type of the dst Operand
 *
 * @return How many words of information the command needs.
 */
int calculateL(int srcType, int dstType);

/**
 * Checks if a line is empty or not (empty = containes only spaces)
 *
 * @param line A string line from a file
 *
 * @return 1 - if empty, 0 - if not
 */
int isEmptyLine(const char *line);

/**
 * Checks if a line starts with ";". if so it should be ignored.
 *
 * @param line A string line from a file
 *
 * @return 1 - if commented, 0 - if not
 */
int isCommentedLine(const char *line);

#endif /* UTILS_H */
