#include "debug.h"

#define MAX_LINES_NUM 1024  /* TODO: decide if size ok */
#define MAX_LINE_LEN 81
#define MAX_LABEL_LEN 31
#define MAX_MACRO_LEN 1024
#define MACRO_START "mcr"
#define MACRO_END "endmcr"

#define DEFINE_DIRECTIVE ".define"
#define DATA_DIRECTIVE ".data"
#define STRING_DIRECTIVE ".string"
#define EXTERN_DIRECTIVE ".extern"
#define ENTRY_DIRECTIVE ".entry"

#define SYMBOL_TYPE_MDEFINE "mdefine"
#define SYMBOL_TYPE_DATA "data"
#define SYMBOL_TYPE_EXTERNAL "external"
#define SYMBOL_TYPE_CODE "code"

#define NUM_DIRECTIVES 4
#define NUM_OPCODES 15

char *DIRECTIVES[] = {".data", ".string", ".extern", ".entry"};

typedef struct Opcode
{
    char *name;
    int operandsNum;
} Opcode;

Opcode OPCODES[] = {
    {"mov", 2},
    {"cmp", 2},
    {"add", 2},
    {"sub", 2},
    {"lea", 2},
    {"not", 1},
    {"clr", 1},
    {"inc", 1},
    {"dec", 1},
    {"jmp", 1},
    {"bne", 1},
    {"red", 1},
    {"prn", 1},
    {"jsr", 1},
    {"rts", 0},
    {"hlt", 0}};