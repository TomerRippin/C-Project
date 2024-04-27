#include "../../Header Files/second_pass.h"

int main()
{
    int retVal;
    printf("Starting main - test_first_pass!\n");

    LinkedList *symbolTable = createList();
    /* TODO: maybe change type to linkedList? */
    BinaryCodesTable *binaryCodesTable = createBinaryCodesTable();

    /* Open the input file */
    FILE *inputFile = fopen("./Tests/test_second_pass/test2_input_second_pass.txt", "r");
    char *fileName = "test_input_second_pass.txt";

    if (inputFile == NULL)
    {
        printf("Error opening file.\n");
        return 1;
    }

    printf("--- Testing First Pass ---\n");

    retVal = firstPass(inputFile, symbolTable, binaryCodesTable);
    if (retVal != SUCCESS)
    {
        printf("DEBUG - ERROR: %d\n", retVal);
        /* return retVal; */
    }
    else
    {
        printf("--- Finished First Pass with SUCCESS result! ---\n");
    }

    fseek(inputFile, 0, SEEK_SET); /* Resets the file pointer to the beginning of the file */

    printf("--- Testing Second Pass ---\n");

    char *extension = "ext";
    retVal = secondPass(inputFile, fileName, symbolTable, binaryCodesTable);

    if (retVal != SUCCESS) {
        printf("DEBUG - ERROR: %d\n", retVal);
    }
    else {
        printf("--- Finished Second Pass with SUCCESS result! ---\n");
    }

    printf("DEBUG - Check replaceFileNameExt\n");
    char *newFileName = replaceFileNameExt(fileName, extension);
    printf("Original Name: %s\nNew fileName: %s\n", fileName, newFileName);
    free(newFileName);

    printf("DEBUG - creating entries file\n");
    char *entryExt = "ent";
    char *entryFileName = replaceFileNameExt(fileName, entryExt);
    printList(symbolTable);
    retVal = handleEntryFile(entryFileName, symbolTable);

    printf("DEBUG - creating externs file\n");
    char *externalExt = "ext";
    char *externalFileName = replaceFileNameExt(fileName, externalExt);
    retVal = handleExternFile(externalFileName, symbolTable);

    printf("Printing symbolTable\n");
    printList(symbolTable);

    printf("Closing files\n");
    fclose(inputFile);

    printf("Freeing symbolTable\n");
    freeList(symbolTable);

    return 0;
}