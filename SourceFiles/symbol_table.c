#include "../HeaderFiles/symbol_table.h"

SymbolTable *createSymbolTable()
{
    SymbolTable *table = (SymbolTable *)malloc(sizeof(SymbolTable));
    if (table == NULL)
    {
        logger(LOG_LEVEL_ERROR, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    table->head = NULL;
    table->last = NULL;
    return table;
}

SymbolNode *searchSymbolNameInTable(SymbolTable *table, char *targetName)
{
    SymbolNode *current = table->head;
    while (current != NULL)
    {
        if (strcmp(current->symbolName, targetName) == 0)
        {
            /* Found the target, return the node */
            return current;
        }
        current = current->next;
    }
    /* Target not found */
    return NULL;
}

SymbolNode *searchSymbolTableWithType(SymbolTable *table, char *labelName, char *labelType, int toInclude)
{
    SymbolNode *current = table->head;
    while (current != NULL)
    {
        if ((strcmp(current->symbolName, labelName) == 0) && (strcmp(current->symbolType, labelType) != toInclude))
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

SymbolNode *searchSymbolNameTypeInTable(SymbolTable *table, char *symbolName, char *symbolType)
{
    SymbolNode *current = table->head;
    while (current != NULL)
    {
        if (strcmp(current->symbolName, symbolName) == 0)
        {
            if (strcmp(current->symbolType, symbolType) == 0)
            {
                /* Found the target, return the node */
                return current;
            }
        }
        current = current->next;
    }
    /* Target not found */
    return NULL;
}

int isSymbolAlreadyExist(SymbolTable *table, char *symbolName, char *symbolType, int symbolValue)
{
    SymbolNode *current = table->head;
    while (current != NULL)
    {
        if (strcmp(current->symbolName, symbolName) == 0 && strcmp(current->symbolType, symbolType) == 0 && current->symbolValue == symbolValue)
        {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void sortSymbolTable(SymbolTable *table)
{
    SymbolNode *node;
    int swapped;

    if (table->head == NULL || table->head->next == NULL)
    {
        return; /* No need to sort empty or single-node lists */
    }

    do {
        swapped = 0;
        node = table->head;

        while (node->next != NULL)
        {
            /* If current node's value is greater than the next node's value */
            if (node->symbolValue > node->next->symbolValue)
            {
                /* Swap node values */
                char *tempName = node->symbolName;
                char *tempType = node->symbolType;
                int tempValue = node->symbolValue;

                node->symbolName = node->next->symbolName;
                node->symbolType = node->next->symbolType;
                node->symbolValue = node->next->symbolValue;

                node->next->symbolName = tempName;
                node->next->symbolType = tempType;
                node->next->symbolValue = tempValue;

                swapped = 1; /* Set swapped flag to true */
            }
            node = node->next;
        }

        table->last = node;

    } while (swapped);
     /* Repeat if any values were swapped in the last pass */
}

void insertToSymbolTable(SymbolTable *table, char *symbolName, char *symbolType, int symbolValue)
{
    SymbolNode *newNode;

    if (isSymbolAlreadyExist(table, symbolName, symbolType, symbolValue))
    {
        return;
    }

    /* Create a new node */ 
    newNode = (SymbolNode *)malloc(sizeof(SymbolNode));
    if (newNode == NULL) {
        logger(LOG_LEVEL_ERROR, "Memory allocation Failed");
        exit(EXIT_FAILURE);
    }

    /* Set symbolType and next pointer */
    newNode->symbolName = strdup(symbolName);
    newNode->symbolType = strdup(symbolType);
    newNode->symbolValue = symbolValue;
    newNode->next = NULL;

    if (table->head == NULL)
    {
        table->head = newNode;
    }
    if (table->last != NULL)
    {
        table->last->next = newNode;
    }
    table->last = newNode;

    logger(LOG_LEVEL_DEBUG, "Inserted to symbolTable: symbol name - <%s> | symbol type - <%d> | symbol value: <%d>", symbolName, symbolType, symbolValue);
}

void freeSymbolNode(SymbolNode *node){
    free(node->symbolName);
    free(node->symbolType);
    free(node);
}

void freeSymbolTable(SymbolTable *table) {
    SymbolNode *current = table->head;
    SymbolNode *tempNodeToFree;
    while (current != NULL)
    {
        tempNodeToFree = current;
        current = current->next;
        freeSymbolNode(tempNodeToFree);
    }
    table->head = NULL;
    table->last = NULL;
}

void printSymbolTable(SymbolTable *table)
{
    SymbolNode *node = table->head;
    printf("-------------------------------------------------------\n");
    printf("| %-15s | %-15s | %-15s |\n", "Symbol Value", "Symbol Type", "Symbol Name");
    printf("|-----------------|-----------------|-----------------|\n");
    while (node != NULL)
    {
        printf("| %04d%-11s | %-15s | %-15s |\n", node->symbolValue, "", node->symbolName, node->symbolType);
        node = node->next;
    }
    printf("-------------------------------------------------------\n");
}