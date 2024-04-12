#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <stdlib.h>

/* TODO: move to a strctures folder */

typedef struct SymbolNode
{
    char *symbolName;
    char *symbolType;
    int symbolValue;
    struct SymbolNode *next;
} SymbolNode;

typedef struct SymbolTable
{
    SymbolNode *head;
    SymbolNode *last;
} SymbolTable;

/* Creates a new SymbolTable - allocates memory, initializes it and returns a pointer to the head */
SymbolTable *createSymbolTable();

/* Inserts a SymbolNode at the beginning of the list */
void insertToSymbolTable(SymbolTable *table, char *symbolName, char *symbolType, int symbolValue);

/* Searches for a symbol name in the symbol table */
SymbolNode *searchSymbolNameInTable(SymbolTable *table, char *symbolName);

/* Function to free memory from a node */
void freeSymbolNode(SymbolNode *node);

/* Function to free memory allocated for the list */
void freeSymbolTable(SymbolTable *table);

#endif /* SYMBOL_TABLE_H */