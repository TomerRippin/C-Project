#include "../HeaderFiles/second_pass.h"

int secondPass(FILE *inputFile, SymbolTable *symbolTable, BinaryCodesTable *binaryCodesTable, int *IC, int *DC)
{
    int errorCode = SUCCESS;
    int hasError = 0;
    int lineNumber = 0;
    int L = 0;
    char line[MAX_LINE_LEN];
    AssemblyLine parsedLine;
    SymbolNode *searchResult;

    while (fgets(line, sizeof(line), inputFile) != NULL)
    {
        lineNumber++;

        /* Remove the newline character at the end of the line */
        line[strcspn(line, "\n")] = '\0';

        logger(LOG_LEVEL_DEBUG, "%d IC: %d DC: %d read line: %s", lineNumber, *IC, *DC, line);

        if (isEmptyLine(line) || isCommentedLine(line)){
            logger(LOG_LEVEL_WARNING, "Empty or Commented Line! Line number: %d", lineNumber);
            continue;
        }

        parsedLine = parseAssemblyLine(line);

        if (strcmp(parsedLine.instruction, DATA_DIRECTIVE) == 0 ||
            strcmp(parsedLine.instruction, STRING_DIRECTIVE) == 0 ||
            strcmp(parsedLine.instruction, EXTERN_DIRECTIVE) == 0 ||
            strcmp(parsedLine.instruction, DEFINE_DIRECTIVE) == 0)  /* TODO: what to do with define? */
        {
            /* string/data/extern - do nothing */
            continue;
        }
        else if (strcmp(parsedLine.instruction, ENTRY_DIRECTIVE) == 0) {
            logger(LOG_LEVEL_DEBUG, "entry - do line 6");
            if (parsedLine.label) {
                logger(LOG_LEVEL_WARNING, "a label is declared in an entry line");
            }
            searchResult = searchSymbolNameInTable(symbolTable, parsedLine.operands);
            if (searchResult == NULL) {
                printError(lineNumber, ERROR_GIVEN_SYMBOL_NOT_EXIST);
                hasError = 1;
                continue;
            }
            else if (strcmp(searchResult->symbolType, SYMBOL_TYPE_EXTERNAL) == 0)  /* TODO: test this error */
            {
                printError(lineNumber, ERROR_LABEL_DECLARED_AS_ENTRY_AND_EXTERNAL);
                hasError = 1;
                continue;
            }
            else {
                /* NOTE: this is not neccessary because of the way we do the first pass (inserting as entries) and 
                  create entries file (ignore duplicates)*/
                insertToSymbolTable(symbolTable, parsedLine.operands, SYMBOL_TYPE_ENTRY, searchResult->symbolValue);
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
    }

    sortSymbolTable(symbolTable);
    sortBinaryCodesTable(binaryCodesTable);

    /* TODO: insert the below functions to second_pass - or maybe not?*/

    return hasError;
}


int handleEntryFile(const char *filename, SymbolTable *symbolTable){
    SymbolNode *current, *searchResult;
    int found;
    FILE *outputFile;

    outputFile = openFile(filename, "w");
    current = symbolTable->head;
    found = 0;

    /* Search for an Entry in the symbol table*/
    while (current != NULL){
        if (strcmp(current->symbolType, SYMBOL_TYPE_ENTRY) == 0)
        {
            /* Found an Entry, should create a file */
            found = 1;
            searchResult = searchSymbolTableWithType(symbolTable, current->symbolName, SYMBOL_TYPE_ENTRY, 0);
            /* Search for the place the Entry is defiend */
            if (searchResult == NULL){
                /* Found an entry but it is not defined anywhere */
                return ERROR_UNKNOWN_INSTRUCTION;
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
    int found = 0;
    FILE *outputFile;
    SymbolNode *current;

    outputFile = openFile(filename, "w");
    current = symbolTable->head;

    /* Search for an Entry in the symbol table*/
    while (current != NULL)
    {
        if (strcmp(current->symbolType, SYMBOL_TYPE_EXTERNAL) == 0)
        {
            /* Found an Entry, should create a file */
            SymbolNode *searchResult = symbolTable->head;
            while (searchResult != NULL)
            {
                if ((strcmp(current->symbolName, searchResult->symbolName) == 0) && (strcmp(searchResult->symbolType, SYMBOL_TYPE_EXTERNAL_USAGE) == 0)){
                    logger(LOG_LEVEL_DEBUG, "inserting label <%s> to extern file at location <%d>", current->symbolName, searchResult->symbolValue);
                    fprintf(outputFile, "%s     %04d\n", searchResult->symbolName, searchResult->symbolValue);
                    found = 1;
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
    BinaryCodesNode *node;
    int state = -1;
    int count = 1;
    char *line = (char*) malloc(sizeof(char) * BINARY_CODE_LEN);
    FILE *outputFile;

    outputFile = openFile(filename, "w");

    /* First line is header - <IC> <DC> */
    sprintf(line, "  %d %d  \n", IC - BASE_INSTRUCTIONS_COUNTER, DC);
    fputs(line, outputFile);

    for (node = binaryCodesTable->head; node != NULL; node = node->next)
    {
        /* ignore all of the DC instructions in the start of the binaryCodesTable */
        if (count <= DC){
            count++;
            continue;
        }
        /* works only if symbol table is sorted - check if a line is already been inserted.*/
        if (node->decAddress == state){
            continue;
        }
        sprintf(line, "%04d %s\n", node->decAddress, decodeBinaryCode(node->binaryCode));
        fputs(line, outputFile);
        state = node->decAddress;
    }

    /* now insert all of the DC instructions in the end of the file */
    count = 1;
    for (node = binaryCodesTable->head; count <= DC; node = node->next){

        sprintf(line, "%04d %s\n", node->decAddress + IC, decodeBinaryCode(node->binaryCode));
        fputs(line, outputFile);

        count++;
    } 

    fclose(outputFile);
    free(line);

    logger(LOG_LEVEL_INFO, "Succesfully created object file");

    return SUCCESS;
}