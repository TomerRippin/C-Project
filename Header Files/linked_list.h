#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** TODO: change line_number to type, in order to use in the symbol table 
 * for now:
 * name = symbolName
 * data = symbolType
 * line_number = symbolValue
*/
typedef struct SymbolNode
{
    char *symbolName;            /* Symbol name */
    char *symbolType;            /* Symbol Value */
    int symbolValue;             /* Line number of the Node  - TODO: decide if needed*/
    struct SymbolNode *next;     /* Pointer to the next node */
};

/* Defines a structure for the linked list node */ 
typedef struct ListNode {
    char *name;            /* Name associated with the Node */
    char *data;            /* The data stored in the Node */
    int line_number;       /* Line number of the Node  - TODO: decide if needed and change to titled*/
    struct ListNode *next; /* Pointer to the next node */
} ListNode;

/* Defines a structure for the linked list */
typedef struct LinkedList {
    ListNode *head;
} LinkedList;

/** Creates a new LinkedList - allocates memory, initializes it and returns a pointer to the head
 * @warning If memory allocation fails, the program will terminate with an error message.
 */
LinkedList *createList();

/** Initializes an empty linked list 
 * TODO: consider delete this
 */
void initializeList(LinkedList *list);

/* Inserts a node at the beginning of the list */ 
void insertToList(LinkedList *list, char *name, char *data, int line_number);

/* Searches for a node name in the linked list */ 
ListNode *searchList(LinkedList *list, char *target_name);

/* Function to free memory allocated for the list */
void freeList(LinkedList *list);

/* Function to free memory from a node */
void freeNode(ListNode *node);

/* Iterates over the nodes of the Link and prints them */
/* TODO: consider deleting this */
void printList(LinkedList *list);
