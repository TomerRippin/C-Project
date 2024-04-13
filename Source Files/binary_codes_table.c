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
    printf("DEBUG - in InsertToBinary\n");
    newNode->binaryCode = malloc(strlen(binaryCode) + 1);
    printf("DEBUG - malloc binary \n");
    strcpy(newNode->binaryCode, binaryCode);
    printf("DEBUG - assign binary \n");
    newNode->sourceLine = malloc(sizeof(AssemblyLine));
    printf("DEBUG - malloc assembly line \n");
    memcpy(newNode->sourceLine, sourceLine, sizeof(AssemblyLine));
    printf("DEBUG - assign assembly \n");
    newNode->decAddress = decAddress;
    /*newNode->sourceLine = sourceLine;
    newNode->binaryCode = binaryCode; */
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
    printf("DEBUG - operands in list = %s \n\n", table->last->sourceLine->operands);
    
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

void printBinaryList(BinaryCodesTable *list)
{
    BinaryCodesNode *current = list->head;
    printf("|    dec    |    binary code    |    operands    |\n");
    printf("|------------|------------|---------------|\n");
    while (current != NULL)
    {
        printf("|    %d    |    %s    |       %s      |\n", current->decAddress, current->binaryCode, current->sourceLine->operands);
        current = current->next;
    }
}