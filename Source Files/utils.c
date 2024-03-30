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

void removeComments(char *str) {
    char *commentStart = strchr(str, ';');
    if (commentStart != NULL) {
        *commentStart = '\0'; /* Null terminate at the comment start to remove the comment */ 
    }
}

/* TODO: remove enters also, and then maybe rename to removeWhitespacesAndComments */
int removeExtraSpacesTabsAndComments(FILE *inputFile, FILE *outputFile) {
    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), inputFile)) {
        removeExtraSpacesTabs(line);
        removeComments(line);
        fprintf(outputFile, "%s", line);
    }
    return 1;
}


int countOccurrences(char *str, char target) {
    int count = 0;  /** Initialize a counter for occurrences */

    /** Iterate through the string until the end ('\0') is reached */
    while (*str != '\0') {
        /** If the current character matches the target character, increment count */
        if (*str == target) {
            count++;
        }
        /** Move to the next character in the string */
        str++;
    }

    return count;  /** Return the total count of occurrences */
}

int isNumber(const char *str) {
    char *endptr;
    strtol(str, &endptr, 10); /* Try to convert the string to a long integer */
    return *endptr == '\0'; /*Return 1 if the conversion reaches the end of the string */
} 