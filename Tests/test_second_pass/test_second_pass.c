#include "../../HeaderFiles/second_pass.h"

int main()
{
    int retVal;
    int IC = BASE_INSTRUCTIONS_COUNTER; /* Insturctions Counter */
    int DC = 0;                         /* Data counter */
    printf("Starting main - test_first_pass!\n");

    SymbolTable *symbolTable = createSymbolTable();
    /* TODO: maybe change type to linkedList? */
    BinaryCodesTable *binaryCodesTable = createBinaryCodesTable();

    /* Open the input file */
    FILE *inputFile = fopen("./Tests/test_second_pass/test2_input_second_pass.txt", "r");
    const char *fileName = "test_input_second_pass.txt";

    if (inputFile == NULL)
    {
        printf("Error opening file.\n");
        return 1;
    }

    printf("--- Testing First Pass ---\n");

    retVal = firstPass(inputFile, symbolTable, binaryCodesTable, &IC, &DC);
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
    retVal = secondPass(inputFile, symbolTable, binaryCodesTable);

    if (retVal != SUCCESS) {
        printf("DEBUG - ERROR: %d\n", retVal);
    }
    else {
        printf("--- Finished Second Pass with SUCCESS result! ---\n");
    }

    printf("DEBUG - Check replaceFileNameExt\n");
    const char *newFileName = replaceFileNameExt(fileName, extension);
    printf("Original Name: %s\nNew fileName: %s\n", fileName, newFileName);

    printf("DEBUG - creating entries file\n");
    const char *entryFileName = replaceFileNameExt(fileName, EXTENSION_ENT);
    printSymbolTable(symbolTable);
    retVal = handleEntryFile(entryFileName, symbolTable);

    printf("DEBUG - creating externs file\n");
    const char *externalFileName = replaceFileNameExt(fileName, EXTENSION_EXT);
    retVal = handleExternFile(externalFileName, symbolTable);

    printf("DEBUG - creating objects file\n");
    const char *objectFileName = replaceFileNameExt(fileName, EXTENSION_OB);
    retVal = createObjectFile(objectFileName, binaryCodesTable, IC, DC);

    printf("Printing symbolTable\n");
    printSymbolTable(symbolTable);

    printf("Closing files\n");
    fclose(inputFile);

    printf("Freeing symbolTable\n");
    freeSymbolTable(symbolTable);

    return 0;
}