#include <stdio.h>
#include <stdlib.h>
#include "../Header Files/linked_list.h"


void initialize_list(LinkedList *list) {
    list->head = NULL;
}

ListNode *search_list(const LinkedList *list, const char *target_name, int *found) {
    ListNode *current = list->head;
    while (current != NULL) {
        if (strcmp(current->name, target_name) == 0) {
            // Found the target, return the node
            *found = 1;
            return current;
        }
        current = current->next;
    }
    // Target not found
    return NULL;
}


void insert_to_list(LinkedList *list, char *name, char *data, int line_number) 
{
    int found;
    found=0;
    ListNode *search_result;

    search_result = search_list(list, name, &found);

    if(found && strcmp(search_result->data, data) != 0){
        fprintf(stderr, "Internal Error"); // TODO: assign an Error Code
        free(name);
        free(data);
        return;
    }

    // Create a new node
    ListNode *new_node = (ListNode *)malloc(sizeof(ListNode));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Set data and next pointer
    new_node->data = data;
    new_node->line_number = line_number;
    new_node->name = name;
    new_node->next = list->head;

    // Update head to point to the new node
    list->head = new_node;
}

void free_node(ListNode *node){
    // Free memory allocated for the name, content and node
    free(node->name);
    free(node->data);
    free(node);
}

// Function to free memory allocated for the list
void free_list(LinkedList *list, void (*free_data)(void *)) {
    ListNode *current = list->head;
    while (current != NULL) {
        ListNode *temp = current;
        current = current->next;
        // Free the data associated with the node
        free_data(temp->data);
        // Free the node itself
        free(temp);
    }
    // Set head to NULL to indicate an empty list
    list->head = NULL;
}
