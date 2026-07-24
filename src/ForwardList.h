#ifndef FORWARDLIST_H
#define FORWARDLIST_H

#include <stdlib.h>
#include <dbg.h>

typedef struct ForwardListNode{
    void *data;
    struct ForwardListNode *next;
}ForwardListNode;

typedef struct{
    size_t count;
    ForwardListNode *head;
}ForwardList;

ForwardList *ForwardList_create(); // Function to create a ForwardList

void ForwardList_clear_destroy(ForwardList *list); // uses both clear and destroy

void ForwardList_clear(ForwardList *list); // If the data stored in the list nodes is dynamically allocated
                                           // then it can be deallocated using this function

void ForwardList_destroy(ForwardList *list); // Destroys the list 

int ForwardList_push(ForwardList *list, void *data); // Pushes the data in the end

void* ForwardList_pop(ForwardList *list); // Pops the data in the end

int ForwardList_shift(ForwardList *list, void *data); // Pushes data in the front

void* ForwardList_unshift(ForwardList *list); // Pops data from the front

#define ForwardList_count(list) ((list) ? (list)->count : -1)

// L denotes the list, V represents the variable you use to repre
#define FORWARDLIST_FOREACH(L, V) check((L) != NULL, "Invalid ForwardList!");\
ForwardListNode *_node = NULL; ForwardListNode *V = NULL;\
for(V = _node = (L)->count ? (L)->head : NULL;_node != NULL;V = _node = _node->next)

#endif
