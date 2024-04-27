#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

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

/** Creates a new SymbolTable - allocates memory, initializes it and returns a pointer to the head
  *@warning If memory allocation fails, the program will terminate with an error message. */
SymbolTable *createSymbolTable();

/* Inserts a SymbolNode at the beginning of the table */
void insertToSymbolTable(SymbolTable *table, char *symbolName, char *symbolType, int symbolValue);

/* Searches a symbol in the symbol table by name */
SymbolNode *searchSymbolNameInTable(SymbolTable *table, char *symbolName);

SymbolNode* searchListWithType(SymbolTable *table, char *labelName, char *labelType, int toInclude);

/* Searches a symbol in the symbol table by name AND type */
SymbolNode *searchSymbolNameTypeInTable(SymbolTable *table, char *symbolName, char *symbolType);

int isAlreadyExist(SymbolTable *table, char *symbolName, char *symbolType, int symbolValue);

/** Sorts the table by symbolValue in ascending order, using bubble sort.
 * NOTE: if table is very large this function might be a bit slow. */
void sortSymbolTableByValue(SymbolTable *table);

/* Function to free memory from a node */
void freeSymbolNode(SymbolNode *node);

/* Function to free memory allocated for the table */
void freeSymbolTable(SymbolTable *table);

/* Pretty print of symbol table */
void printSymbolTable(SymbolTable *table);

#endif /* SYMBOL_TABLE_H */