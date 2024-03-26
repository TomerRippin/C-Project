#include "utils.h"

int main() {
    const char *inputFilename = "./Tests/test_clean_file/test_mess_input.as";
    const char *outputFilename = "./Tests/test_clean_file/test_clean_output.as";
    int result;
    result = removeExtraSpacesTabsAndComments(inputFilename, outputFilename);
    result ++;
    return 0;
}