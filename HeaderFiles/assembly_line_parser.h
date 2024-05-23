#ifndef ASSEMBLY_LINE_PARSER_H
#define ASSEMBLY_LINE_PARSER_H

#include "utils.h"

/**
 * @struct Operand
 * @brief A structure representing an operand in an assembly instruction.
 *
 * @var Operand::adrType
 * The addressing type of the operand. It can be one of the following: {-1,0,1,2,3}
 * @var Operand::value
 * The value of the operand. If no operand, value is '\0'.
 */
typedef struct Operand
{
    int adrType;
    char *value;
} Operand;

/**
 * @struct AssemblyLine
 * @brief A structure representing a parsed line of assembly code.
 *
 * @var AssemblyLine::label
 * The label part of the assembly line. NULL if no label is present.
 * @var AssemblyLine::instruction
 * The instruction part of the assembly line. NULL if no instruction is present.
 * @var AssemblyLine::operands
 * The operands part of the assembly line. NULL if no operands are present.
 * @var AssemblyLine::src
 * The src operand - holds adr type and value of the dst operand.
 * @var AssemblyLine::dst
 * The dst operand - holds adr type and value of the dst operand.
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

/* Pretty print of assembly line */
void printAssemblyLine(AssemblyLine *parsedLine);

/* Pretty print operands */
void printOperandsAfterParsing(AssemblyLine *parsedLine);

void freeAssemblyLine(AssemblyLine *line);

void freeOperand(Operand *operand);

/**
 * @brief Checks if a given AssemblyLine is a directive line.
 * The instruction in a directive line is one of DIRECTIVES.
 *
 * @param parsedLine An AssemblyLine object representing a parsed assembly line.
 * @return int 1 if the line is a directive line, and 0 otherwise.
 */
int isDirectiveLine(AssemblyLine *parsedLine);

/**
 * @brief Checks if a given AssemblyLine is a command line.
 * The instruction in a command line is an OPCODE.
 *
 * @param parsedLine An AssemblyLine object representing a parsed assembly line.
 * @return int 1 if the line is a command line, and 0 otherwise.
 */
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
 * @brief Checks if a symbol (label) is valid.
 * A valid symbol starts with an alphabetic letter (big or small),
 * followed by alphabetic letters or numbers, and its maximum length is 31.
 *
 * @param label The label to check.
 * @return int 1 if the label is valid, and 0 otherwise.
 */
int isValidSymbol(const char *label);

/**
 * @brief Checks if a register operand is valid.
 * A valid register starts with 'r' followed by a single digit from 0 to NUM_REGISTERS.
 * e.g: r1, r8
 *
 * @param operand The operand to check.
 * @return int 1 if the operand is a valid register, and 0 otherwise.
 */
int isValidRegisterOperand(const char *operand);

/**
 * @brief This function parses the adrType of operand into operandType.
 *
 * Operand addressing type, can be one of the following:
 *  - -1 - if operand doesn't exist
 *  -  0 - Immediate addressing
 *  -  1 - Direct addressing
 *  -  2 - Index fixed addressing
 *  -  3 - Register direct addressing
 * This function also runs validations on each operand that matches an addressing, returns an ERROR if validation failes.
 *
 * @param operand The input operand string to be parsed.
 * @return int SUCCESS code or ERROR code.
 */
int parseOperandAdressing(const char *operand, int *operandType);

/**
 * @brief This function sets src and dst variables of an AssemblyLine.
 *
 * This function parses the operands in an AssemblyLine to Operand objects.
 * If operands doesn't exist, is sets default values to Operand: adrType = -1, value = '\0'.
 *
 * @param parsedLine An AssemblyLine object representing a parsed assembly line.
 * @return int SUCCESS code or ERROR code.
 */
int parseOperands(struct AssemblyLine *parsedLine);

/**
 * @brief This function returns the Opcode code for a given opcode (according to the table at page 18 in the book).
 *
 * @param instruction A pointer to a string representing the instruction
 * @return Return the code attr of the matching opcode or -1 if the string does not match any known opcodes
 */
int getOpcodeCode(char *instruction);

/**
 * @brief This function returns the instruction operands number for a given opcode.
 * It iterates through the list of known opcodes and compare the string with each opcode in the list.
 *
 * @param instruction A pointer to a string representing the instruction
 * @return Return the number of operands of the matching opcode or -1 if the string does not match any known opcodes
 */
int getOpcodeOperandsNumber(char *instruction);

#endif /* ASSEMBLY_LINE_PARSER_H */