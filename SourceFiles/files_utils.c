#include "../HeaderFiles/files_utils.h"

char *replaceFileNameExt(char *fileName, char *newExtension)
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

