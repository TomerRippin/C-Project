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
int isNumber(const char *str) {
    char *endptr;
    strtol(str, &endptr, 10); /* Try to convert the string to a long integer */
    return *endptr == '\0'; /*Return 1 if the conversion reaches the end of the string */
} 