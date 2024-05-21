#ifndef MACROS_LIST_H
#define MACROS_LIST_H

#include "utils.h"

typedef struct MacroNode
{
    char *name;            /* Name associated with the Node */
    char *data;            /* The data stored in the Node */
    struct MacroNode *next; /* Pointer to the next node */
} MacroNode;

typedef struct MacrosList
{
    MacroNode *head;
} MacrosList;

/** Creates a new MacrosList - allocates memory, initializes it and returns a pointer to the head
 * @warning If memory allocation fails, the program will terminate with an error message. */
MacrosList *createMacrosList();

/* Inserts a node at the beginning of the list */
void insertToMacrosList(MacrosList *list, char *name, char *data);

/* Searches for a node name in the linked list */
MacroNode *searchMacrosList(MacrosList *list, char *target_name);

/* Function to free memory from a node */
void freeMacroNode(MacroNode *node);

/* Function to free memory allocated for the list */
void freeMacrosList(MacrosList *list);

/* Iterates over the nodes of the Link and prints them */
void printMacrosList(MacrosList *list);

#endif /* MACROS_LIST_H */