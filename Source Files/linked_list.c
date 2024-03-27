#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"


void initializeList(LinkedList *list) {
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

ListNode *searchList(LinkedList *list, char *targetName) {
    ListNode *current = list->head;
    while (current != NULL) {
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


void insertToList(LinkedList *list, char *name, char *data, int line_number) 
{
    ListNode *searchResult;

    searchResult = searchList(list, name);

    if(searchResult != NULL && strcmp(searchResult->data, data) != 0){
        fprintf(stderr, "Internal Error"); /* TODO: assign an Error Code */
        free(name);
        free(data);
        return;
    }

    /* Create a new node */ 
    ListNode *new_node = (ListNode *)malloc(sizeof(ListNode));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    /* Set data and next pointer */
    new_node->name = strdup(name);
    new_node->data = strdup(data);
    new_node->line_number = line_number;
    new_node->next = list->head;

    /* Update head to point to the new node */ 
    list->head = new_node;
}

void freeNode(ListNode *node){
    free(node->name);
    free(node->data);
    free(node);
}

void freeList(LinkedList *list) {
    ListNode *current = list->head;
    while (current != NULL) {
        ListNode *temp = current;
        current = current->next;
        freeNode(temp);
    }
    list->head = NULL;
}

void printList(LinkedList *list)
{
    ListNode *current = list->head;
    while (current != NULL)
    {
        printf("Name: %s\nData: %s\n", current->name, current->data);
        current = current->next;
    }
}