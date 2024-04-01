#include "first_pass.h"

int main()
{
    printf("Starting main - test_first_pass!\n");
    /* Open the input and output files */
    FILE *inputFile = fopen("./Tests/test_first_pass/test_input_first_pass.txt", "r");
    FILE *outputFile = fopen("./Tests/test_first_pass/test_output_first_pass.txt", "w");

    if (inputFile == NULL || outputFile == NULL)
    {
        printf("Error opening file.\n");
        return 1;
    }

    FILE *tempFile = tmpfile();
    removeExtraSpacesTabsAndComments(inputFile, tempFile);

    printf("first pass");
    firstPass(tempFile, outputFile);

    printf("Closing files\n");
    fclose(inputFile);
    fclose(tmpfile);
    fclose(outputFile);

    return 0;
}