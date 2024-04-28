#ifndef BINARY_CODES_TABLE_H
#define BINARY_CODES_TABLE_H

#include "constants.h"
#include "assembly_line_parser.h"

/* TODO: move to a strctures folder */

typedef struct BinaryCodesNode
{
    int decAddress;
    /* AssemblyLine *sourceLine; */  /* TODO: maybe delete */
    char *binaryCode;
    char *sourceCode; /* TODO: used for pretty prints as in the book, and maybe can replace usage of sourceLine? */
    struct BinaryCodesNode *next;
} BinaryCodesNode;

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

/* Pretty print of binary codes list */
void printBinaryList(BinaryCodesTable *list);

#endif  /* BINARY_CODES_TABLE_H */