#ifndef OPERANDS_PARSER_H
#define OPERANDS_PARSER_H

/**
 * Function to parse the type of operand.
 * @param operand The input operand string to be parsed.
 * @return The type of operand.
 */
int parseOperandAdressing(const char *operand);

/**
 * Function to count the occurrences of a specific character in a string.
 * 
 * @param str Pointer to the input string.
 * @param target The character to count occurrences of.
 * 
 * @return The number of occurrences of the target character in the string.
 */
int countOccurrences(char *str, char target);


int parseOperands(struct AssemblyLine *parsedLine);


#endif /* OPERANDS_PARSER_H */