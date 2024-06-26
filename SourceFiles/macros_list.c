#include "../HeaderFiles/macros_list.h"

MacrosList *createMacrosList()
{
    MacrosList *list;

    list = (MacrosList *)malloc(sizeof(MacrosList));
    if (list == NULL)
    {
        logger(LOG_LEVEL_ERROR, "\x1b[1m%s (%d) ", getErrorMessage(ERROR_MEMORY_ALLOC_FAILED), ERROR_MEMORY_ALLOC_FAILED);
        exit(ERROR_MEMORY_ALLOC_FAILED);
    }

    list->head = NULL;
    return list;
}

MacroNode *searchMacrosList(MacrosList *list, char *targetName)
{
    MacroNode *current;

    current = list->head;
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

int isAlreadyExist(MacrosList *list, char *name, char *data)
{
    MacroNode *current;

    current = list->head;
    while (current != NULL)
    {
        if (strcmp(current->name, name) == 0 && strcmp(current->data, data) == 0)
        {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void insertToMacrosList(MacrosList *list, char *name, char *data)
{
    MacroNode *new_node;

    if (isAlreadyExist(list, name, data))
    {
        return;
    }

    /* Creates a new node */
    new_node = (MacroNode *)malloc(sizeof(MacroNode));
    if (new_node == NULL)
    {
        logger(LOG_LEVEL_ERROR, "\x1b[1m%s (%d) ", getErrorMessage(ERROR_MEMORY_ALLOC_FAILED), ERROR_MEMORY_ALLOC_FAILED);
        exit(ERROR_MEMORY_ALLOC_FAILED);
    }

    /* Set data and next pointer */
    new_node->name = strdup(name);
    new_node->data = strdup(data);
    new_node->next = list->head;

    /* Update head to point to the new node */
    list->head = new_node;
}

void freeMacroNode(MacroNode *node)
{
    if (node != NULL)
    {
        free(node->name);
        free(node->data);
        free(node);
    }
}

void freeMacrosList(MacrosList *list)
{
    if (list != NULL)
    {
        MacroNode *current;

        current = list->head;
        while (current != NULL)
        {
            MacroNode *temp = current;
            current = current->next;
            freeMacroNode(temp);
        }
        list->head = NULL;
    }
}

void printMacrosList(MacrosList *list)
{
    MacroNode *current;

    current = list->head;
    printf("---------------------------\n");
    printf("|    Name    |    Data    |\n");
    printf("|------------|------------|\n");
    while (current != NULL)
    {
        printf("|    %s    |    %s    |\n", current->name, current->data);
        current = current->next;
    }
    printf("---------------------------\n");
}