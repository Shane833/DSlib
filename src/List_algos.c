#include <lcthw/list_algos.h>
#include <lcthw/dbg.h>

// Swap function for swapping the values of the given listnodes
inline void ListNode_swap(ListNode* a, ListNode* b)
{
	void* temp = a->value;
	a->value = b->value;
	b->value = temp;
}

int List_bubble_sort(List* list, List_compare cmp)
{
	int sorted = 1; // flag to break the loop if the list is sorted
	if(List_count(list) <= 1){
		return 0; // already sorted
	}
	
	do{
		sorted = 1; // initializing it to be 1
		LIST_FOREACH(list, first, next, cur){ // traversing the entirity of the list
			if(cur->next){
				if(cmp(cur->value, cur->next->value) > 0){ // if the next value is smaller
					ListNode_swap(cur, cur->next); // we swap them
					sorted = 0; // and since we have swapped them we set sorted to be 0
				}
			}
		}
	} while(!sorted); // loops until the data is sorted
	
	return 0;
}

// This is the merge function to be called after dividing into left and right lists
inline List* List_merge(List* left, List* right, List_compare cmp)
{
	// List to store the result after merging the two lists
	List* result = List_create();
	void* val = NULL; // pointer to store the value temporary values
	
	// We will keep looping till either of the list is empty
	while(List_count(left) > 0 || List_count(right) > 0){
		if(List_count(left) > 0 && List_count(right) > 0){ // While both of the lists have some elements
			if(cmp(List_first(left), List_first(right)) <= 0){ // we will compare the first elements of each list
				val = List_shift(left); 
			}else{
				val = List_shift(right);
			}
			List_push(result,val); // then accordingly we will insert the elements in the list
			
		}else if(List_count(left) > 0){ // if the right list is empty and there exists elements in the left list
			val = List_shift(left);
			List_push(result, val);
		}else if(List_count(right) > 0){ // if the left list is empty and there exists elements in the right list
			val = List_shift(right);
			List_push(result, val);
		}
	}
	
	return result;
}

List* List_merge_sort(List* list, List_compare cmp)
{
	if(List_count(list) <= 1){ // if the length of the list 1, then no need to sort it
		return list; // as simply return the list
	}
	
	// Creating two sets of list for holding the left and right
	// elements of the list
	List* left = List_create();
	List* right = List_create();
	// Calculating the mid point for the list
	int middle = List_count(list) / 2;
	
	LIST_FOREACH(list, first, next, cur){
		if(middle > 0){ // first copying the left part of the list
			List_push(left, cur->value);
		} else{ // Then copying the remaining into the right list
			List_push(right, cur->value);
		}
		
		middle--; 
	}
	
	// Recursively call the function for both set of lists
	List* sort_left = List_merge_sort(left, cmp);
	List* sort_right = List_merge_sort(right, cmp);
	
	// if no change has been done after further dividing
	// then we simply destroy the lists
	if(sort_left != left)
		List_destroy(left);
	if(sort_right != right)
		List_destroy(right);
	
	// Calling the merge function to merge the both lists
	return List_merge(sort_left, sort_right, cmp);
}