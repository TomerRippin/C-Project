#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MAX_LINES_NUM 1024  /* TODO: decide if size ok */
#define MAX_LINE_LEN 81
#define MAX_LABEL_LEN 31
#define MAX_MACRO_LEN 1024
#define MACRO_START "mcr"
#define MACRO_END "endmcr"
#define MAX_MEMORY_WORDS 4096
#define MAX_NUM_LEN 5

#define BASE_INSTRUCTIONS_COUNTER 100

#define DEFINE_DIRECTIVE ".define"
#define DATA_DIRECTIVE ".data"
#define STRING_DIRECTIVE ".string"
#define EXTERN_DIRECTIVE ".extern"
#define ENTRY_DIRECTIVE ".entry"

#define SYMBOL_TYPE_MDEFINE "mdefine"
#define SYMBOL_TYPE_DATA "data"
#define SYMBOL_TYPE_EXTERNAL "external"
#define SYMBOL_TYPE_ENTRY "entry"
#define SYMBOL_TYPE_CODE "code"
#define SYMBOL_TYPE_EXTERNAL_USAGE "external usage"

#define NUM_DIRECTIVES 4
#define NUM_REGISTERS 8
#define NUM_OPCODES 16
#define NUM_REGISTERS 8

#define BINARY_CODE_LEN 14
#define DECODED_BINARY_CODE_LEN 8
#define DECODED_SYMBOL_0 '*'
#define DECODED_SYMBOL_1 '#'
#define DECODED_SYMBOL_2 '%'
#define DECODED_SYMBOL_3 '!'

#define EXTENSION_AS "as"
#define EXTENSION_AS_CLEAN "as-clean"
#define EXTENSION_AM "am"
#define EXTENSION_ENT "ent"
#define EXTENSION_EXT "ext"
#define EXTENSION_OB "ob"

extern const char *DIRECTIVES[NUM_DIRECTIVES];

typedef struct Opcode
{
    int code;
    char *name;
    int operandsNum;
} Opcode;

extern const Opcode OPCODES[];

#endif  /* CONSTANTS_H */

