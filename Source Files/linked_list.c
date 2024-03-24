#include <stdio.h>
#include <stdlib.h>
#include "../Header Files/linked_list.h"


void initializeList(LinkedList *list) {
    list->head = NULL;
}

ListNode *searchList(const LinkedList *list, const char *target_name) {
    ListNode *current = list->head;
    while (current != NULL) {
        if (strcmp(current->name, target_name) == 0) {
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
        fprintf(stderr, "Internal Error"); // TODO: assign an Error Code
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
    new_node->data = data;
    new_node->line_number = line_number;
    new_node->name = name;
    new_node->next = list->head;

    /* Update head to point to the new node */ 
    list->head = new_node;
}

void freeNode(ListNode *node){
    /* Free memory allocated for the name, content and node */
    free(node->name);
    free(node->data);
    free(node);
}

void freeList(LinkedList *list) {
    ListNode *current = list->head;
    while (current != NULL) {
        ListNode *temp = current;
        current = current->next;
        /* Free the node itself */
        freeNode(temp);
    }
    /* Set head to NULL to indicate an empty list */
    list->head = NULL;
}

