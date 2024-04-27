#include "../Header Files/symbol_table.h"

void initializeList(SymbolTable *list) {
    list->head = NULL;
}

SymbolTable *createList()
{
    SymbolTable *list = (SymbolTable *)malloc(sizeof(SymbolTable));
    if (list == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    initializeList(list);
    return list;
}

SymbolNode *searchList(SymbolTable *list, char *targetName) {
    SymbolNode *current = list->head;
    while (current != NULL) {
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

SymbolNode* searchListWithType(SymbolTable *list, char *labelName, char *labelType, int toInclude){
    SymbolNode *current = list->head;
    while (current != NULL){
        if ((strcmp(current->symbolName, labelName) == 0) && (strcmp(current->symbolType, labelType) != toInclude)){
            return current;
        }
        current = current->next;
    }
    return NULL;
}

int isAlreadyExiest(SymbolTable *list, char* symbolName, char* symbolType, int symbolValue){
    SymbolNode *current = list->head;
    while(current != NULL){
        if (strcmp(current->symbolName, symbolName) == 0 && strcmp(current->symbolType, symbolType) == 0 && current->symbolValue == symbolValue){
            return 1;
        }
        current = current->next;
    }
    return 0;
}


void insertToList(SymbolTable *list, char *symbolName, char *symbolType, int symbolValue)
{
    SymbolNode *new_node;

    if(isAlreadyExiest(list, symbolName, symbolType, symbolValue)){
        logger(LOG_LEVEL_WARNING, "Trying to insert a duplicate, Not inserting");
        return;
    }

    /* Create a new node */ 
    new_node = (SymbolNode *)malloc(sizeof(SymbolNode));
    if (new_node == NULL) {
        logger(LOG_LEVEL_ERROR, "Memory allocation Failed");
        exit(EXIT_FAILURE);
    }

    /* Set symbolType and next pointer */
    new_node->symbolName = strdup(symbolName);
    new_node->symbolType = strdup(symbolType);
    new_node->symbolValue = symbolValue;
    new_node->next = list->head;

    /* Update head to point to the new node */ 
    list->head = new_node;
}

void freeNode(SymbolNode *node){
    free(node->symbolName);
    free(node->symbolType);
    free(node);
}

void freeList(SymbolTable *list) {
    SymbolNode *current = list->head;
    while (current != NULL) {
        SymbolNode *temp = current;
        current = current->next;
        freeNode(temp);
    }
    list->head = NULL;
}

void printList(SymbolTable *list)
{
    SymbolNode *current = list->head;
    printf("|    Name    |    Data    |    LineNum    |\n");
    printf("|------------|------------|---------------|\n");
    while (current != NULL)
    {
        printf("|    %s    |    %s    |       %d      |\n", current->symbolName, current->symbolType, current->symbolValue);
        current = current->next;
    }
}