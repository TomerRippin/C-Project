#ifndef CONSTANTS_H
#define CONSTANTS_H

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
#define NUM_REGISTERS 8
#define NUM_OPCODES 16
#define NUM_REGISTERS 8

#define BINARY_CODE_LEN 15

extern const char *DIRECTIVES[NUM_DIRECTIVES];

typedef struct Opcode
{
    int code;
    char *name;
    int operandsNum;
} Opcode;

extern const Opcode OPCODES[];

#endif  /* CONSTANTS_H */

