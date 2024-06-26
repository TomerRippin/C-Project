#include "../HeaderFiles/second_pass.h"

int secondPass(FILE *inputFile, SymbolTable *symbolTable, BinaryCodesTable *binaryCodesTable, int *IC, int *DC)
{
    int errorCode, hasError, lineNumber, L;
    char line[MAX_LINE_LEN];
    AssemblyLine parsedLine;
    SymbolNode *searchResult;

    errorCode = SUCCESS;
    hasError = lineNumber = L = 0;

    while (fgets(line, sizeof(line), inputFile) != NULL)
    {
        lineNumber++;

        /* Removes the newline character at the end of the line */
        line[strcspn(line, "\n")] = '\0';

        logger(LOG_LEVEL_DEBUG, "%d IC: %d DC: %d read line: %s", lineNumber, *IC, *DC, line);

        if (isEmptyLine(line) || isCommentedLine(line)){
            continue;
        }

        parsedLine = parseAssemblyLine(line);

        if (strcmp(parsedLine.instruction, DATA_DIRECTIVE) == 0 || strcmp(parsedLine.instruction, STRING_DIRECTIVE) == 0 ||
            strcmp(parsedLine.instruction, EXTERN_DIRECTIVE) == 0 || strcmp(parsedLine.instruction, DEFINE_DIRECTIVE) == 0)
        {
            /* string/data/extern - do nothing */
            /* NOTE: The book didn't said what to do with define, we assume it also should be ignored */
            continue;
        }
        else if (strcmp(parsedLine.instruction, ENTRY_DIRECTIVE) == 0) {
            if (parsedLine.label) {
                logger(LOG_LEVEL_WARNING, "A label is declared in an entry line! Line number: %d", lineNumber);
            }
            /* Check if there is also an .extern decleration to the same symbol */
            searchResult = searchSymbolNameTypeInTable(symbolTable, parsedLine.operands, SYMBOL_TYPE_EXTERNAL);
            if (searchResult != NULL) {
                printError(lineNumber, ERROR_SYMBOL_DECLARED_AS_ENTRY_AND_EXTERNAL);
                hasError = 1;
                continue;
            }
            searchResult = searchSymbolNameInTable(symbolTable, parsedLine.operands);
            if (searchResult == NULL)
            {
                printError(lineNumber, ERROR_GIVEN_SYMBOL_NOT_EXIST);
                hasError = 1;
                continue;
            }
            else {
                /* NOTE: we have already inserted entry symbol to the symbolTable in the first_pass. */
                continue;
            }
        }
        else {
            errorCode = parseOperands(&parsedLine);
            if (errorCode != SUCCESS)
            {
                printError(lineNumber, errorCode);
                hasError = 1;
                continue;
            }

            errorCode = handleOperandsBinaryCode(&parsedLine, binaryCodesTable, symbolTable, *IC + 1);  /* NOTE: this will still work even if operands is null */
            if (errorCode != SUCCESS)
            {
                printError(lineNumber, errorCode);
                hasError = 1;
                continue;
            }

            L = calculateL(parsedLine.src->adrType, parsedLine.dst->adrType);
            *IC = *IC + L;
        }

        if ((*IC + *DC) > MAX_MEMORY_WORDS)
        {
            logger(LOG_LEVEL_ERROR, "\x1b[1m%s (%d) ", getErrorMessage(ERROR_MEMORY_OVERFLOW), ERROR_MEMORY_OVERFLOW);
            return ERROR_MEMORY_OVERFLOW;
        }
    }

    sortSymbolTable(symbolTable);
    sortBinaryCodesTable(binaryCodesTable);

    return hasError;
}


