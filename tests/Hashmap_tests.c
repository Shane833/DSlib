#include "minunit.h"
#include <hashmap.h>
#include <assert.h>
#include <bstrlib.h>

Hashmap* map = NULL;
static int traverse_called = 0; 
struct tagbstring test1 = bsStatic("test data 1");
struct tagbstring test2 = bsStatic("test data 2"); 
struct tagbstring test3 = bsStatic("test data 3");
struct tagbstring test4 = bsStatic("test data 4");
struct tagbstring test5 = bsStatic("test data 5");
struct tagbstring test6 = bsStatic("test data 6");
struct tagbstring expect1 = bsStatic("THE VALUE 1");
struct tagbstring expect2 = bsStatic("THE VALUE 2");
struct tagbstring expect3 = bsStatic("THE VALUE 3");
struct tagbstring expect4 = bsStatic("THE VALUE 4");
struct tagbstring expect5 = bsStatic("THE VALUE 5");
struct tagbstring expect6 = bsStatic("THE VALUE 6");


static int traverse_good_cb(HashmapNode* node)
{
	debug("KEY : %s", bdata((bstring) node->key));
	traverse_called++;
	return 0;
}

static int traverse_fail_cb(HashmapNode* node)
{
	debug("KEY : %s", bdata((bstring) node->key));
	traverse_called++;
	
	if(traverse_called == 2){
		return 1;
	}else{
		return 0;
	}
}

char* test_create()
{
	/* Original Code
	map = Hashmap_create(NULL, NULL);
	*/
	// Impovement, Lets the user decide the no. of buckets
	//map = Hashmap_createDynamic(NULL, NULL, 5, 0.75);
	map = Hashmap_createStatic(NULL, NULL, 5);
	//map = Hashmap_create(NULL, NULL);
	mu_assert(map != NULL, "Failed to create map.");
	
	return NULL;
}

char* test_destroy()
{
	Hashmap_destroy(map);
	return NULL;
}

/* Hashmap_getThreshold() function has been made static to hashmap.c so can't be called outside 
char * test_threshold()	
{
	size_t threshold = Hashmap_getThreshold(map);
	mu_assert(threshold == 3, "ERROR : Wrong threshold computed!");

	return NULL;
}
*/

char* test_get_set()
{
	// since bstring is actually pointer to a tagbstring
	// If we provide their address it is equivalent to passing a bstring
	int rc = Hashmap_set(map, &test1, &expect1);
	mu_assert(rc == 0, "Failed to set &test1");
	bstring result = Hashmap_get(map, &test1); // returns the void*, i.e. the address
	mu_assert(result == &expect1, "Wrong value for test1");
	
	rc = Hashmap_set(map, &test2, &expect2);
	mu_assert(rc == 0, "Failed to set &test2");
	result = Hashmap_get(map, &test2);
	mu_assert(result == &expect2, "Wrong value for test2");
	
	rc = Hashmap_set(map, &test3, &expect3);
	mu_assert(rc == 0, "Failed to set &test3");
	result = Hashmap_get(map, &test3);
	mu_assert(result == &expect3, "Wrong value for test2");

	rc = Hashmap_set(map, &test4, &expect4);
	mu_assert(rc == 0, "Failed to set &test1");
	result = Hashmap_get(map, &test4); // returns the void*, i.e. the address
	mu_assert(result == &expect4, "Wrong value for test1");

	return NULL;
}

char* test_overwrite()
{
	int rc = Hashmap_set(map, &test1, &expect1);
	mu_assert(rc == 0, "Failed to set &test1");
	bstring result = Hashmap_get(map, &test1); // returns the void*, i.e. the address
	mu_assert(result == &expect1, "Wrong value for test1");
	
	rc = Hashmap_getSize(map);
	mu_assert(rc == 4, "Wrong value for map size");

	return NULL;
}

char * test_resize()
{
	int rc = Hashmap_set(map, &test4, &expect4);
	mu_assert(rc == 0, "Failed to set &test1");

	mu_assert(Hashmap_getSize(map) == 4, "Wrong size due to resizing!");
	
	bstring result = Hashmap_get(map, &test4); // returns the void*, i.e. the address
	mu_assert(result == &expect4, "Wrong value for test1");
	

	return NULL;
}

char * test_overflow()
{
	int rc = Hashmap_set(map, &test5, &expect5);
	mu_assert(rc == 0, "Failed to set &test5");

	rc = Hashmap_set(map, &test6, &expect6);
	mu_assert(rc == 0, "Failed to set &test6");

	return NULL;
}


char* test_traverse()
{
	int rc = Hashmap_traverse(map, traverse_good_cb);
	mu_assert(rc == 0, "Failed to traverse");
	mu_assert(traverse_called == 6, "Wrong count traverse");
	
	traverse_called = 0;
	rc = Hashmap_traverse(map, traverse_fail_cb);
	mu_assert(rc == 1, "Failed to traverse");
	mu_assert(traverse_called == 2, "Wrong count traverse for fail");
	
	return NULL;
}

char* test_delete()
{
	bstring deleted = (bstring) Hashmap_delete(map, &test1);
	mu_assert(deleted != NULL, "Got NULL on delete");
	mu_assert(deleted == &expect1, "should get test1");
	mu_assert(5 == Hashmap_getSize(map), "Wrong size on deletion");
	bstring result = Hashmap_get(map, &test1);
	mu_assert(result == NULL, "Should delete");
	
	deleted = (bstring) Hashmap_delete(map, &test2);
	mu_assert(deleted != NULL, "Got NULL on delete");
	mu_assert(deleted == &expect2, "should get test2");
	mu_assert(4 == Hashmap_getSize(map), "Wrong size on deletion");
	result = Hashmap_get(map, &test2);
	mu_assert(result == NULL, "should delete");
	
	deleted = (bstring) Hashmap_delete(map, &test3);
	mu_assert(deleted != NULL, "Got NULL on delete");
	mu_assert(deleted == &expect3, "should get test3");
	mu_assert(3 == Hashmap_getSize(map), "Wrong size on deletion");
	result = Hashmap_get(map, &test3);
	mu_assert(result == NULL, "should delete");

	deleted = (bstring) Hashmap_delete(map, &test4);
	mu_assert(deleted != NULL, "Got NULL on delete");
	mu_assert(deleted == &expect4, "should get test3");
	mu_assert(2 == Hashmap_getSize(map), "Wrong size on deletion");
	result = Hashmap_get(map, &test4);
	mu_assert(result == NULL, "should delete");

	deleted = (bstring) Hashmap_delete(map, &test5);
	mu_assert(deleted != NULL, "Got NULL on delete");
	mu_assert(deleted == &expect5, "should get test3");
	mu_assert(1 == Hashmap_getSize(map), "Wrong size on deletion");
	result = Hashmap_get(map, &test5);
	mu_assert(result == NULL, "should delete");

	deleted = (bstring) Hashmap_delete(map, &test6);
	mu_assert(deleted != NULL, "Got NULL on delete");
	mu_assert(deleted == &expect6, "should get test3");
	mu_assert(0 == Hashmap_getSize(map), "Wrong size on deletion");
	result = Hashmap_get(map, &test6);
	mu_assert(result == NULL, "should delete");
		
	return NULL;
}

char* all_tests()
{
	mu_suite_start();
	mu_run_test(test_create);
	mu_run_test(test_get_set);
	mu_run_test(test_overwrite);
	mu_run_test(test_resize);
	mu_run_test(test_overflow)
	mu_run_test(test_traverse);
	mu_run_test(test_delete);
	mu_run_test(test_destroy);
	
	return NULL;
}

RUN_TESTS(all_tests);