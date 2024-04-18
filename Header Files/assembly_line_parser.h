#ifndef ASSEMBLY_LINE_PARSER_H
#define ASSEMBLY_LINE_PARSER_H

#include <ctype.h>
#include "errors.h"
#include "utils.h"

/**
 * @struct Operand
 * @brief A structure representing an operand in an assembly instruction.
 *
 * @var Operand::adrType
 * The addressing type of the operand. It can be one of the following:
 * -1 - if operand doesn't exist
 * 0  - Immediate addressing
 * 1  - Direct addressing
 * 2  - Index fixed addressing
 * 3  - Register direct addressing
 *
 * @var Operand::value
 * The value of the operand.
 */
typedef struct Operand
{
    int adrType;
    char *value;
} Operand;

/**
 * @struct AssemblyLine
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

void printOperandsAfterParsing(AssemblyLine *parsedLine);

void freeAssemblyLine(AssemblyLine *line);

int isDirectiveLine(AssemblyLine *parsedLine);

int isCommandLine(AssemblyLine *parsedLine);

/**
 * @brief Checks if a string definition is valid.
 * A valid string is a string that starts and ends with a quote (").
 *
 * @param str The string to check.
 * @return int 1 if the string is valid, and 0 otherwise.
 */
int isValidString(char *str);

/**
 * @brief Checks if a label is valid.
 * A valid label starts with an alphabetic letter (big or small),
 * followed by alphabetic letters or numbers, and its maximum length is 31.
 *
 * @param label The label to check.
 * @return int 1 if the label is valid, and 0 otherwise.
 */
int isValidLabel(const char *label);

int isValidRegisterOperand(const char *operand);

/**
 * Function to parse the adrType of operand.
 *
 * @param operand The input operand string to be parsed.
 * @return The adrType of operand.
 */
int parseOperandAdressing(const char *operand, int *operandType);

int parseOperands(struct AssemblyLine *parsedLine);

/**
 * @brief This function returns the Opcode code for a given opcode (according to the table at page 18 in the book).
 *
 * @param instruction A pointer to a string representing the instruction
 *
 * @return Return the code attr of the matching opcode or -1 if the string does not match any known opcodes
 */
int getOpcodeCode(char *instruction);

/**
 * @brief This function returns the instruction operands number for a given opcode.
 * It iterates through the list of known opcodes and compare the string with each opcode in the list.
 *
 * @param instruction A pointer to a string representing the instruction
 *
 * @return Return the number of operands of the matching opcode or -1 if the string does not match any known opcodes
 */
int getOpcodeOperandsNumber(char *instruction);

#endif /* ASSEMBLY_LINE_PARSER_H */