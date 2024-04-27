#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

/* Defines a structure for the linked list node */ 
typedef struct SymbolNode {
    char *symbolName;            /* Name associated with the Node */
    char *symbolType;            /* The data stored in the Node */
    int symbolValue;       /* Line number of the Node  - TODO: decide if needed and change to titled*/
    struct SymbolNode *next; /* Pointer to the next node */
} SymbolNode;

/* Defines a structure for the linked list */
typedef struct SymbolTable {
    SymbolNode *head;
} SymbolTable;

/** Creates a new SymbolTable - allocates memory, initializes it and returns a pointer to the head
 * @warning If memory allocation fails, the program will terminate with an error message.
 */
SymbolTable *createList();

/** Initializes an empty linked list 
 * TODO: consider delete this
 */
void initializeList(SymbolTable *list);

/* Inserts a node at the beginning of the list */ 
void insertToList(SymbolTable *list, char *symbolName, char *symbolType, int symbolValue);

/* Searches for a node symbolName in the linked list */ 
SymbolNode *searchList(SymbolTable *list, char *target_symbolName);

SymbolNode* searchListWithType(SymbolTable *list, char *labelName, char *labelType, int toInclude);

/* Searches a symbol in symbol table by symbolName and type
SymbolNode *searchSymbolNameTypeInTable(SymbolTable *table, char *symbolName, char *symbolType);

** Sorts the table by symbolValue in ascending order, using bubble sort.
 * NOTE: if table is very large this function might be a bit slow.
void sortSymbolTableByValue(SymbolTable *table); */

/* Function to free memory from a node */
void freeNode(SymbolNode *node);

/* Function to free memory allocated for the list */
void freeList(SymbolTable *list);

/* Iterates over the nodes of the Link and prints them */
/* TODO: consider deleting this */
void printList(SymbolTable *list);

#endif /* SYMBOL_TABLE_H */