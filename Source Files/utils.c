#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Header Files/constants.h"
#include "../Header Files/utils.h"


void removeExtraSpacesTabs(char *str) {
    int i, j;
    int isSpace = 0;
    for (i = 0, j = 0; str[i]; i++) {
        if (str[i] == ' ' || str[i] == '\t') {
            if (!isSpace) {
                if (i != 0) {  /* Check if it's the first character of the line */ 
                    str[j++] = ' ';  /* Keep only one space at the beginning */ 
                }
                isSpace = 1;
            }
        } else if (str[i] == ',') {
            str[j++] = str[i]; /*  Keep the comma */
            isSpace = 1; /* Set the flag to remove subsequent spaces */ 
        } else {
            str[j++] = str[i];
            isSpace = 0;
        }
    }
    str[j] = '\0';
}

void removeComments(char *str) {
    char *commentStart = strchr(str, ';');
    if (commentStart != NULL) {
        *commentStart = '\0'; /* Null terminate at the comment start to remove the comment */ 
    }
}

int removeExtraSpacesTabsAndComments(FILE *inputFile, FILE *outputFile) {
    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), inputFile)) {
        removeExtraSpacesTabs(line);
        removeComments(line);
        fprintf(outputFile, "%s", line);
    }
    return 1;
}