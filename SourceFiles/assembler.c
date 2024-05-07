#include "../HeaderFiles/assembler.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        logger(LOG_LEVEL_WARNING, "Usage: %s <input_file.as>\n", argv[0]);
        return ERROR_WRONG_ASSEMBLER_USAGE;
    }

    SymbolTable *symbolTable = createSymbolTable();
    BinaryCodesTable *binaryCodesTable = createBinaryCodesTable();
    int funcRetVal, IC, DC;
    IC = DC = BASE_INSTRUCTIONS_COUNTER; /* TODO: change later, make them a variable of the passes */
    /* TODO: really need to change this, because ob file is wrong */

    const char *inputFileName = argv[1];
    FILE *inputFile = fopen(inputFileName, "r");
    if (!inputFile)
    {
        logger(LOG_LEVEL_ERROR, "Error opening file: %s\n", inputFileName);
        return ERROR_OPEN_FILE;
    }

    logger(LOG_LEVEL_INFO, "Cleaning file");
    const char *cleanFileName = replaceFileNameExt(inputFileName, EXTENSION_AS_CLEAN);
    FILE *cleanFile = fopen(cleanFileName, "w");
    if (!cleanFile)
    {
        logger(LOG_LEVEL_ERROR, "Error opening file: %s\n", cleanFileName);
        return ERROR_OPEN_FILE;
    }
    funcRetVal = cleanAssemblyFile(inputFile, cleanFile);
    fclose(inputFile);
    fclose(cleanFile);
    if (funcRetVal != SUCCESS)
    {
        logger(LOG_LEVEL_ERROR, "An error as occured: %d\n", funcRetVal);
        return funcRetVal;
    }

    logger(LOG_LEVEL_INFO, "Pre assembler - unpacking macros");
    const char *amFileName = replaceFileNameExt(inputFileName, EXTENSION_AM);
    FILE *amFile = fopen(amFileName, "w");
    cleanFile = fopen(cleanFileName, "r");
    if (!cleanFile || !amFile)
    {
        logger(LOG_LEVEL_ERROR, "Error opening file: %s or %s\n", cleanFileName, amFileName);
        return ERROR_OPEN_FILE;
    }
    funcRetVal = preAssembler(cleanFile, amFile);
    if (funcRetVal != SUCCESS)
    {
        logger(LOG_LEVEL_ERROR, "An error as occured: %d\n", funcRetVal);
        return funcRetVal;
    }
    fclose(cleanFile);
    fclose(amFile);

    logger(LOG_LEVEL_INFO, "First pass");
    amFile = fopen(amFileName, "r");
    if (!amFile)
    {
        logger(LOG_LEVEL_ERROR, "Error opening file: %s\n", amFileName);
        return ERROR_OPEN_FILE;
    }
    funcRetVal = firstPass(amFile, symbolTable, binaryCodesTable);
    if (funcRetVal != SUCCESS)
    {
        logger(LOG_LEVEL_ERROR, "An error as occured");
        return funcRetVal;
    }
    logger(LOG_LEVEL_INFO, "First pass - SUCCESS! printing binaryCodesTable:");
    /* printBinaryCodesTable(binaryCodesTable); */

    logger(LOG_LEVEL_INFO, "Second pass");
    fseek(amFile, 0, SEEK_SET); /* Resets the file pointer to the beginning of the file */
    funcRetVal = secondPass(amFile, symbolTable, binaryCodesTable);
    if (funcRetVal != SUCCESS)
    {
        logger(LOG_LEVEL_ERROR, "An error as occured");
        return funcRetVal;
    }
    fclose(amFile);
    logger(LOG_LEVEL_INFO, "Second pass - SUCCESS! printing binaryCodesTable:");
    /* printBinaryCodesTable(binaryCodesTable); */

    logger(LOG_LEVEL_INFO, "Exporting files");
    const char *entryFileName = replaceFileNameExt(inputFileName, EXTENSION_ENT);
    funcRetVal = handleEntryFile(entryFileName, symbolTable);
    if (funcRetVal != SUCCESS)
    {
        logger(LOG_LEVEL_ERROR, "An error as occured: %d\n", funcRetVal);
        return funcRetVal;
    }

    const char *externalFileName = replaceFileNameExt(inputFileName, EXTENSION_EXT);
    funcRetVal = handleExternFile(externalFileName, symbolTable);
    if (funcRetVal != SUCCESS)
    {
        logger(LOG_LEVEL_ERROR, "An error as occured: %d\n", funcRetVal);
        return funcRetVal;
    }

    const char *objectFileName = replaceFileNameExt(inputFileName, EXTENSION_OB);
    funcRetVal = createObjectFile(objectFileName, binaryCodesTable, IC, DC);
    if (funcRetVal != SUCCESS)
    {
        logger(LOG_LEVEL_ERROR, "An error as occured: %d\n", funcRetVal);
        return funcRetVal;
    }

    logger(LOG_LEVEL_INFO, "SUCCESS! Done assembler");

    /* free and close files */

    freeSymbolTable(symbolTable);
    freeBinaryCodesTable(binaryCodesTable);

    return SUCCESS;
}