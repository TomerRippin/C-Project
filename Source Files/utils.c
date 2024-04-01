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

/* TODO: remove enters also, and then maybe rename to removeWhitespacesAndCommentsFromFile */
int removeExtraSpacesTabsAndComments(FILE *inputFile, FILE *outputFile) {
    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), inputFile)) {
        removeExtraSpacesTabs(line);
        removeComments(line);
        fprintf(outputFile, "%s", line);
    }
    return 1;
}

/* TODO: maybe return a new string and not modify the same string */
/* TODO: remove whitespaces only if str not starts and ends with "" */
void removeSpacesAndTabsFromString(char *str)
{
    char *i = str; /* Pointer to iterate through the string */
    char *j = str; /* Pointer to track the position to insert non-space characters */

    while (*i != '\0')
    {
        if (*i != ' ' && *i != '\t')
        {
            *j = *i;
            j++;
        }
        i++;
    }

    *j = '\0';
}