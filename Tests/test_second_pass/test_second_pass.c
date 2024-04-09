#include "../../Header Files/second_pass.h"

int main()
{
    int retVal;
    printf("Starting main - test_first_pass!\n");

    LinkedList *symbolTable = createList();
    /* TODO: maybe change type to linkedList? */
    int binaryCodesTable[MAX_LINES_NUM];

    /* Open the input file */
    FILE *inputFile = fopen("./Tests/test_second_pass/test_input_second_pass.txt", "r");
    char *fileName = "test_input_second_pass.txt";
    char *extension = "ext";

    if (inputFile == NULL)
    {
        printf("Error opening file.\n");
        return 1;
    }

    printf("Testing Second Pass\n");

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

    printf("Printing symbolTable\n");
    printList(symbolTable);

    /**
    printf("Printing binaryCodesTable\n");
    int i;
    for (i = 0; i < MAX_LINES_NUM * sizeof(int); i++)
    {
        printf("binaryCodesTable[%d]: %d\n", i, binaryCodesTable[i]);
    }
    */

    printf("Closing files\n");
    fclose(inputFile);

    printf("Freeing symbolTable\n");
    freeList(symbolTable);

    return 0;
}