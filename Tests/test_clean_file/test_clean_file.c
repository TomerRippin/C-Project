#include "utils.h"

int main() {
    FILE *inputFile = fopen("./Tests/test_clean_file/test_mess_input.as", "r");
    FILE *outputFile = fopen("./Tests/test_clean_file/test_clean_output.as", "w");
    int result;
    result = removeExtraSpacesTabsAndComments(inputFile, outputFile);
    result ++;
    fclose(inputFile);
    fclose(outputFile);
    return 0;
}