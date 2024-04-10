#ifndef BINARY_CODES_TABLE_H
#define BINARY_CODES_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include "constants.h"

typedef struct BinaryCodesNode
{
    int decAddress;
    char *sourceLine;
    char *binaryCode[BINARY_CODE_LEN];
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

/* Inserts a BinaryCodesNode at the beginning of the list */
void insertToBinaryCodesTable(BinaryCodesTable *table, int decAddress, char *sourceLine, char *binaryCode);

/* Function to free memory from a node */
void freeBinaryCodesNode(BinaryCodesNode *node);

/* Function to free memory allocated for the list */
void freeBinaryCodesTable(BinaryCodesTable *table);

#endif  /* BINARY_CODES_TABLE_H */