int handleEntryFile(const char *filename, SymbolTable *symbolTable)
{
    int found;
    FILE *outputFile;
    SymbolNode *current, *searchResult;

    outputFile = openFile(filename, "w");
    current = symbolTable->head;
    found = 0;

    /* Search for an Entry in the symbol table*/
    while (current != NULL) {
        if (strcmp(current->symbolType, SYMBOL_TYPE_ENTRY) == 0)
        {
            /* Found an Entry, should create a file */
            found = 1;
            searchResult = searchSymbolTableWithType(symbolTable, current->symbolName, SYMBOL_TYPE_ENTRY, 0);
            /* Search for the place the Entry is defiend */
            if (searchResult == NULL) {
                logger(LOG_LEVEL_ERROR, "\x1b[1m%s (%d) - %s", getErrorMessage(ERROR_ENTRY_NOT_DEFINED), ERROR_ENTRY_NOT_DEFINED, current->symbolName);
                return ERROR_ENTRY_NOT_DEFINED;
            }
            else {
                logger(LOG_LEVEL_DEBUG, "inserting label <%s> to entries file at location <%04d>", current->symbolName, searchResult->symbolValue);
                fprintf(outputFile, "%s     %04d\n", searchResult->symbolName, searchResult->symbolValue);
            }
        }
        current = current->next;
    }
    fclose(outputFile);
    if (!found){
        /* No Entries found. delete Entries file */
        logger(LOG_LEVEL_INFO, "No Entries found. Not creating a .ent file");
        remove(filename);
    }
    else
    {
        logger(LOG_LEVEL_INFO, "Succesfully created entry file");
    }
    return SUCCESS;
}

int handleExternFile(const char *filename, SymbolTable *symbolTable) {
    int found;
    FILE *outputFile;
    SymbolNode *current;

    found = 0;
    outputFile = openFile(filename, "w");
    current = symbolTable->head;

    /* Searchs for an Extern label in the symbol table */
    while (current != NULL)
    {
        if (strcmp(current->symbolType, SYMBOL_TYPE_EXTERNAL) == 0)
        {
            /* Search for a usage of this external label */
            SymbolNode *searchResult = symbolTable->head;
            
            while (searchResult != NULL)
            {
                if ((strcmp(current->symbolName, searchResult->symbolName) == 0) && (strcmp(searchResult->symbolType, SYMBOL_TYPE_EXTERNAL_USAGE) == 0)){
                    /* Found an Extern label usage, should create a file */
                    found = 1;

                    logger(LOG_LEVEL_DEBUG, "inserting label <%s> to extern file at location <%d>", current->symbolName, searchResult->symbolValue);
                    fprintf(outputFile, "%s     %04d\n", searchResult->symbolName, searchResult->symbolValue);
                }
                searchResult = searchResult->next;
            }
        }
        current = current->next;
    }
    fclose(outputFile);
    if (!found)
    {
        /* No Entries found. delete Extern file */
        logger(LOG_LEVEL_INFO, "No externs found. Not creating a .ext file");
        remove(filename);
    }
    else
    {
        logger(LOG_LEVEL_INFO, "Succesfully created extern file");
    }
    return SUCCESS;
}

int createObjectFile(const char *filename, BinaryCodesTable *binaryCodesTable, int IC, int DC)
{
    int state;  /* Holds the ob line number, used to ignore writing duplicates to file */
    int count;
    char *line;
    FILE *outputFile;
    BinaryCodesNode *node;

    state = -1;
    count = 1;
    outputFile = openFile(filename, "w");
    line = (char *)malloc(sizeof(char) * BINARY_CODE_LEN);
    if (line == NULL)
    {
        logger(LOG_LEVEL_ERROR, "\x1b[1m%s (%d) ", getErrorMessage(ERROR_MEMORY_ALLOC_FAILED), ERROR_MEMORY_ALLOC_FAILED);
        exit(ERROR_MEMORY_ALLOC_FAILED);
    }

    /* First line is header - <IC> <DC> */
    sprintf(line, "  %d %d  \n", IC - BASE_INSTRUCTIONS_COUNTER, DC);
    fputs(line, outputFile);

    /** Rest of the lines are - <address> <decoded binary code> 
      * First all IC is written and then all DC */
    for (node = binaryCodesTable->head; node != NULL; node = node->next) {
        /* Ignore all of the DC instructions in the start of the binaryCodesTable - will be written in the end */
        if (count <= DC) {
            count++;
            continue;
        }
        /* Works only if symbol table is sorted - check if a line is already been inserted.*/
        if (node->decAddress == state) {
            continue;
        }
        sprintf(line, "%04d %s\n", node->decAddress, decodeBinaryCode(node->binaryCode));
        fputs(line, outputFile);
        state = node->decAddress;
    }

    /* Now inserts all of the DC instructions in the end of the file */
    count = 1;
    for (node = binaryCodesTable->head; count <= DC; node = node->next) {

        sprintf(line, "%04d %s\n", node->decAddress + IC, decodeBinaryCode(node->binaryCode));
        fputs(line, outputFile);

        count++;
    } 

    fclose(outputFile);
    free(line);

    logger(LOG_LEVEL_INFO, "Succesfully created object file");

    return SUCCESS;
}