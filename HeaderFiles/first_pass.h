#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include "errors.h"
#include "utils.h"
#include "binary_code.h"
#include "symbol_table.h"
#include "assembly_line_parser.h"
#include "binary_codes_table.h"

/**
 * @brief This function handles define line.
 * Define lines structure is: <label:-optional> .define <symbol>=<value>
 *
 * @param parsedLine Pointer to the parsed assembly line.
 * @param symbolTable Pointer to the symbol table.
 *
 * @return int SUCCESS or Error code.
 *
 */
int handleDefine(AssemblyLine *parsedLine, SymbolTable *symbolTable);

/**
 * @brief This function handles data directive line.
 * Data directive lines structure is: <label:-optional> .data <values devided by `,`>
 *
 * @param parsedLine Pointer to the parsed assembly line.
 * @param symbolTable Pointer to the symbol table.
 * @param binaryCodesTable Pointer to the binary codes table.
 * @param DC Pointer to the Data Counter.
 *
 * @return int SUCCESS or Error code.
 *
 */
int handleDataDirective(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryCodesTable *binaryCodesTable, int *DC);

/**
 * @brief This function handles string directive line.
 * String directive lines structure is: <label:-optional> .string "<data>"
 *
 * @param parsedLine Pointer to the parsed assembly line.
 * @param symbolTable Pointer to the symbol table.
 * @param binaryCodesTable Pointer to the binary codes table.
 * @param DC Pointer to the Data Counter.
 *
 * @return int SUCCESS or Error code.
 *
 */
int handleStringDirective(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryCodesTable *binaryCodesTable, int *DC);

/**
 * @brief This function handles extern directive line.
 * Extern directive lines structure is: <label:-optional> .extern <operands>
 *
 * @param parsedLine Pointer to the parsed assembly line.
 * @param symbolTable Pointer to the symbol table.
 * @param binaryCodesTable Pointer to the binary codes table.
 *
 * @return int SUCCESS or Error code.
 *
 */
int handleExternDirective(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryCodesTable *binaryCodesTable);

/**
 * @brief This function handles entry directive line.
 * Entry directive lines structure is: <label:-optional> .entry <operands>
 *
 * @param parsedLine Pointer to the parsed assembly line.
 * @param symbolTable Pointer to the symbol table.
 * @param binaryCodesTable Pointer to the binary codes table.
 *
 * @return int SUCCESS or Error code.
 *
 */
int handleEntryDirective(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryCodesTable *binaryCodesTable);

/**
 * @brief This function handles command lines.
 * Command lines structure is: <label:-optional> <opcode> <src_operand-optional> <dst_operand-optional>
 *
 * @param parsedLine Pointer to the parsed assembly line.
 * @param symbolTable Pointer to the symbol table.
 * @param binaryCodesTable Pointer to the binary codes table.
 * @param IC Pointer to the Instructions Counter.
 *
 * @return int SUCCESS or Error code.
 *
 */
int handleCommandLine(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryCodesTable *binaryCodesTable, int *IC);

/**
 * @brief This function performs the first pass over the assembly code.
 *
 * The function reads the assembly code line by line from the input file.
 * It parses each line using assembly_line_parser and handles it based on its type (directive or command)
 * using matching handle function.
 * The labels from the parsed lines are inserted to symbolTable.
 * The parsed lines are decoded to binary code and inserted to binaryCodesTable.
 *
 * @param inputFile Pointer to the input file containing the assembly code.
 * @param symbolTable Pointer to a symbol table to be populated.
 * @param binaryCodesTable Pointer to a binary codes table to be populated.
 *
 * @return int SUCCESS or Error code.
 *
 */
int firstPass(FILE *inputFile, SymbolTable *symbolTable, BinaryCodesTable *binaryCodesTable);

#endif /* FIRST_PASS_H */