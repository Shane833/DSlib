#include <ForwardList.h>

// Utility function for creating a node of the list with specified value
static ForwardListNode *ForwardListNode_create(void *data){
    ForwardListNode *node = malloc(sizeof(ForwardListNode));
    check(node != NULL, "Failed to create ForwardList Node!");

    node->data = data; 
    node->next = NULL;

    return node;
error:
    return NULL;
}

ForwardList *ForwardList_create(){
    ForwardList *list = malloc(sizeof(ForwardList));
    check(list != NULL, "Failed to create ForwardList!");
    
    list->head = NULL;
    list->count = 0;

    return list;
error:
    return NULL;
}

void ForwardList_clear(ForwardList *list){
    if(list){
        if(list->count){
            FORWARDLIST_FOREACH(list, cur){
                free(cur->data); // deallocating data
            }
        }
    }
error: // Fallthrough
    return;
}

void ForwardList_destroy(ForwardList *list){
   if(list){
       if(list->count != 0){
            ForwardListNode *prev = NULL;
            ForwardListNode *cur = NULL; 

            prev = list->head; // Initially points to the head
            if(prev->next){
                cur = prev->next; // Points to the next node after head
            }

            while(prev){
                free(prev);
                prev = cur;

                if(cur){
                cur = cur->next;
                }
            }
        }

        list->count = 0;
        list->head = NULL;

        free(list);
   } 
}

void ForwardList_clear_destroy(ForwardList *list){ // combines both the operations
    ForwardList_clear(list);
    ForwardList_destroy(list);
}

int ForwardList_push(ForwardList *list, void *data){
    check(list != NULL, "Invalid ForwardList!");
    check(data != NULL, "Invalid data!");

    ForwardListNode *node = ForwardListNode_create(data);
    check(node != NULL, "Failed to create node!");

    if(list->count == 0){ // Emtpy List
        list->head = node;
    }
    else{
        // traverse to the end 
        ForwardListNode *temp = list->head;
        while(temp->next != NULL){
            temp = temp->next;
        }

        temp->next = node; // Adding the node to the list
    }

    list->count++; // Incrementing the size

    return 0;
error:
    return -1;
}

int ForwardList_shift(ForwardList *list, void *data){
    check(list != NULL, "Invalid ForwardList!");
    check(data != NULL, "Invalid data!");

    ForwardListNode *node = ForwardListNode_create(data);
    check(node != NULL, "Failed to create node!");

    if(list->count == 0){
        list->head = node;
    }else{
        node->next = list->head; // Add it to the front
        list->head = node; // update the list head
    }

    list->count++; // increment the size

    return 0;
error:
    return -1;
} 

void* ForwardList_pop(ForwardList *list){
    check(list != NULL, "Invalid ForwardList!");
    check(list->count != 0, "Empty List!");
    
    // Traverse just before the last node
    ForwardListNode *prev = NULL, *cur = NULL;
    
    prev = cur = list->head;

    while(cur->next){ // If the next of cur is NULL then prev would 
                      // definitely point to the 2nd last node
        prev = cur;
        cur = cur->next;
    }

    void *data = cur->data; // copy the data

    if(prev != cur){ // if its not the only node left
        prev->next = NULL;
    }

    list->count--; // Decrement the size
    free(cur); // deallocate the memory for the node

    return data;
error:
    return NULL;
}

void* ForwardList_unshift(ForwardList *list){
    check(list != NULL, "Invalid ForwardList!");
    check(list->count != 0, "Empty List!"); 

    ForwardListNode *cur = list->head; // Get the ptr to head

    void *data = cur->data; // copy the data

    list->head = list->head->next; // move the head ahead
    
    list->count--; // Decrement the size
    free(cur); // deallocate the node

    return data;
error:
    return NULL;
}
