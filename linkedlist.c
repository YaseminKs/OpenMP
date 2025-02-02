#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Define a node structure
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Function to create a new node
Node* create_node( int data ){
    Node* new_node = ( Node* )malloc( sizeof( Node ) );
    new_node -> data = data;
    new_node -> next = NULL;
    return new_node;
}

// Function to append a node to the end of the list
void append( Node** head, int data ){
    Node* new_node = create_node( data );
    if( *head == NULL ){
        *head = new_node;
        return;
    }
    Node* temp = *head;
    while( temp -> next != NULL ){
        temp = temp -> next;
    }
    temp -> next = new_node;
}

// Function to print the linked list
void print_list( Node* head ){
    Node* temp = head;
    while( temp != NULL ){
        printf( "%d -> ", temp -> data );
        temp = temp->next;
    }
    printf( "NULL\n" );
}

// Function to process each node in parallel (e.g., multiply data by 2)
void process_list( Node* head ){
    Node* temp = head;
    int count = 0;

    // First, count the number of nodes
    while( temp != NULL ){
        count++;
        temp = temp -> next;
    }

    Node* node_array[ count ];
    temp = head;
    int index = 0;

    // Store pointers in an array for parallel access
    while( temp != NULL ){
        node_array[ index++ ] = temp;
        temp = temp -> next;
    }

    // Process nodes in parallel using OpenMP
    #pragma omp parallel for
    for( int i = 0 ; i < count ; i++ ){
        node_array[ i ] -> data *= 2;  // Example operation: multiply by 2
    }
}

// Function to free the linked list
void free_list( Node* head ){
    Node* temp;
    while( head != NULL ){
        temp = head;
        head = head->next;
        free( temp );
    }
}

int main(){
    Node* head = NULL;

    // Create a linked list with some values
    for( int i = 1 ; i <= 10 ; i++ ){
        append( &head, i );
    }

    printf( "Original List:\n" );
    print_list( head );

    // Process the list in parallel
    process_list( head );

    printf( "Processed List (Each value multiplied by 2):\n" );
    print_list( head );

    // Free memory
    free_list( head );

    return 0;
}
