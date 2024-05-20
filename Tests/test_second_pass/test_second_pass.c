#include "../../HeaderFiles/second_pass.h"

int main()
{
    int retVal, IC, DC;
    SymbolTable *symbolTable;
    BinaryCodesTable *binaryCodesTable;
    FILE *inputFile;
    char *fileName;
    const char *entryFileName, *externalFileName, *objectFileName;
    logger(LOG_LEVEL_DEBUG, "Starting main - test_first_pass!\n");

    IC = BASE_INSTRUCTIONS_COUNTER; /* Insturctions Counter */
    DC = 0;                         /* Data counter */

    symbolTable = createSymbolTable();
    binaryCodesTable = createBinaryCodesTable();

    /* Open the input file */
    inputFile = fopen("./Tests/test_second_pass/test2_input_second_pass.txt", "r");
    fileName = "test_input_second_pass.txt";

    if (inputFile == NULL)
    {
        logger(LOG_LEVEL_DEBUG, "Error opening file.\n");
        return 1;
    }

    logger(LOG_LEVEL_DEBUG, "--- Testing First Pass ---\n");

    retVal = firstPass(inputFile, symbolTable, binaryCodesTable, &IC, &DC);
    if (retVal != SUCCESS)
    {
        logger(LOG_LEVEL_DEBUG, "ERROR: %d\n", retVal);
        /* return retVal; */
    }
    else
    {
        printf("--- Finished First Pass with SUCCESS result! ---\n");
    }

    fseek(inputFile, 0, SEEK_SET); /* Resets the file pointer to the beginning of the file */

    logger(LOG_LEVEL_DEBUG, "--- Testing Second Pass ---\n");

    IC = BASE_INSTRUCTIONS_COUNTER; /* Insturctions Counter */
    DC = 0;                         /* Data counter */

    retVal = secondPass(inputFile, symbolTable, binaryCodesTable, &IC, &DC);

    if (retVal != SUCCESS) {
        logger(LOG_LEVEL_DEBUG, "ERROR: %d\n", retVal);
    }
    else {
        logger(LOG_LEVEL_DEBUG, "--- Finished Second Pass with SUCCESS result! ---\n");
    }

    logger(LOG_LEVEL_DEBUG, "creating entries file\n");
    entryFileName = replaceFileNameExt(fileName, EXTENSION_ENT);
    printSymbolTable(symbolTable);
    retVal = handleEntryFile(entryFileName, symbolTable);

    logger(LOG_LEVEL_DEBUG, "creating externs file\n");
    externalFileName = replaceFileNameExt(fileName, EXTENSION_EXT);
    retVal = handleExternFile(externalFileName, symbolTable);

    logger(LOG_LEVEL_DEBUG, "creating objects file\n");
    objectFileName = replaceFileNameExt(fileName, EXTENSION_OB);
    retVal = createObjectFile(objectFileName, binaryCodesTable, IC, DC);

    logger(LOG_LEVEL_DEBUG, "Printing symbolTable\n");
    printSymbolTable(symbolTable);

    logger(LOG_LEVEL_DEBUG, "Closing files\n");
    fclose(inputFile);

    logger(LOG_LEVEL_DEBUG, "Freeing symbolTable\n");
    freeSymbolTable(symbolTable);

    return 0;
}