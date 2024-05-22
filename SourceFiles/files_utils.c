#include "../HeaderFiles/files_utils.h"

FILE *openFile(const char *fileName, const char *mode)
{
    FILE *file = fopen(fileName, mode);
    if (!file)
    {
        logger(LOG_LEVEL_ERROR, "\x1b[1m%s (%d)\x1b[0m | Couldn't open file: `%s`", getErrorMessage(ERROR_OPEN_FILE), ERROR_OPEN_FILE, fileName);
        exit(ERROR_OPEN_FILE);
    }
    return file;
}

int isCRLF(FILE *file)
{
    int prevChar = EOF;
    int currentChar;

    while ((currentChar = fgetc(file)) != EOF)
    {
        if (currentChar == '\n')
        {
            if (prevChar == '\r')
            {
                return 1; /* CRLF detected */
            }
        }
        prevChar = currentChar;
    }
    return 0; /* LF detected */
}

int areLinesTooLong(FILE *file)
{
    char biggerLine[MAX_LINE_LEN + 100]; /* 100 is arbitrary, could be 1 */
    int isLong;

    isLong = 0;

    while ((fgets(biggerLine, sizeof(biggerLine), file) != NULL) && !isLong)
    {
        if (strlen(biggerLine) > MAX_LINE_LEN)
        {
            isLong = 1;
        }
    }
    return isLong;
}

char *replaceFileNameExt(const char *fileName, char *newExtension)
{
    char *lastPeriod, *newFileName, *temp;

    newFileName = malloc((strlen(fileName) + strlen(newExtension) + 2) * sizeof(char));  /* +2 for the dot and null terminator */
    if (newFileName == NULL)
    {
        logger(LOG_LEVEL_ERROR, "\x1b[1m%s (%d)\x1b[0m", getErrorMessage(ERROR_MEMORY_ALLOC_FAILED), ERROR_MEMORY_ALLOC_FAILED);
        exit(ERROR_MEMORY_ALLOC_FAILED);
    }
    strcpy(newFileName, fileName);

    /* Find the last occurrence of '.' in the new file name  in order to remove it */
    lastPeriod = strrchr(newFileName, '.');
    if (lastPeriod != NULL)
    {
        *lastPeriod = '\0';
    }

    /* Check if newExtension starts with a dot. If not, prepend one. */
    if (newExtension[0] != '.')
    {
        temp = malloc((strlen(newExtension) + 2) * sizeof(char)); /* +2 for the dot and null terminator */
        if (temp == NULL)
        {
            logger(LOG_LEVEL_ERROR, "\x1b[1m%s (%d) ", getErrorMessage(ERROR_MEMORY_ALLOC_FAILED), ERROR_MEMORY_ALLOC_FAILED);
            exit(ERROR_MEMORY_ALLOC_FAILED);
        }
        sprintf(temp, ".%s", newExtension);
        newExtension = temp;
    }

    /* Append the new extension to the file name */
    strcat(newFileName, newExtension);

    if (newExtension[0] != '.')
    {
        free(temp);
    }

    return newFileName;
}

void removeExtraSpacesTabs(char *str)
{
    int i, j;
    int isSpace = 1;

    for (i = 0, j = 0; str[i]; i++)
    {
        if (str[i] == ' ' || str[i] == '\t')
        {
            if (!isSpace)
            {
                str[j++] = ' '; /* Keep only one space */
                isSpace = 1;
            }
        }
        else if (str[i] == '=')
        {
            /* Remove spaces before = */
            if (j > 0 && str[j - 1] == ' ')
            {
                j--;
            }
            str[j++] = '='; /* Keep the = */
            isSpace = 1; /* Set the flag to remove subsequent spaces */
        }
        else if (str[i] == ',')
        {
            /* Remove spaces before , */
            if (j > 0 && str[j - 1] == ' ')
            {
                j--;
            }
            str[j++] = ','; /* Keep the , */
            /* Remove spaces after , */
            isSpace = 1; /* Set the flag to remove subsequent spaces */
        }
        else
        {
            str[j++] = str[i];
            isSpace = 0;
        }
    }
    str[j] = '\0';
}

void removeTrailingSpaces(char *line) {
    int found = 0;
    int len = strlen(line);
    while (len > 0 && (line[len - 2] == ' ' || line[len - 2] == '\t')) {
        line[--len] = '\0';
        found = 1;
    }
    if (found){
        line[len - 1] = '\n';
    }
    
}

int cleanAssemblyFile(FILE *inputFile, FILE *outputFile)
{
    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), inputFile))
    {
        removeExtraSpacesTabs(line);
        removeTrailingSpaces(line);
        fprintf(outputFile, "%s", line);
    }
    return SUCCESS;
}