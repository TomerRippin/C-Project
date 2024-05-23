#include "../HeaderFiles/binary_codes_table.h"

BinaryCodesTable *createBinaryCodesTable()
{
    BinaryCodesTable *table;
    table = (BinaryCodesTable *)malloc(sizeof(BinaryCodesTable));
    if (table == NULL)
    {
        logger(LOG_LEVEL_ERROR, "\x1b[1m%s (%d)\x1b[0m", getErrorMessage(ERROR_MEMORY_ALLOC_FAILED), ERROR_MEMORY_ALLOC_FAILED);
        exit(ERROR_MEMORY_ALLOC_FAILED);
    }
    table->head = NULL;
    table->last = NULL;
    table->length = 0;
    return table;
}

int insertToBinaryCodesTable(BinaryCodesTable *table, int decAddress, AssemblyLine *sourceLine, char *binaryCode, char *sourceCode)
{
    BinaryCodesNode *newNode;
    newNode = (BinaryCodesNode *)malloc(sizeof(BinaryCodesNode));

    if (newNode == NULL)
    {
        logger(LOG_LEVEL_ERROR, "\x1b[1m%s (%d)\x1b[0m", getErrorMessage(ERROR_MEMORY_ALLOC_FAILED), ERROR_MEMORY_ALLOC_FAILED);
        exit(ERROR_MEMORY_ALLOC_FAILED);
    }

    /* Shouldn't happen but just in case */
    if (binaryCode == NULL || sourceCode == NULL)
    {
        return GENERAL_ERROR;
    }

    newNode->binaryCode = malloc(strlen(binaryCode) + 1);
    strcpy(newNode->binaryCode, binaryCode);

    newNode->sourceCode = malloc(strlen(sourceCode) + 1);
    strcpy(newNode->sourceCode, sourceCode);

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
    if (node != NULL)
    {
        free(node->binaryCode);
        free(node->sourceCode);
        free(node);
        node = NULL;
    }
}

/* Function to free memory allocated for the list */
void freeBinaryCodesTable(BinaryCodesTable *table) {
    if (table != NULL)
    {
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
    }
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