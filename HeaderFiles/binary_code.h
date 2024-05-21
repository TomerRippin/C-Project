#ifndef BINARY_CODE_H
#define BINARY_CODE_H

#include "errors.h"
#include "logger.h"
#include "constants.h"
#include "assembly_line_parser.h"
#include "symbol_table.h"
#include "binary_codes_table.h"

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
 * @brief Converts a binary string (representing bits) to a decimal number.
 *
 * @param binary - A pointer to a character array (string) representing binary string.
 * @return A decimal represantation of the binary number.
 */
int convertBinaryToDecimal(char *binary);

/**
 * @brief Decodes a given binaryCode to a decoded string, in the format of `encrypted-base-4`:
 * ----------------------------------------------
 * |   Normal Base 4    |  0  |  1  |  2  |  3  |
 * |--------------------|-----|-----|-----|-----|
 * |  Encrypted Base 4  |  *  |  #  |  %  |  !  |
 * ----------------------------------------------
 * 
 * @param binaryCode - A pointer to a character array (string) representing binary code.
 * @return A pointer to the encrypted-base-4 binary string representation of the code.
 */
char* decodeBinaryCode(char *binaryCode);

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
 * @brief Creates one binary code of an operand with address type = 0 and inserts to binaryCodesTable.
 * 
 * The structure of an operand in adr type 0 is: #<a number or a defined symbol>, e.g: #-1 or #sz
 * The structure of the binary code is:
 * - bits 0-1: ARE code: 'A' - 00
 * - bits 2-13: the number in Two's complement method
 *
 * @return SUCCESS code or ERROR code.
 */
int handleAdrType0(Operand *operand, AssemblyLine *parsedLine, BinaryCodesTable *binaryCodesTable, SymbolTable *symbolTable, int *IC);

/**
 * @brief Creates one binary code of an operand with address type = 1 and inserts to binaryCodesTable.
 * 
 * The structure of an operand in adr type 1 is: <a symbol>.
 * The structure of the binary code is:
 * - bits 0-1: ARE codex:
 *                          - 'R' - 10 - for internal symbol
 *                          - 'E' - 01 - for external symbol
 * - bits 2-13: the opcode code
 *
 * @return SUCCESS code or ERROR code (symbol not exist or symbol wrong type).
 */
int handleAdrType1(Operand *operand, AssemblyLine *parsedLine, BinaryCodesTable *binaryCodesTable, SymbolTable *symbolTable, int *IC);

/**
 * @brief Creates the binary code of an operand with address type = 2 and inserts to binaryCodesTable.
 * In this case it creates two binary words and inserts to binaryCodesTable.
 *
 * The structure of an operand in adr type 2 is: <symbol>[<symbol or number>], e.g: STR[5] or MOU[sz]
 * The function seperates between the symbol and the index (data inside the brackets), and creates two binary words:
 *
 * The structure of the symbol's word:
 * - bits 0-1: ARE codex:
 *                         - 'R' - 10 - for internal symbol
 *                         - 'E' - 01 - for external symbol
 * - bits 2-13: the symbol's address in the object file
 *
 * The structure of the index's word:
 * - bits 0-1: ARE codex:
 *                         - 'A' - 00 - for a number
 *                         - 'R' - 10 - for internal symbol
 * - bits 2-13: the index's value
 *
 * @return SUCCESS code or ERROR code.
 */
int handleAdrType2(Operand *operand, AssemblyLine *parsedLine, BinaryCodesTable *binaryCodesTable, SymbolTable *symbolTable, int *IC);

/**
 * @brief Creates one binary code of an operand with address type = 3 and inserts to binaryCodesTable.
 *
 * The structure of an operand in adr type 3 is: r<1-8>, e.g: r5.
 * The structure of the binary code is:
 * - bits 0-1: ARE codex - 'A' - 00
 * - bits 2-4: if isSource=0 -> the dst register number, else -> 0
 * - bits 5-7: if isSource=0 -> the src register number, else -> 0
 * - bist 8-13: 0 (not in use)
 *
 * @param isSource 1/0 - determines if the operand is src or dst
 *
 * @return SUCCESS code or ERROR code.
 */
int handleAdrType3(Operand *operand, int isSource, AssemblyLine *parsedLine, BinaryCodesTable *binaryCodesTable, int *IC);

/**
 * @brief Creates one binary code of a src operand and a dst operand with address type = 3 and inserts to binaryCodesTable.
 *
 * The structure of an operand in adr type 3 edge case is: r<1-8>,r<1-8>, e.g: r5,r1.
 * The structure of the binary code is:
 * - bits 0-1: ARE codex - 'A' - 00
 * - bits 2-4: the dst register number
 * - bits 5-7: the src register number
 * - bist 8-13: 0 (not in use)
 *
 * @return SUCCESS code or ERROR code.
 */
int handleAdrType3EdgeCase(AssemblyLine *parsedLine, BinaryCodesTable *binaryCodesTable, int *IC);

int handleOperandsBinaryCode(AssemblyLine *parsedLine, BinaryCodesTable *binaryCodesTable, SymbolTable *symbolTable, int IC);

#endif /* BINARY_CODE_H */