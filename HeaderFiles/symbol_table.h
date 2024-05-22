#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "utils.h"

/**
 * @struct SymbolNode
 * @brief A structure representing a symbol node.
 *
 * A valid symbol in this assembly language starts with an alphabetic letter (big or small),
 * followed by alphabetic letters or numbers, and its maximum length is 31.
 *
 * @var SymbolNode::symbolName
 * Decimal address of the source code.
 * @var SymbolNode::symbolType
 * Symbol type, can be one of the following:
 * - SYMBOL_TYPE_MDEFINE
 * - SYMBOL_TYPE_DATA
 * - SYMBOL_TYPE_EXTERNAL
 * - SYMBOL_TYPE_ENTRY
 * - SYMBOL_TYPE_CODE
 * - SYMBOL_TYPE_EXTERNAL_USAGE
 * @var SymbolNode::symbolValue
 * Holds the address of the symbol.
 */
typedef struct SymbolNode
{
    char *symbolName;
    char *symbolType;
    int symbolValue;
    struct SymbolNode *next;
} SymbolNode;

/**
 * @struct SymbolTable
 * @brief A structure representing a symbol table that holds symbol nodes (much like linked list).
 *
 * @var SymbolTable::head
 * Holds the first node in the table.
 * @var SymbolTable::last
 * Holds the last node in the table.
 */
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

/** Searches a symbol in the symbol table by name and by type
* @param toInclude whether to search for the exact type or all of the types that are not the type insertd to the function */
SymbolNode* searchSymbolTableWithType(SymbolTable *table, char *labelName, char *labelType, int toInclude);

/* Searches a symbol in the symbol table by name AND type */
SymbolNode *searchSymbolNameTypeInTable(SymbolTable *table, char *symbolName, char *symbolType);

int isSymbolAlreadyExist(SymbolTable *table, char *symbolName, char *symbolType, int symbolValue);

/** Sorts the table by symbolValue in ascending order, using bubble sort.
 * NOTE: if table is very large this function might be a bit slow. */
void sortSymbolTable(SymbolTable *table);

/* Function to free memory from a node */
void freeSymbolNode(SymbolNode *node);

/* Function to free memory allocated for the table */
void freeSymbolTable(SymbolTable *table);

/* Pretty print of symbol table */
void printSymbolTable(SymbolTable *table);

#endif /* SYMBOL_TABLE_H */