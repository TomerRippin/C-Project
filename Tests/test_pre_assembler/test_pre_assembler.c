#include "pre_assembler.h"

int main()
{
    printf("Starting main - test_pre_assembler!\n");
    /* Open the input and output files */
    FILE *inputFile = fopen("./Tests/test_pre_assembler/input_test_pre_assembler.txt", "r");
    FILE *outputFile = fopen("./Tests/test_pre_assembler/output_test_pre_assembler.txt", "w");

    if (inputFile == NULL || outputFile == NULL)
    {
        printf("Error opening file.\n");
        return 1;
    }

    printf("preAssembler\n");
    preAssembler(inputFile, outputFile);

    printf("Closing files\n");
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}