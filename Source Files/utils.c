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


int parseOperandAdressing(const char *operand)
{
    char *ch;
    /* Check if the operand is empty
    TODO: return an Error Code  */ 
    if (*operand == '\0') {
        return -1;
    }

    /* Check for immediate addressing */ 
    if (*operand == '#') {
        /* Check if the remaining characters are digits */ 
        while (*(++operand) != '\0') {
            if (!isdigit(*operand)) {
                return -1; /* Invalid immediate address TODO: return an Error Code */ 
            }
        }
        return 0;
    }

    /* Check for register addressing */ 
    if (operand[0] == 'r' && isdigit(operand[1]) && operand[2] == '\0') {
        return 3;
    }

    /* Check for straight addressing or index addressing */ 
    const char *labelEnd = strchr(operand, '[');
    if (labelEnd != NULL) {
        /* Check if the label is followed by '[' and ']' */ 
        const char *indexStart = labelEnd + 1;
        const char *indexEnd = strchr(indexStart, ']');
        if (indexEnd != NULL && labelEnd - operand > 0 && indexEnd - indexStart > 0) {
            /* Check if the label contains only valid characters */ 
            for (*ch = operand; ch < labelEnd; ch++) {
                if (!isalnum(*ch) && *ch != '_') {
                    return -1; /* Invalid label  TODO: return an Error Code*/ 
                }
            }
            /* Check if the index contains only digits */ 
            for (*ch = indexStart; ch < indexEnd; ch++) {
                if (!isdigit(*ch)) {
                    return -1; /* Invalid index  TODO: return an Error Code */ 
                }
            }
            return 2;
        }
    } else {
        /* Check if the operand contains only valid characters for a label */ 
        for (*ch = operand; *ch != '\0'; ch++) {
            if (!isalnum(*ch) && *ch != '_') {
                return -1; /* Invalid label  TODO: return an Error Code */ 
            }
        }
        return 1; /* Straight adressing */
    }

    return -1; /* No valid addressing type found  TODO: return an Error Code */ 
}

int isNumber(const char *str)
{
    char *endptr;
    strtol(str, &endptr, 10); /* Try to convert the string to a long integer */
    return *endptr == '\0';   /*Return 1 if the conversion reaches the end of the string */
}