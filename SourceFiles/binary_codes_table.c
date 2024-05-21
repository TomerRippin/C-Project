#include "../HeaderFiles/binary_codes_table.h"

BinaryCodesTable *createBinaryCodesTable()
{
    BinaryCodesTable *table = (BinaryCodesTable *)malloc(sizeof(BinaryCodesTable));
    if (table == NULL)
    {
        logger(LOG_LEVEL_ERROR, "Memory allocation failed\n");
        exit(GENERAL_ERROR);
    }
    table->head = NULL;
    table->last = NULL;
    table->length = 0;
    return table;
}

int insertToBinaryCodesTable(BinaryCodesTable *table, int decAddress, AssemblyLine *sourceLine, char *binaryCode, char *sourceCode)
{
    BinaryCodesNode *newNode = (BinaryCodesNode *)malloc(sizeof(BinaryCodesNode));
    /* TODO: is it really needed? maybe were Magzimim */
    if (newNode == NULL)
    {
        logger(LOG_LEVEL_ERROR, "Memory allocation failed\n");
        exit(GENERAL_ERROR);
    }
    if (binaryCode == NULL || sourceCode == NULL)
    {
        return GENERAL_ERROR;
    }

    newNode->binaryCode = malloc(strlen(binaryCode) + 1);
    strcpy(newNode->binaryCode, binaryCode);

    newNode->sourceCode = malloc(strlen(sourceCode) + 1);
    strcpy(newNode->sourceCode, sourceCode);

    /*
    logger(LOG_LEVEL_DEBUG, "3");
    newNode->sourceLine = malloc(sizeof(AssemblyLine));
    memcpy(newNode->sourceLine, sourceLine, sizeof(AssemblyLine));
    */

    newNode->decAddress = decAddress;

    newNode->next = NULL;
    if (table->head == NULL) {
        table->head = newNode;
    }
    if (table->last != NULL) {
        table->last->next = newNode;
    }
    table->last = newNode;
    table->length++;

    logger(LOG_LEVEL_DEBUG, "Inserted to binaryCodesTable: source code - <%s> | binary code - <%s> | dec address: <%d>", sourceCode, binaryCode, decAddress);

    return SUCCESS;
}

void sortBinaryCodesTable(BinaryCodesTable *table)
{
    BinaryCodesNode *node;
    int swapped;
    
    if (table->head == NULL || table->head->next == NULL)
    {
        return; /* No need to sort empty or single-node lists */
    }

    do
    {
        swapped = 0;
        node = table->head;

        while (node->next != NULL)
        {
            /* If current node's dec address is greater than the next node's value */
            if (node->decAddress > node->next->decAddress)
            {
                /* Swap node values */
                char *tempBinary = node->binaryCode;
                char *tempSource = node->sourceCode;
                int tempAddr = node->decAddress;

                node->binaryCode = node->next->binaryCode;
                node->sourceCode = node->next->sourceCode;
                node->decAddress = node->next->decAddress;

                node->next->binaryCode = tempBinary;
                node->next->sourceCode = tempSource;
                node->next->decAddress = tempAddr;

                swapped = 1; /* Set swapped flag to true */
            }
            node = node->next;
        }

        table->last = node;

    } while (swapped); /* Repeat if any values were swapped in the last pass */
}

void freeBinaryCodesNode(BinaryCodesNode *node)
{
    free(node->binaryCode);
    free(node->sourceCode);
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

void printBinaryCodesTable(BinaryCodesTable *table)
{
    BinaryCodesNode *current = table->head;
    printf("--------------------------------------------\n");
    printf("| Addr | %-15s | %-15s |\n", "Binary Code", "Source Code");
    printf("|------|-----------------|-----------------|\n");
    while (current != NULL)
    {
        printf("| %04d | %-15s | %-15s |\n", current->decAddress, current->binaryCode, current->sourceCode);
        current = current->next;
    }
    printf("--------------------------------------------\n");
}