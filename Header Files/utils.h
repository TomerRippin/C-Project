# include <stdio.h>

/* Function to remove extra spaces and tabs from a string */
void removeExtraSpacesTabs(char *str);

/* Function to remove comments (lines starting with ';') from a string */
void removeComments(char *str);

/* Function to remove extra spaces, tabs, and comments from a file and create a modified file */
int removeExtraSpacesTabsAndComments(FILE *inputFile, FILE *outputFile);

void removeSpacesAndTabsFromString(char *str);