# include <stdio.h>

/**
 * @brief Removes extra spaces and tabs from a string.
 *
 * This function removes extra spaces and tabs from the given string.
 * Additionally, it ensures that only one space remains if spaces are found at the beginning of the string.
 *
 * @param str The string to be modified (in-place).
 */
void removeExtraSpacesTabs(char *str);

/**
 * @brief Removes comments (lines starting with ';') from a string.
 *
 * This function removes comments (lines starting with ';') from the given string.
 *
 * @param str The string to be modified (in-place).
 */
void removeComments(char *str);

/**
 * @brief Removes extra spaces, tabs, and comments from a file.
 *
 * This function reads an input file line by line, removes extra spaces, tabs, and comments from each line,
 * and then writes the modified lines to an output file.
 *
 * @param inputFile A pointer to the input file.
 * @param outputFilename A pointer to the output file.
 */
int removeExtraSpacesTabsAndComments(FILE *inputFile, FILE *outputFile);
