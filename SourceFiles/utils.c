#include "../HeaderFiles/utils.h"

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
    if (srcType == 3 && dstType == 3){
        return L + 1;
    }
    if (srcType == 0 || srcType == 3 || srcType == 1){
        L = L + 1;
    }
    if (dstType == 0 || dstType == 3 || dstType == 1){
        L = L + 1;
    }
    if (dstType == 2){
        L = L + 2;
    }
    if (srcType == 2){
        L = L + 2;
    }
    return L;
}


int isEmptyLine(const char *line) {
    while (*line != '\0') {
        if (!isspace(*line)) {
            return 0;  /* Not empty */
        }
        line++; 
    }
    return 1;
}

int isCommentedLine(const char *line) {
    if (*line == ';'){
        return 1; /* Commented line*/
    }
    return 0;
}