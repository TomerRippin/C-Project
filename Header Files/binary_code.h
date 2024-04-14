#ifndef BINARY_CODE_H
#define BINARY_CODE_H

#include <stdio.h>
#include <string.h>
#include "constants.h"
#include "assembly_line_parser.h"

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
 * @brief Reverses a string of bits.
 * For example:
 *  - input: 0010
 *  - output: 0100
 *
 * @param bitsArray - A pointer to a character array (string) representing bits.
 * @noreturn This array is reversed in place.
 */
void reverseBits(char *bitsArray);

/**
 * Creates the binary code of an opcode.
 * NOTE: it reverses the bits so the bit in place 0 will be the on the right.
 *
 * @param parsedLine - A pointer to an AssemblyLine struct.
 * @return A pointer to a character array (string) representing the binary code of the opcode.
 */
char *getOpcodeBinaryCode(AssemblyLine *parsedLine);

#endif /* BINARY_CODE_H */