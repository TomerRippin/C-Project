#include "../HeaderFiles/files_utils.h"

char *replaceFileNameExt(const char *fileName, char *newExtension)
{
    char *lastPeriod, *newFileName;

    newFileName = malloc((sizeof(fileName) + sizeof(newExtension)) * sizeof(char));
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
        char *temp = malloc((strlen(newExtension) + 2) * sizeof(char)); /* +2 for the dot and null terminator */
        sprintf(temp, ".%s", newExtension);
        newExtension = temp;
    }

    /* Append the new extension to the file name */
    strcat(newFileName, newExtension);

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

void removeComments(char *str)
{
    char *commentStart = strchr(str, ';');
    if (commentStart != NULL)
    {
        *commentStart = '\0'; /* Null terminate at the comment start to remove the comment */
    }
}

/* TODO: remove enters also, and then maybe rename to removeWhitespacesAndCommentsFromFile */
int cleanAssemblyFile(FILE *inputFile, FILE *outputFile)
{
    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), inputFile))
    {
        removeExtraSpacesTabs(line);
        removeComments(line);
        fprintf(outputFile, "%s", line);
    }
    return SUCCESS;
}