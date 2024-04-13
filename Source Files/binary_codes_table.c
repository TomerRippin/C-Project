#include "binary_codes_table.h"

BinaryCodesTable *createBinaryCodesTable()
{
    BinaryCodesTable *table = (BinaryCodesTable *)malloc(sizeof(BinaryCodesTable));
    if (table == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    table->head = NULL;
    table->last = NULL;
    table->length = 0;
    return table;
}

void insertToBinaryCodesTable(BinaryCodesTable *table, int decAddress, AssemblyLine *sourceLine, char *binaryCode)
{
    BinaryCodesNode *newNode = (BinaryCodesNode *)malloc(sizeof(BinaryCodesNode));
    /* TODO: is it really needed? maybe were Magzimim */
    if (newNode == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    newNode->decAddress = decAddress;
    newNode->sourceLine = sourceLine;
    newNode->binaryCode = binaryCode;
    newNode->next = NULL;
    printAssemblyLine(newNode->sourceLine);
    if (table->head == NULL) {
        table->head = newNode;
    }
    if (table->last != NULL) {
        table->last->next = newNode;
    }
    table->last = newNode;
    table->length++;
}

void freeBinaryCodesNode(BinaryCodesNode *node)
{
    free(node->sourceLine);
    free(node->binaryCode);
    free(node);
}

/* Function to free memory allocated for the list */
void freeBinaryCodesTable(BinaryCodesTable *table) {
    BinaryCodesNode *current = table->head;
    BinaryCodesNode *tempNodeToFree;
    while (current != NULL)
    {
        tempNodeToFree = current;
        current = current->next;
        freeBinaryCodesNode(tempNodeToFree);
    }
    table->head = NULL;
    table->last = NULL;
    free(table);
}