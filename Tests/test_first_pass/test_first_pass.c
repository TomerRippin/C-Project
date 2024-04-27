#include "../../Header Files/first_pass.h"

int main()
{
    int retVal;
    SymbolTable *symbolTable;
    /* TODO: maybe change type to linkedList? */
    BinaryCodesTable *binaryCodesTable = createBinaryCodesTable();
    FILE *inputFile, *inputFile2;

    printf("Starting main - test_first_pass!\n");
    symbolTable = createList();

    /* Open the input files */
    inputFile = fopen("./Tests/test_first_pass/test_input_first_pass.txt", "r");
    inputFile2 = fopen("./Tests/test_first_pass/test_input_first_pass_symbol_exist.txt", "r");

    if (inputFile == NULL)
    {
        printf("Error opening file.\n");
        return 1;
    }

    printf("Testing First Pass\n");
    
    printf("Input file: test_input_first_pass\n");
    retVal = firstPass(inputFile, symbolTable, binaryCodesTable);
    if (retVal != SUCCESS) {
        printf("DEBUG - ERROR: %d\n", retVal);
        /* return retVal; */
    }
    else {
        printf("--- Finished First Pass with SUCCESS result! ---\n");
    }

    printf("Printing symbolTable\n");
    printList(symbolTable);

    freeList(symbolTable);
    symbolTable = createList();

    printf("Input file: test_input_first_pass_symbol_exist\n");
    retVal = firstPass(inputFile2, symbolTable, binaryCodesTable);
    if (retVal != SUCCESS)
    {
        printf("--- GOT AN ERROR AS EXPECTED: %d ---\n", retVal);
    }

    printf("--- Printing symbolTable ---\n");
    printList(symbolTable);

    /**
    printf("Printing binaryCodesTable\n");
    int i;
    for (i = 0; i < MAX_LINES_NUM * sizeof(int); i++)
    {
        printf("binaryCodesTable[%d]: %d\n", i, binaryCodesTable[i]);
    }
    */

    printf("--- Checking decodeBinaryCode ---\n");
    char binary[15] = "00011011111001";
    printf("Binary string: 00011011111001\n");
    printf("Expected decoded string: *#%%!!%%#\n");
    char *decoded = decodeBinaryCode(binary);
    printf("Got decoded string: %s\n", decoded);
    free(decoded);

    printf("--- Closing files ---\n");
    fclose(inputFile);
    fclose(inputFile2);

    printf("--- Freeing symbolTable ---\n");
    freeList(symbolTable);

    return 0;
}