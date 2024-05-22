#include "../HeaderFiles/assembler.h"

int main(int argc, char *argv[])
{
    SymbolTable *symbolTable;
    BinaryCodesTable *binaryCodesTable;
    FILE *cleanFile, *inputFile, *amFile;
    int funcRetVal, IC, DC;
    char *cleanFileName;
    const char *inputFileName;
    const char *amFileName;
    const char *entryFileName;
    const char *externalFileName;
    const char *objectFileName;

    if (argc != 2)
    {
        logger(LOG_LEVEL_WARNING, "Usage: %s <input_file.as>\n", argv[0]);
        return ERROR_WRONG_ASSEMBLER_USAGE;
    }

    symbolTable = createSymbolTable();
    binaryCodesTable = createBinaryCodesTable();
    IC = BASE_INSTRUCTIONS_COUNTER; /* Insturctions Counter */
    DC = 0;                         /* Data counter */

    inputFileName = argv[1];
    inputFile = openFile(inputFileName, "r");

    if (isCRLF(inputFile) == 1)
    {
        logger(LOG_LEVEL_ERROR, "Error file is in unsopported Windows format (CRLF) instead of Unix (LF)");
        return ERROR_UNSUPPORTED_CRLF_FORMAT;
    }
    fseek(inputFile, 0, SEEK_SET); /* Resets the file pointer to the beginning of the file */

    logger(LOG_LEVEL_INFO, "\x1b[1mRunning assembler on file: %s\x1b[0m", inputFileName);

    /* Clean File */
    logger(LOG_LEVEL_INFO, "\x1b[1mCleaning file");
    cleanFileName = replaceFileNameExt(inputFileName, EXTENSION_AS_CLEAN);
    cleanFile = openFile(cleanFileName, "w");
    funcRetVal = cleanAssemblyFile(inputFile, cleanFile);
    fclose(inputFile);
    fclose(cleanFile);
    if (funcRetVal != SUCCESS)
    {
        logger(LOG_LEVEL_INFO, "Received an error, exit code: %d", funcRetVal);
        remove(cleanFileName);
        return funcRetVal;
    }
    logger(LOG_LEVEL_INFO, "Done cleaning file, created temp file: %s", cleanFileName);

    /* Pre Assembler */
    logger(LOG_LEVEL_INFO, "\x1b[1mPre assembler - unpacking macros\x1b[0m");
    amFileName = replaceFileNameExt(inputFileName, EXTENSION_AM);
    amFile = openFile(amFileName, "w");
    cleanFile = openFile(cleanFileName, "r");
    funcRetVal = preAssembler(cleanFile, amFile);
    fclose(cleanFile);
    fclose(amFile);
    if (funcRetVal != SUCCESS)
    {
        remove(cleanFileName);
        logger(LOG_LEVEL_INFO, "Received an error, exit code: %d", funcRetVal);
        return funcRetVal;
    }
    remove(cleanFileName);
    logger(LOG_LEVEL_INFO, "Done pre assembler, created new file: %s", amFileName);

    /* First Pass */
    logger(LOG_LEVEL_INFO, "\x1b[1mFirst pass\x1b[0m");
    amFile = openFile(amFileName, "r");
    funcRetVal = firstPass(amFile, symbolTable, binaryCodesTable, &IC, &DC);
    if (funcRetVal != SUCCESS)
    {
        logger(LOG_LEVEL_INFO, "Received an error, exit code: %d", funcRetVal);
        return funcRetVal;
    }
    logger(LOG_LEVEL_INFO, "Done first pass");

    /* Second Pass */
    logger(LOG_LEVEL_INFO, "\x1b[1mSecond pass\x1b[0m");
    fseek(amFile, 0, SEEK_SET); /* Resets the file pointer to the beginning of the file */
    IC = 100;
    funcRetVal = secondPass(amFile, symbolTable, binaryCodesTable, &IC, &DC);
    if (funcRetVal != SUCCESS)
    {
        logger(LOG_LEVEL_INFO, "Received an error, exit code: %d", funcRetVal);
        return funcRetVal;
    }
    fclose(amFile);
    logger(LOG_LEVEL_INFO, "Done second pass");

    /* Export Files */
    logger(LOG_LEVEL_INFO, "\x1b[1mExport files\x1b[0m");
    entryFileName = replaceFileNameExt(inputFileName, EXTENSION_ENT);
    funcRetVal = handleEntryFile(entryFileName, symbolTable);
    if (funcRetVal != SUCCESS)
    {
        logger(LOG_LEVEL_INFO, "Received an error, exit code: %d", funcRetVal);
        return funcRetVal;
    }

    externalFileName = replaceFileNameExt(inputFileName, EXTENSION_EXT);
    funcRetVal = handleExternFile(externalFileName, symbolTable);
    if (funcRetVal != SUCCESS)
    {
        logger(LOG_LEVEL_INFO, "Received an error, exit code: %d", funcRetVal);
        return funcRetVal;
    }

    objectFileName = replaceFileNameExt(inputFileName, EXTENSION_OB);
    funcRetVal = createObjectFile(objectFileName, binaryCodesTable, IC, DC);
    if (funcRetVal != SUCCESS)
    {
        logger(LOG_LEVEL_INFO, "Received an error, exit code: %d", funcRetVal);
        return funcRetVal;
    }

    freeSymbolTable(symbolTable);
    freeBinaryCodesTable(binaryCodesTable);

    logger(LOG_LEVEL_INFO, "\x1b[1mSUCCESS! Done assembler\x1b[0m");

    return SUCCESS;
}