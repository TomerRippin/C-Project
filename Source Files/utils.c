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
                str[j++] = ' ';
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
int removeExtraSpacesTabsAndComments(const char *inputFilename, const char *outputFilename) {
    FILE *inputFile = fopen(inputFilename, "r");
    if (inputFile == NULL) {
        fprintf(stderr, "Error opening input file %s\n", inputFilename); /* TODO: use a general function to handel files */
        return 0;
    }

    FILE *outputFile = fopen(outputFilename, "w");
    if (outputFile == NULL) {
        fprintf(stderr, "Error creating output file %s\n", outputFilename); /* TODO: use a general function to handel files */
        fclose(inputFile);
        return 0;
    }

    char line[MAX_LINE_LEN]; /* Assuming maximum line length of MAX_LINE_LEN characters */ 
    while (fgets(line, sizeof(line), inputFile)) {
        removeExtraSpacesTabs(line);
        removeComments(line);
        fprintf(outputFile, "%s", line); /*  Write the modified line to the output file */
    }

    fclose(inputFile);
    fclose(outputFile);
    return 1;
}