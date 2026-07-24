#include <ForwardList.h>
#include "minunit.h"

ForwardList *list = NULL;
char* names[] = {"shane", "shivang", "sks"};
size_t arr_size = sizeof(names) / sizeof(names[0]);

char* test_create(){
   list = ForwardList_create();
   mu_assert(list != NULL, "Failed to create list!");

   return NULL;
}

char* test_destroy(){
    ForwardList_destroy(list);

    return NULL;
}

char* test_push(){
   int r = 0;

   for(int i = 0;i < arr_size;i++){
       r = ForwardList_push(list, names[i]);
       mu_assert(r == 0, "Failed to push data!");
       mu_assert(ForwardList_count(list) == (i + 1), "Failed to update the list size!");
   }
    
   return NULL;
}

char* test_pop(){
    char *result = NULL;
    for(int i = 0;i < arr_size;i++){
        result = ForwardList_pop(list);
        mu_assert(result == names[arr_size - 1 - i], "Wrong result of pop!");
        mu_assert(ForwardList_count(list) == arr_size - 1 - i, "Failed to update the list size!");
    }
    
    result = ForwardList_pop(list);
    mu_assert(result == NULL, "Wrong result from pop!");

    return NULL;
}

char* test_shift(){
    int r = 0;

    for(int i = 0;i < arr_size;i++){
        r = ForwardList_shift(list, names[i]);
        mu_assert(r == 0, "Failed to shift data!");
        mu_assert(ForwardList_count(list) == (i + 1), "Failed to update the list size!");
    }
    
    mu_assert(list->head->data == names[arr_size - 1], "Wrong data at the head!");

    return NULL;
}

char* test_unshift(){
    char *result = NULL; 
    for(int i = 0;i < arr_size;i++){
        result = ForwardList_unshift(list);
        mu_assert(result == names[arr_size - 1 - i], "Wrong result of unshift!");
        mu_assert(ForwardList_count(list) == arr_size - 1 - i, "Failed to update the list size!");
    }

    result = ForwardList_unshift(list);
    mu_assert(result == NULL, "Wrong result from unshift!");

    return NULL;
}

char* all_tests(){
    mu_suite_start(); 
    mu_run_test(test_create);
    /*
    mu_run_test(test_push);
    mu_run_test(test_pop);
    mu_run_test(test_push);
    mu_run_test(test_pop);
    */
    mu_run_test(test_shift);
    mu_run_test(test_unshift);
    mu_run_test(test_shift);
    mu_run_test(test_unshift);
    mu_run_test(test_destroy);
    return NULL;
}

RUN_TESTS(all_tests);
