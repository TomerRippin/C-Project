#ifndef BINARY_CODES_TABLE_H
#define BINARY_CODES_TABLE_H

#include "constants.h"
#include "assembly_line_parser.h"

/**
 * @struct BinaryCodesNode
 * @brief A structure representing a binary code node.
 *
 * @var BinaryCodesNode::decAddress
 * Decimal address of the source code.
 * @var BinaryCodesNode::binaryCode
 * The binary code of the source code, length: \BINARY_CODE_LEN\.
 * @var BinaryCodesNode::sourceCode
 * The source code.
 */
typedef struct BinaryCodesNode
{
    int decAddress;
    char *binaryCode;
    char *sourceCode;
    struct BinaryCodesNode *next;
} BinaryCodesNode;

/**
 * @struct BinaryCodesTable
 * @brief A structure representing a binary codes table that holds binary codes nodes (much like linked list).
 *
 * @var BinaryCodesTable::head
 * Holds the first node in the table.
 * @var BinaryCodesTable::last
 * Holds the last node in the table.
 */
typedef struct BinaryCodesTable
{
    BinaryCodesNode *head;
    BinaryCodesNode *last;
    int length;
} BinaryCodesTable;

/* Creates a new BinaryCodesTable - allocates memory, initializes it and returns a pointer to the head */
BinaryCodesTable *createBinaryCodesTable();

/** Inserts a BinaryCodesNode at the beginning of the list
 * @return SUCCESS code or ERROR code. */
int insertToBinaryCodesTable(BinaryCodesTable *table, int decAddress, AssemblyLine *sourceLine, char *binaryCode, char *sourceCode);

/** Sorts the table by decAddress in ascending order, using bubble sort.
 * NOTE: if table is very large this function might be a bit slow. */
void sortBinaryCodesTable(BinaryCodesTable *table);

/* Function to free memory from a node */
void freeBinaryCodesNode(BinaryCodesNode *node);

/* Function to free memory allocated for the list */
void freeBinaryCodesTable(BinaryCodesTable *table);

/* Pretty print of binary codes table */
void printBinaryCodesTable(BinaryCodesTable *table);

#endif  /* BINARY_CODES_TABLE_H */