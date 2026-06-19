#ifndef lcthw_list_algos_h
#define lcthw_list_algos_h

#include <lcthw/list.h>

// This is a function pointer for a custom compare function which the user
// can provide defining how the comparision must be made between the lists
typedef int (*List_compare) (const void* a, const void* b);

// Simple bubble sort function to linearly traverse the list 
// and based on the compare function sorts the list
int List_bubble_sort(List* list, List_compare cmp);

// This follows the basic implementation of the merge sort
// however it will return a new sorted list instead of sorting
// the list in place like in the case of arrays
List* List_merge_sort(List* list, List_compare cmp);

#endif
