/* Define a structure for the linked list node */ 
typedef struct ListNode {
    char *name;            /* Name associated with the Node */
    char *data;            /* The data stored in the Node */
    int line_number;       /* Line number of the Node */
    struct ListNode *next; /* Pointer to the next node */
} ListNode;

/* Define a structure for the linked list */ 
typedef struct {
    ListNode *head;
} LinkedList;

/* Function to initialize an empty linked list */
void initializeList(LinkedList *list);

/* Function to insert a node at the beginning of the list */ 
void insertToList(LinkedList *list, char *name, char *data, int line_number);

/* Function to search for a node name in the linked list */ 
ListNode *searchList(LinkedList *list, char *target_name);

/* Function to free memory allocated for the list */
void freeList(LinkedList *list);

/* Function to free memory from a node */
void freeNode(ListNode *node);
