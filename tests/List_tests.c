#include "minunit.h"
#include <List.h>
#include <assert.h>

static List* list = NULL;
char* test1 = "test1 data";
char* test2 = "test2 data";
char* test3 = "test3 data";
char* test4 = "test4 data";

char* test_create()
{
	list = List_create();
	mu_assert(list != NULL, "Failed to create list");
	
	return NULL;
}

char* test_destroy()
{
	List_clear_destroy(list);
	
	return NULL;
}

char* test_push_pop()
{
	List_push(list, test1);
	mu_assert(List_last(list) == test1, "Wrong last value");
	
	List_push(list, test2);
	mu_assert(List_last(list) == test2, "Wrong last value");
	
	List_push(list, test3);
	mu_assert(List_last(list) == test3, "Wrong last value");
	mu_assert(List_count(list) == 3, "Wrong count on push");
	
	char* val = List_pop(list);
	mu_assert(val == test3, "Wrong value on pop");
	
	val = List_pop(list);
	mu_assert(val == test2, "Wrong value on pop");
	
	val = List_pop(list);
	mu_assert(val == test1, "Wrong value on pop");
	mu_assert(List_count(list) == 0, "Wrong count after pop");
	
	return NULL;
}

char* test_unshift()
{
	List_unshift(list, test1);
	mu_assert(List_first(list) == test1, "Wrong first value");
	
	List_unshift(list, test2);
	mu_assert(List_first(list) == test2, "Wrong first value");
	
	List_unshift(list, test3);
	mu_assert(List_first(list) == test3, "Wrong last value");
	mu_assert(List_count(list) == 3, "Wrong count on unshift");
	
	return NULL;
}

char* test_remove()
{
	// we only need to test the middle remove case since push/shift
	// already tests the other cases
	
	char* val = List_remove(list, list->first->next);
	mu_assert(val == test2, "Wrong removed element");
	mu_assert(List_count(list) == 2, "Wrong count after remove");
	mu_assert(List_first(list) == test3, "Wrong first after remove");
	mu_assert(List_last(list) == test1, "Wrong last after remove");
	
	return NULL;
}

char* test_shift()
{
	mu_assert(List_count(list) != 0, "Wrong count before shift");
	
	char* val = List_shift(list);
	mu_assert(val == test3, "Wrong value on shift");
	
	val = List_shift(list);
	mu_assert(val == test1, "Wrong value on shift");
	mu_assert(List_count(list) == 0, "Wrong count after shift");
	
	return NULL;
}

char* test_split()
{
	char* test_data[3] = {test1, test3, test4};
	
	List* combined = List_create();
	
	for(int i = 0;i < sizeof(test_data)/sizeof(test_data[0]);i++){
		List_push(combined, test_data[i]);
	}
	
	mu_assert(List_count(combined) == 3, "Push failed!");
	
	mu_assert(combined->first->value == test1, "Push failed at first!");
	mu_assert(combined->last->value == test4, "Push failed at last!");
	
	List* splitted = List_split(combined, 1); // split at the second position
	mu_assert(splitted != NULL, "Split List failed!");
	mu_assert(splitted->first->value == test3, "Split failed!");
	mu_assert(splitted->last->value == test4, "Split failed!");
	mu_assert(List_count(splitted) == 2, "Split failed!");
	
	mu_assert(combined->last->value == test1, "Split failed!");
	mu_assert(List_count(combined) == 1, "Split failed!");
    
	List_destroy(combined);
	List_destroy(splitted);
	
	return NULL;
}

char* test_merge()
{	
	List* first = List_create();
	List_push(first, test1);
	List_push(first, test2);
	
	List* second = List_create();
	List_push(second, test3);
	List_push(second, test4);
	
	List_merge(first, second);
	
	mu_assert(List_count(first) == 4, "Count update failed");
	mu_assert(List_count(second) == 0, "Count update failed");

	mu_assert(List_last(first) == test4, "Last update failed");
	mu_assert(second->last == NULL, "Last update failed");
	
	List_destroy(first);
	List_destroy(second);
	
	return NULL;
}

char* all_tests()
{
	mu_suite_start();
	
	mu_run_test(test_create);
	mu_run_test(test_push_pop);
	mu_run_test(test_unshift);
	mu_run_test(test_remove);
	mu_run_test(test_shift);
	mu_run_test(test_split)
	mu_run_test(test_merge);
	mu_run_test(test_destroy);
	
	return NULL;
}

RUN_TESTS(all_tests);
