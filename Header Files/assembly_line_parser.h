#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"

/*TODO: document like the other structs */
typedef struct Operand
{
    int *type; /* what type of adressing the operand has (0, 1, 2, 3) */
    char *value; /* the value of the operand  (r8, sz, LOOP, ARR[0] etc... )*/
} Operand;

/**
 * @brief A structure representing a line of assembly code.
 *
 * @var AssemblyLine::label
 * The label part of the assembly line. NULL if no label is present.
 * @var AssemblyLine::instruction
 * The instruction part of the assembly line. NULL if no instruction is present.
 * @var AssemblyLine::operands
 * The operands part of the assembly line. NULL if no operands are present.
 */
typedef struct AssemblyLine
{
    char *label;
    char *instruction;
    char *operands;
    Operand *src;
    Operand *dst;
    /* int *operandsNum; */
} AssemblyLine;


/**
 * @brief Parses a line of assembly code into an AssemblyLine struct.
 *
 * The assembly line is expected to be in the format "<label> <instruction> <operands>".
 * For example: "MAIN: .data 5", ".define sz=2", "mov STR[5],STR[2]", "END: hlt"
 * The label is optional and if exists, is expected to be followed by a ':'.
 * The instruction and operands are separated by a space. If no space is present, there is no operands and
 * the entire line after the label is treated as the instruction.
 *
 * @param line The line of assembly code to parse.
 * @return An AssemblyLine struct representing the parsed assembly line.
 */
struct AssemblyLine parseAssemblyLine(const char *line);

void printAssemblyLine(AssemblyLine *parsedLine);

void freeAssemblyLine(AssemblyLine *line);

/* Idea - validations */
int isValidLabel(char *str);
int isValidString(char *str);
