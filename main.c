//Done by Lana Batnij 1200308
//importing the libs
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//create a node Block that will hold
typedef struct Node {
    char word[30];
    struct Node *next;
    struct Node *prev;
} Node;

//a Doubly Linked List was created and it has a pointer to the head of the list
typedef struct DoublyLinkedList {
    Node *head;
} DoublyLinkedList;


/* -This function is for creating nodes
   -The Word is inserted into the node
   -And all pointer will be set to the null*/
Node *createNode( char *word) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode != NULL) {
        strcpy(newNode->word, word);
        newNode->next = NULL;
        newNode->prev = NULL;
    }
    return newNode;
}

/* -this Function works to insert the newly added Nodes to the list
   -if the head was null then we set the new node as the head
   -else the head is not null the the list will have some nodes in
   it and that means we will keep checking if the next not null till
   we reach a null next the we will add the not to it by connecting the pointers*/
void insertAtEnd(DoublyLinkedList *DLL, Node *newNode) {
    if (DLL->head == NULL) {
        DLL->head = newNode;
    } else {
        Node *current = DLL->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
        newNode->prev = current;
    }
}

/*-Here The Function will load the strings from the file
  -And will read the file line by line the take the word
  -well, at first it will create a node that will have the read word
  -then this node will be inserted to the list
  -all this happen by using the previous functions  */
void loadStrings(DoublyLinkedList *DLL) {
    FILE *file = fopen("Strings.txt", "r");
    if (file == NULL) {
        printf("Error opening The file: Strings.txt\n");
        return;
    }

    char buffer[30];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
        Node *newNode = createNode(buffer);
        if (newNode != NULL) {
            insertAtEnd(DLL, newNode);
        } else {
            printf("Memory allocation error.\n");
            break;
        }
    }

    fclose(file);
}

/* -this Function will print the words that is saved in the list
   -this is applied by simple while loop */
void printStrings(DoublyLinkedList *DLL) {
    Node *current = DLL->head;
    while (current != NULL) {
        printf("%s\n", current->word);
        current = current->next;
    }
}

//The Radix function sort that will sort the strings by:
//the least significant digit which is the Last letter
//in the String
void radixSortStrings(DoublyLinkedList *DLL) {
    //the var Max length is initialized to 0
    //the current is pointer that point to the head of the list
    int maxLength = 0;
    Node *current = DLL->head;
    /*This while loop is to find the word with the longest length in the loop*/
    while (current != NULL) {
        int length = strlen(current->word);
        if (length > maxLength) {
            maxLength = length;
        }
        current = current->next;
    }


    /*This loop iterates from the least significant digit to the most significant digit
      Bucket and Tail are arrays initialized to store the nodes and that is based on
      the current digit that we are at */
    for (int i = maxLength - 1; i >= 0; i--) {
        Node *bucket[128] = {NULL};
        Node *tail[128] = {NULL};
        Node *current = DLL->head, *next = NULL;

        // Distribute nodes into buckets
        /*- This loop will iterates at the list and at each node
          it will check the current character then will place the node
          at suitable bucket
          - the nodes will be add to the end of the Bucket*/
        while (current != NULL) {
            next = current->next;
            unsigned char letter = (unsigned char)current->word[i];

            if (bucket[letter] == NULL) {
                bucket[letter] = current;
                tail[letter] = current;
                current->next = NULL;
                current->prev = NULL;
            } else {
                tail[letter]->next = current;
                current->prev = tail[letter];
                current->next = NULL;
                tail[letter] = current;
            }

            current = next;
        }

        // Reconstruct the linked list from buckets
        /*- prevBucketEnd was used to keep eye on the
          track of the end of the previous bucket*/
        DLL->head = NULL;
        Node *BucketEnd = NULL;
        for (int j = 0; j < 128; j++) {
            if (bucket[j] != NULL) {
                if (DLL->head == NULL) {
                    DLL->head = bucket[j];
                } else {
                    BucketEnd->next = bucket[j];
                    bucket[j]->prev = BucketEnd;
                }

                BucketEnd = tail[j];
            }
        }
    }
}

/* -The last function will save the sorted list in a file
   -This was applied by a while loop that writes the words in the list to a file */
void saveToFile(Node *head) {
    FILE *file = fopen("Saved.txt", "w");
    if (file == NULL) {
        printf("Could not open the file saved.txt \n");
        return;
    }

    Node *current = head;
    while (current != NULL) {
        fprintf(file, "%s\n", current->word);
        current = current->next;
    }

    fclose(file);
}

int main() {
    //a list made in the main and its head is assigned to be null
    DoublyLinkedList DLL;
    DLL.head = NULL;
    //Choice is the var that will save the user choice from the menu
    int choice = 0;


    //A Menu was created
    printf("******************************************\n");
    printf("*******Welcome To The First Project*******\n");
    printf("******************************************\n");
    while (choice != 8) {
        printf("\n1. Load the strings\n");
        printf("2. Print the strings before sorting\n");
        printf("3. Sort the strings\n");
        printf("4. Print the sorted strings\n");
        printf("5. Add a new word to the list of sorted strings (and sort it)\n");
        printf("6. Delete a word from the sorted strings\n");
        printf("7. Save to output file\n");
        printf("8. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        // Apply the actions depending on the user choice
        if (choice == 1) {
            loadStrings(&DLL);
            printf("The Load Process was successful\n");
        } else if (choice == 2) {
            printStrings(&DLL);
        } else if (choice == 3) {
            radixSortStrings(&DLL);
            printf("The Sorting Done successfully\n");
        } else if (choice == 4) {
            printStrings(&DLL);
        } else if (choice == 5) {
            //here is to add a word to the list
            printf("Enter the new word: ");
            char word_data[30];
            scanf("%s", word_data);
            // the insert function was used then the sort function
            insertAtEnd(&DLL, createNode(word_data));
            radixSortStrings(&DLL);
            printf("The word is Added successfully\n");
        } else if (choice == 6) {
            //those actions to delete a word
            printf("Enter the word to delete: ");
            char word_data[30];
            scanf("%s", word_data);
            Node *current = DLL.head;
            //after reading the word from the user we search for the word in the list
            while (current != NULL) {
                if (strcmp(current->word, word_data) == 0) {
                    if (current->prev != NULL) {
                        current->prev->next = current->next;
                    } else {
                        DLL.head = current->next;
                    }

                    if (current->next != NULL) {
                        current->next->prev = current->prev;
                    }

                    // Free method was used to delete it from the list
                    free(current);
                    break;
                }
                current = current->next;
            }
            printf("The Word is deleted successfully\n");
        } else if (choice == 7) {
            saveToFile(DLL.head);
            printf("Saved successfully\n");
        } else if (choice == 8) {
            printf("Exiting program.\n");
            printf("BYE\n");
        } else {
            printf("Invalid choice. Please enter a number between 1 and 8.\n");
        }
    }

    return 0;
}
