#define MAX_LINE_LEN 81
#define MAX_MACRO_LEN 1024
#define MACRO_START "mcr"
#define MACRO_END "endmcr"

#define DEFINE_DIRECTIVE ".define"
#define NUM_INSTRUCTIONS 4

char *INSTRUCTIONS[] = {".data", ".string", ".extern", ".entry"};
