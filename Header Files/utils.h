# include <stdio.h>

/* Function to remove extra spaces and tabs from a string */
void removeExtraSpacesTabs(char *str);

/* Function to remove comments (lines starting with ';') from a string */
void removeComments(char *str);

/* Function to remove extra spaces, tabs, and comments from a file and create a modified file */
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
 * @param immediateValue Output parameter to store the immediate value if applicable.
 * @param label Output parameter to store the label if applicable.
 * @param index Output parameter to store the index if applicable.
 * @return An integer representing the type of operand (0 for immediate, 1 for straight, 2 for index, 3 for register, -1 for invalid).
 */
int parseOperandAdressing(const char *operand, int *immediateValue, char *label, int *index);