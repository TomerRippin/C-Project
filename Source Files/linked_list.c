#include "../Header Files/linked_list.h"

void initializeList(LinkedList *list)
{
    list->head = NULL;
}

LinkedList *createList()
{
    LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
    if (list == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    initializeList(list);
    return list;
}

ListNode *searchList(LinkedList *list, char *targetName)
{
    ListNode *current = list->head;
    while (current != NULL)
    {
        if (strcmp(current->name, targetName) == 0)
        {
            /* Found the target, return the node */
            return current;
        }
        current = current->next;
    }
    /* Target not found */
    return NULL;
}

ListNode *searchListWithType(LinkedList *list, char *labelName, char *labelType, int toInclude)
{
    ListNode *current = list->head;
    while (current != NULL)
    {
        if ((strcmp(current->name, labelName) == 0) && (strcmp(current->data, labelType) != toInclude))
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

int isAlreadyExist(LinkedList *list, char *name, char *data, int lineNumber)
{
    ListNode *current = list->head;
    while (current != NULL)
    {
        if (strcmp(current->name, name) == 0 && strcmp(current->data, data) == 0 && current->lineNumber == lineNumber)
        {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void insertToList(LinkedList *list, char *name, char *data, int lineNumber)
{
    ListNode *new_node;

    if (isAlreadyExist(list, name, data, lineNumber))
    {
        logger(LOG_LEVEL_WARNING, "Trying to insert a duplicate, Not inserting");
        return;
    }

    /* Create a new node */
    new_node = (ListNode *)malloc(sizeof(ListNode));
    if (new_node == NULL)
    {
        logger(LOG_LEVEL_ERROR, "Memory allocation Failed");
        exit(EXIT_FAILURE);
    }

    /* Set data and next pointer */
    new_node->name = strdup(name);
    new_node->data = strdup(data);
    new_node->lineNumber = lineNumber;
    new_node->next = list->head;

    /* Update head to point to the new node */
    list->head = new_node;
}

void freeNode(ListNode *node)
{
    free(node->name);
    free(node->data);
    free(node);
}

void freeList(LinkedList *list)
{
    ListNode *current = list->head;
    while (current != NULL)
    {
        ListNode *temp = current;
        current = current->next;
        freeNode(temp);
    }
    list->head = NULL;
}

void printList(LinkedList *list)
{
    ListNode *current = list->head;
    printf("|    Name    |    Data    |    LineNum    |\n");
    printf("|------------|------------|---------------|\n");
    while (current != NULL)
    {
        printf("|    %s    |    %s    |       %d      |\n", current->name, current->data, current->lineNumber);
        current = current->next;
    }
}