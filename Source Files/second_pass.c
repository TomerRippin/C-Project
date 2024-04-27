#include "../Header Files/second_pass.h"

int secondPass(FILE *inputFile, char *inputFileName, SymbolTable *symbolTable, BinaryCodesTable *binaryCodesTable)
{
    int IC = BASE_INSTRUCTIONS_COUNTER;
    char line[MAX_LINE_LEN];
    AssemblyLine parsedLine;
    SymbolNode *searchResult;

    /* TODO: maybe dont go over all the lines, and just go over the symbolTable + binaryCodesTable */
    while (fgets(line, sizeof(line), inputFile) != NULL)
    {
        /* Remove the newline character at the end of the line */
        line[strcspn(line, "\n")] = '\0';

        parsedLine = parseAssemblyLine(line);
        printAssemblyLine(&parsedLine);


        if (strcmp(parsedLine.instruction, DATA_DIRECTIVE) == 0 ||
                 strcmp(parsedLine.instruction, STRING_DIRECTIVE) == 0 ||
                 strcmp(parsedLine.instruction, EXTERN_DIRECTIVE) == 0 ||
                 strcmp(parsedLine.instruction, DEFINE_DIRECTIVE) == 0)  /* TODO: what to od with define? */
        {
            logger(LOG_LEVEL_DEBUG, "string/data/extern - do nothing");
            continue;
        }
        else if (strcmp(parsedLine.instruction, ENTRY_DIRECTIVE) == 0) {
            logger(LOG_LEVEL_DEBUG, "entry - do line 6");
            if (parsedLine.label) {
                logger(LOG_LEVEL_WARNING, "a label is declared in an entry line");
            }
            searchResult = searchList(symbolTable, parsedLine.operands);
            if (searchResult == NULL) {
                return ERROR_GIVEN_SYMBOL_NOT_EXIST;
            }
            else if (strcmp(searchResult->symbolType, SYMBOL_TYPE_EXTERNAL) == 0)  /* TODO: test this error */
            {
                return ERROR_LABEL_DECLARED_AS_ENTRY_AND_EXTERNAL;
            }
            else {
                /* TODO: line 6 - update entry symbol */
                printf("DEBUG - line 6 - TODO");
            }
        }
        else {
            /* TODO: section 7-8 */
            printf("DEBUG - section 7-8\n");
            int funcsRetVal = parseOperands(&parsedLine);
            if (funcsRetVal != SUCCESS)
            {
                logger(LOG_LEVEL_ERROR, "got an error in 'parseOperands': %d", funcsRetVal);
                return funcsRetVal;
            }

            funcsRetVal = handleOperandsBinaryCode(&parsedLine, binaryCodesTable, symbolTable, IC + 1);  /* NOTE: this will still work even if operands is null */
            if (funcsRetVal != SUCCESS)
            {
                logger(LOG_LEVEL_ERROR, "got an error in 'handleOperandsBinaryCode': %d", funcsRetVal);
                return funcsRetVal;
            }
            int L = calculateL(parsedLine.src->adrType, parsedLine.dst->adrType);
            IC = IC + L;
        }
    }

    printBinaryList(binaryCodesTable);
    /* Print the extern labels and their addresses to the '.externals' output file */
    /* Print the entry labels and their addresses to the '.entries' output file */
    /* Free all the allocated memory and resources used during the second pass */

    return SUCCESS;
}


int handleEntryFile(char *filename, SymbolTable *symbolTable){
    SymbolNode *current = symbolTable->head;
    int found = 0;

    FILE* outputFile = fopen(filename, "w");
    if (outputFile == NULL)
    {
        logger(LOG_LEVEL_ERROR, "Error opening entry file.\n");
        return ERROR_OPEN_FILE;
    }

    /* Search for an Entry in the symbol table*/
    while (current != NULL){
        if (strcmp(current->symbolType, SYMBOL_TYPE_ENTRY) == 0){
            /* Found an Entry, should create a file */
            logger(LOG_LEVEL_DEBUG, "Found an Entry - label name: <%s>", current->symbolName);
            found = 1;
            SymbolNode *searchResult = searchListWithType(symbolTable, current->symbolName, SYMBOL_TYPE_ENTRY, 0);
            logger(LOG_LEVEL_DEBUG, "result.symbolType: %s", searchResult->symbolType);
            /* Search for the place the Entry is defiend */
            if (searchResult == NULL){
                /* Found an entry but it is not defiend anywhere */
                return ERROR_UNKNOWN_INSTRUCTION;
            }
            else {
                logger(LOG_LEVEL_INFO, "inserting label <%s> to entries file at location <%04d>", current->symbolName, searchResult->symbolValue);
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
    return SUCCESS;
}

int handleExternFile(char *filename, SymbolTable *symbolTable){
    SymbolNode *current = symbolTable->head;
    int found = 0;
    FILE* outputFile = fopen(filename, "w");
    if (outputFile == NULL)
    {
        logger(LOG_LEVEL_ERROR, "Error opening extern file.\n");
        return ERROR_OPEN_FILE;
    }

    /* Search for an Entry in the symbol table*/
    while (current != NULL){
        if (strcmp(current->symbolType, SYMBOL_TYPE_EXTERNAL) == 0){
            /* Found an Entry, should create a file */
            logger(LOG_LEVEL_DEBUG, "Found an Entry - label name: <%s>", current->symbolName);
            SymbolNode *searchResult = symbolTable->head;
            while (searchResult != NULL)
            {
                if ((strcmp(current->symbolName, searchResult->symbolName) == 0) && (strcmp(searchResult->symbolType, SYMBOL_TYPE_EXTERNAL_USAGE) == 0)){
                    logger(LOG_LEVEL_INFO, "inserting label <%s> to extern file at location <%d>", current->symbolName, searchResult->symbolValue);
                    fprintf(outputFile, "%s     %04d\n", searchResult->symbolName, searchResult->symbolValue);
                    found = 1;
                }
                searchResult = searchResult->next;
            }
        }
        current = current->next;
    }
    fclose(outputFile);
    if (!found){
        /* No Entries found. delete Extern file */
        logger(LOG_LEVEL_INFO, "No externs found. Not creating a .ext file");
        remove(filename);
    }
    return SUCCESS;
}

int createObjectFile(char *filename, BinaryCodesTable *binaryCodesTable, int IC, int DC)
{
    BinaryCodesNode *node;
    char *line = (char*) malloc(sizeof(char) * BINARY_CODE_LEN);
    FILE* outputFile = fopen(filename, "w");
    if (outputFile == NULL)
    {
        logger(LOG_LEVEL_ERROR, "Error opening objects file.\n");
        return ERROR_OPEN_FILE;
    }

    /* First line is header - <IC> <DC> */
    sprintf(line, "%d %d\n", IC - BASE_INSTRUCTIONS_COUNTER, DC);
    fputs(line, outputFile);

    for (node = binaryCodesTable->head; node != NULL; node = node->next)
    {
        sprintf(line, "%04d %s\n", node->decAddress, decodeBinaryCode(node->binaryCode));
        fputs(line, outputFile);
    }

    fclose(outputFile);
    free(line);

    return SUCCESS;
}