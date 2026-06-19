#include <list.h>
#include <dbg.h>

List* List_create()
{
	return calloc(1, sizeof(List)); // Used calloc because it zero-initialized the data structure
}

void List_destroy(List* list)
{
    
	LIST_FOREACH(list, first, next, cur){ // we go through the entire list
		if(cur->prev){ // and if the previous exists we simply deallocate memory from it
			free(cur->prev); // Visually looks cool *****->####
		}
	}
    
    /*
    ListNode *cur = list->first;
    for(int i = 0;i < List_count(list);i++){
        if(cur->prev){
            free(cur->prev);
        }
        cur = cur->next;
    }
	free(list->last); // at last we free the end node
	free(list); // and then free the whole list
	*/
}

void List_clear(List* list)
{
	// Traversing the whole list
    
	LIST_FOREACH(list, first, next, cur){
		free(cur->value); // and freeing the value element of then ode
	}
    
    /*
    ListNode *cur = list->first;
    for(int i = 0;i < List_count(list);i++, cur=cur->next){
        free(cur->value);
    }
    */
    /*
    ListNode *cur = list->first;
    while(cur){
        free(cur->value);
        cur = cur->next;
    }
    */
}

void List_clear_destroy(List* list)
{
    if(list){
	// Combines both of the operations into a handy function
	List_clear(list);
	List_destroy(list);
    }
}

void List_push(List* list, void* value)
{
	ListNode* node = calloc(1, sizeof(ListNode)); // first we create a new node
	check_mem(node); // safety measure
	
	node->value = value; // giving it the provided value
	
	if(list->last == NULL){ // condition when the list is just created
		list->first = node; // we simply assign the node as both the first and the last
		list->last = node;
	}else{ // however when the last exists we simply make it as the last
		list->last->next = node;
		node->prev = list->last;
		list->last = node;
	}
	
	list->count++; // increment the no. of nodes
	
	error:
		return;
}

void* List_pop(List* list)
{
	ListNode* node = list->last; // creating a pointer to the last node
	return node != NULL ? List_remove(list, node) : NULL; // simply removing the last node
}

void List_unshift(List* list, void* value)
{
	// In this function we will push node to the beginning of the list
	ListNode* node = calloc(1, sizeof(ListNode)); // create a new node as usual
	check_mem(node); // safety first
	
	node->value = value; // giving it the provided value
	
	if(list->first == NULL){ // if the list is just created then
		list->first = node; // make the list as both the first and last element
		list->last = node;
	}else{ // however when first exists we simply make the node as first
		node->next = list->first;
		list->first->prev = node;
		list->first = node;
	}
	
	list->count++;
	
	error:
		return;
}

void* List_shift(List* list)
{
	ListNode* node = list->first; // create a pointer to the first node
	return node != NULL ? List_remove(list, node) : NULL; // simply remove the first node if it exists
}

void* List_remove(List* list, ListNode* node)
{
	void* result = NULL;
	
	check(list->first && list->last, "List is empty.");
	check(node, "node can't be NULL");
	
	if(node == list->first && node == list->last){ // checking if the list only has one element
		list->first = NULL; // simply setting the first and last to be null
		list->last = NULL;
	}else if(node == list->first){ // if it is the first node
		list->first = node->next; // then we make the next of first as the new first
		check(list->first != NULL, "Invalid list, somehow got a first that is NULL.");
		list->first->prev = NULL; // and set the previous to be null
	}else if(node == list->last){ // if it is the last node
		list->last = node->prev; // then make its previous the last
		check(list->last != NULL, "Invalid list, somehow got a next that is NULL.");
		list->last->next = NULL; // and sets its next to be null
	}else{ // if it is something in the middle
		ListNode* after = node->next; // then we simly connect its next and previous with one another
		ListNode* before = node->prev;
		after->prev = before;
		before->next = after;
	}
	
	list->count--; // decrement the no. of nodoes
	result = node->value; // the value to be returned
	free(node); // simply deallocate the memory from the node
	
	error:
		return result;
}
