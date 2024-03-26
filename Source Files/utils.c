#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Header Files/constants.h"
#include "../Header Files/utils.h"

/* Function to remove extra spaces and tabs from a string */
void removeExtraSpacesTabs(char *str) {
    int i, j;
    int isSpace = 0;
    for (i = 0, j = 0; str[i]; i++) {
        if (str[i] == ' ' || str[i] == '\t') {
            if (!isSpace) {
                /* Check if it's the first character of the line */
                if (i != 0) {  
                    str[j++] = ' ';
                }
                isSpace = 1;
            }
        } else {
            str[j++] = str[i];
            isSpace = 0;
        }
    }
    str[j] = '\0';
}


/* Function to remove comments (lines starting with ';') from a string */
void removeComments(char *str) {
    char *commentStart = strchr(str, ';');
    if (commentStart != NULL) {
        *commentStart = '\0'; /* Null terminate at the comment start to remove the comment */ 
    }
}

/* Function to remove extra spaces, tabs, and comments from a file and create a modified file */
int removeExtraSpacesTabsAndComments(FILE *inputFile, FILE *outputFile) {
    char line[MAX_LINE_LEN]; /* Assuming maximum line length of MAX_LINE_LEN characters */ 
    while (fgets(line, sizeof(line), inputFile)) {
        removeExtraSpacesTabs(line);
        removeComments(line);
        fprintf(outputFile, "%s", line); /*  Write the modified line to the output file */
    }
    return 1;
}