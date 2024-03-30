#define MAX_LINE_LEN 81
#define MAX_MACRO_LEN 1024
#define MACRO_START "mcr"
#define MACRO_END "endmcr"

#define DEFINE_DIRECTIVE ".define"
#define NUM_INSTRUCTIONS 4
#define NUM_OPCODES 15

char *INSTRUCTIONS[] = {".data", ".string", ".extern", ".entry"};

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


/* Define the registers */
char *REGS[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

typedef struct Operand
{
    int type; /* what type of adressing the operand has (0, 1, 2, 3) */
    char *value; /* the value of the operand  (r8, sz, LOOP, ARR[0] etc... )*/
} Operand;