// Define a structure for the linked list node
typedef struct ListNode {
    char *name; // Name associated with the Node
    char *data; // The data stored in the Node  
    int line_number; // Line number of the Node
    struct ListNode *next; // Pointer to the next node
} ListNode;

// Define a structure for the linked list
typedef struct {
    ListNode *head;
} LinkedList;

// Function to initialize an empty linked list
void initialize_list(LinkedList *list);

// Function to insert data at the beginning of the list
void insert_to_list(LinkedList *list, char *name, char *data, int line_number);

// Function to search for data in the list
ListNode *search_list(const LinkedList *list, const char *target_name, int *found);

// Function to free memory allocated for the list
void free_list(LinkedList *list, void (*free_data)(void *));

// Function to free memory from a node
void free_node(ListNode *node);

