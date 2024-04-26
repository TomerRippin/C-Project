#include "utils.h"

int main() {
    FILE *inputFile = fopen("./Tests/test_clean_file/test2_mess_input.txt", "r");
    FILE *outputFile = fopen("./Tests/test_clean_file/test2_clean_output.as", "w");
    int result;
    result = removeExtraSpacesTabsAndComments(inputFile, outputFile);
    result ++;
    fclose(inputFile);
    fclose(outputFile);
    return 0;
}