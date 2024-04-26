#ifndef BINARY_CODE_H
#define BINARY_CODE_H

#include <stdio.h>
#include <string.h>
#include "errors.h"
#include "logger.h"
#include "constants.h"
#include "assembly_line_parser.h"
#include "linked_list.h"
#include "binary_codes_table.h"

/* TOOD: decide if this function will insert to binaryCodesTable also or no */

/**
 * @brief Converts an integer to a binary string representation, and returns the binary string.
 * If needed, the returned value will be padded with zeros according to len.
 *
 * @param num The integer to be converted.
 * @param len The length of the binary string representation.
 * @return A pointer to the binary string representation of the integer.
 *
 */
char *convertIntToBinary(int num, int len);

/**
 * @brief Converts an integer to a binary string using the Two's complement method.
 * If needed, the returned value will be padded with zeros according to len.
 *
 * @param num The integer to be converted.
 * @param len The length of the binary string representation.
 * @return A pointer to the binary string representation of the integer.
 *
 */
char *convertIntToTCBinary(int num, int len);

/**
 * @brief Reverses a string of bits.
 * For example:
 *  - input: 0010
 *  - output: 0100
 *
 * @param bitsArray - A pointer to a character array (string) representing bits.
 * @noreturn This array is reversed in place.
 */
void reverseBits(char *bitsArray);

int getOperandsBinaryCode(AssemblyLine *parsedLine, LinkedList *symbolTable, BinaryCodesTable *binaryCodesTable, int IC);

/**
 * @brief Creates the binary code of an opcode and inserts to binaryCodesTable.
 *
 * The structure of an opcode's binary code is:
 * - bits 0-1: ARE codex - 'A'
 * - bits 2-3: src operand address type
 * - bits 4-5: dst operand address type
 * - bits 6-9: the opcode code
 * - bits 10-13: 0 (not in use)
 *
 * @return SUCCESS code or ERROR code.
 */
int handleOpcodeBinaryCode(AssemblyLine *parsedLine, BinaryCodesTable *binaryCodesTable, int *IC);

/**
 * @brief Creates the binary code of an operand with address type = 0 and inserts to binaryCodesTable.
 *
 * The structure of this operand binary code is:
 * - bits 0-1: ARE codex - 'A' - 00
 * - bits 2-14: the number in Two's complement method
 *
 * @return SUCCESS code or ERROR code.
 */
int handleAdrType0(Operand *operand, AssemblyLine *parsedLine, BinaryCodesTable *binaryCodesTable, LinkedList *symbolTable, int *IC);

/**
 * @brief Creates the binary code of an operand with address type = 1 and inserts to binaryCodesTable.
 *
 * The structure of this operand binary code is:
 * - bits 0-1: ARE codex:
 *                          - 'R' - 10 - for internal symbol
 *                          - 'E' - 01 - for external symbol
 * - bits 2-14: the opcode code
 *
 * @return SUCCESS code or ERROR code (symbol not exist or symbol wrong type).
 */
int handleAdrType1(Operand *operand, AssemblyLine *parsedLine, BinaryCodesTable *binaryCodesTable, LinkedList *symbolTable, int *IC);

int handleAdrType2(Operand *operand, AssemblyLine *parsedLine, BinaryCodesTable *binaryCodesTable, LinkedList *symbolTable, int *IC);

/**
 * @brief Creates the binary code of an operand with address type = 3.
 * In this case it creates only one binary code and inserts to binaryCodesTable.
 * 
 * @param isSource 1/0 - determines if the operand is src or dst
 *
 * The structure of this operand binary code is:
 * - bits 0-1: ARE codex - 'A' - 00
 * - bits 2-4: if isSource=0 -> the dst register number, else -> 0
 * - bits 5-7: if isSource=0 -> the src register number, else -> 0
 * - bist 8-14: 0 (not in use)
 *
 * @return SUCCESS code or ERROR code.
 */
int handleAdrType3(Operand *operand, int isSource, AssemblyLine *parsedLine, BinaryCodesTable *binaryCodesTable, int *IC);

/**
 * @brief Creates the binary code of a src operand and a dst operand with address type = 3.
 * In this case it creates only one binary code and inserts to binaryCodesTable.
 *
 * The structure of this operand binary code is:
 * - bits 0-1: ARE codex - 'A' - 00
 * - bits 2-4: the dst register number
 * - bits 5-7: the src register number
 * - bist 8-14: 0 (not in use)
 *
 * @return SUCCESS code or ERROR code.
 */
int handleAdrType3EdgeCase(AssemblyLine *parsedLine, BinaryCodesTable *binaryCodesTable, int *IC);

int handleOperandsBinaryCode(AssemblyLine *parsedLine, BinaryCodesTable *binaryCodesTable, LinkedList *symbolTable, int IC);

#endif /* BINARY_CODE_H */