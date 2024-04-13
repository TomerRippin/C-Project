#ifndef BINARY_CODES_TABLE_H
#define BINARY_CODES_TABLE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "constants.h"
#include "assembly_line_parser.h"

/* TODO: move to a strctures folder */

typedef struct BinaryCodesNode
{
    int decAddress;
    AssemblyLine *sourceLine; /* TODO: decide maybe to insert the parsed line and not the original line */
    char *binaryCode;
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
void insertToBinaryCodesTable(BinaryCodesTable *table, int decAddress, AssemblyLine *sourceLine, char *binaryCode);

/* Function to free memory from a node */
void freeBinaryCodesNode(BinaryCodesNode *node);

/* Function to free memory allocated for the list */
void freeBinaryCodesTable(BinaryCodesTable *table);

#endif  /* BINARY_CODES_TABLE_H */