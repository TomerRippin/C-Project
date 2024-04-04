# include <stdio.h>

/* Function to remove extra spaces and tabs from a string */
void removeExtraSpacesTabs(char *str);

/* Function to remove comments (lines starting with ';') from a string */
void removeComments(char *str);

/**
 * @brief Removes extra spaces, tabs, and spaces after commas from a string.
 *
 * This function removes extra spaces, tabs, and spaces after commas from the given string.
 * Additionally, it ensures that only one space remains if spaces are found at the beginning of the string.
 *
 * @param str The string to be modified (in-place).
 */
int removeExtraSpacesTabsAndComments(FILE *inputFile, FILE *outputFile);


/**
 * Function to count the occurrences of a specific character in a string.
 * 
 * @param str Pointer to the input string.
 * @param target The character to count occurrences of.
 * 
 * @return The number of occurrences of the target character in the string.
 */
int countOccurrences(char *str, char target);


/**
 * Function to check if a string represents a valid number.
 * 
 * @param str The input string to be checked.
 * @return 1 if the string is a number, 0 otherwise.
 * 
 */
int isNumber(const char *str);

/**
 * Function to parse the type of operand.
 * @param operand The input operand string to be parsed.
 * @return The type of operand.
 */
int parseOperandAdressing(const char *operand);