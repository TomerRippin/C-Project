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
            /* Remove spaces before comma */ 
            if (j > 0 && str[j - 1] == ' ') {
                j--;
            }
            str[j++] = ','; /* Keep the comma */ 
            /* Remove spaces after comma */ 
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

int isNumber(const char *str)
{
    /* TODO: check if +<number> is also accepted */
    char *endptr;
    strtol(str, &endptr, 10); /* Try to convert the string to a long integer */
    return *endptr == '\0'; /* Return 1 if the conversion reaches the end of the string */
}

char *strdup(const char *source)
{
    size_t len;
    char *duplicate;

    if (source == NULL)
    {
        return NULL;
    }

    len = strlen(source) + 1;
    duplicate = malloc(len);

    if (duplicate != NULL)
    {
        memcpy(duplicate, source, len);
    }

    return duplicate;
}

int countOccurrences(char *str, char target)
{
    int count = 0; /** Initialize a counter for occurrences */

    /** Iterate through the string until the end ('\0') is reached */
    while (*str != '\0')
    {
        /** If the current character matches the target character, increment count */
        if (*str == target)
        {
            count++;
        }
        /** Move to the next character in the string */
        str++;
    }
    return count; /** Return the total count of occurrences */
}

int calculateL(int srcType, int dstType) {
    int L = 1; /* at least the base word */
    if (srcType == -1 && dstType == -1)
    {
        return L;
    }
    else if (srcType == 4 && dstType == 4){
        return L;
    }
    else if (srcType == 0 || srcType == 3 || srcType == 4){
        L = L + 1;
    }
    else if (dstType == 0 || dstType == 3 || srcType == 4){
        L = L + 1;
    }
    else if (dstType == 2){
        L = L + 2;
    }
    else if (srcType == 2){
        L = L + 2;
    }
    return L;
}