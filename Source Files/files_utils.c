#include "../Header Files/files_utils.h"

char *replaceFileNameExt(char *fileName, char *newExtension)
{
    char *lastPeriod, *newFileName;

    newFileName = handle_malloc((sizeof(fileName) + sizeof(newExtension)) * sizeof(char));
    strcpy(newFileName, fileName);

    /* Find the last occurrence of '.' in the new file name  in order to remove it */
    lastPeriod = strrchr(newFileName, '.');
    if (lastPeriod != NULL)
    {
        *lastPeriod = '\0';
    }

    /* Append the new extension to the file name */
    strcat(newFileName, newExtension);

    return newFileName;
}

