#include "symbol_table.h"

/* TODO: move to a strctures folder */

SymbolTable *createSymbolTable()
{
    SymbolTable *table = (SymbolTable *)malloc(sizeof(SymbolTable));
    if (table == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    table->head = NULL;
    return table;
}

void insertToSymbolTable(SymbolTable *table, char *symbolName, char *symbolType, int symbolValue)
{
    SymbolNode *newNode = (SymbolNode *)malloc(sizeof(SymbolNode));
    /* TODO: is it really needed? maybe were Magzimim */
    if (newNode == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strcpy(newNode->symbolName, symbolName);
    strcpy(newNode->symbolValue, symbolValue);
    newNode->symbolValue = symbolValue;
    newNode->next = NULL;

    if (table->head == NULL) {
        table->head = newNode;
    }
    if (table->last != NULL) {
        table->last->next = newNode;
    }
    table->last = newNode;
}

void freeSymbolNode(SymbolNode *node)
{
    free(node->symbolName);
    free(node->symbolType);
    free(node);
}

/* Function to free memory allocated for the list */
void freeSymbolTable(SymbolTable *table) {
    SymbolNode *current = table->head;
    SymbolNode *tempNodeToFree;
    while (current != NULL)
    {
        tempNodeToFree = current;
        current = current->next;
        freeNode(tempNodeToFree);
    }
    table->head = NULL;
    table->last = NULL;
    free(table);
}