#include "minunit.h"
#include "../src/jclib/list.h"
#include <assert.h>

typedef int (*lib_function) (const char *data);
char *lib_file = "build/libjclib.so";
void *lib = NULL;

static List *list = NULL;
char *test1 = "test1 data";
char *test2 = "test2 data";
char *test3 = "test3 data";

List* setup_five_ints() {
    List * new_int_list = List_create();
    int *a = (int*)malloc(sizeof(int));
    int *b = (int*)malloc(sizeof(int));
    int *c = (int*)malloc(sizeof(int));
    int *d = (int*)malloc(sizeof(int));
    int *e = (int*)malloc(sizeof(int));
    *a = 1;
    *b = 2;
    *c = 3;
    *d = 4;
    *e = 5;
    List_push(new_int_list, a);
    List_push(new_int_list, b);
    List_push(new_int_list, c);
    List_push(new_int_list, d);
    List_push(new_int_list, e);
    return new_int_list;
}

char *test_create()
{
    list = List_create();
    mu_assert(list != NULL, "Failed to create list.");
    //printf("Count: %d\n", List_count(list));
    mu_assert(list->count == 0, "Default count is not 0.");

    return NULL;
}

char *test_destroy()
{
    List_clear_destroy(list);

    return NULL;
}

char *test_push_pop()
{
    List_push(list, test1);
    mu_assert(List_last(list) == test1, "Wrong last value.");

    List_push(list, test2);
    mu_assert(List_last(list) == test2, "Wrong last value.");

    List_push(list, test3);
    mu_assert(List_last(list) == test3, "Wrong last value.");
    mu_assert(List_count(list) == 3, "Wrong count on push.");

    //debug
    //printf("List_print, expect 3 values:\n");
    //List_print(list);

    char *val = List_pop(list);
    mu_assert(val == test3, "Wrong value on pop.");

    //debug
    //printf("List_print (after 1st pop)\n");
    //List_print(list);

    val = List_pop(list);
    mu_assert(val == test2, "Wrong value on pop.");

    //debug
    //printf("List_print (after 2nd pop)\n");
    //List_print(list);

    val = List_pop(list);
    mu_assert(val == test1, "Wrong value on pop.");
    mu_assert(List_count(list) == 0, "Wrong count after pop.");

    //printf("trying to pop 4th time\n");
    val = List_pop(list);
    mu_assert(val == NULL, "Wrong value on pop.");
    mu_assert(List_count(list) == 0, "Wrong count after pop.");

    return NULL;
}

char *test_unshift()
{
    List_unshift(list, test1);
    mu_assert(List_first(list) == test1, "Wrong first value.");

    List_unshift(list, test2);
    mu_assert(List_first(list) == test2, "Wrong first value.");

    List_unshift(list, test3);
    mu_assert(List_first(list) == test3, "Wrong first value.");
    mu_assert(List_count(list) == 3, "Wrong count on unshift.");

    return NULL;
}

char *test_remove()
{
    // we only need to test the middle remove case since push/shift 
    // already tests the other cases

    char *val = List_remove(list, list->first->next);
    mu_assert(val == test2, "Wrong removed element.");
    mu_assert(List_count(list) == 2, "Wrong count after remove.");
    mu_assert(List_first(list) == test3, "Wrong first after remove.");
    mu_assert(List_last(list) == test1, "Wrong last after remove.");

    return NULL;
}

char *test_shift()
{
    mu_assert(List_count(list) != 0, "Wrong count before shift.");

    char *val = List_shift(list);
    mu_assert(val == test3, "Wrong value on shift.");

    val = List_shift(list);
    mu_assert(val == test1, "Wrong value on shift.");
    mu_assert(List_count(list) == 0, "Wrong count after shift.");

    return NULL;
}

char *test_get()
{
    List * our_ints = setup_five_ints();

    for (int i = 0; i < 5; i++) {
        //printf("%d ", *(int*) List_get_value(our_ints, i));
        mu_assert(*(int*) List_get_value(our_ints, i) == i+1, 
                "Wrong value at List_get_value().");
    }
    //printf("\n");

    List_destroy(our_ints);
    return NULL;
}

char *test_swap()
{
    // setup
    List * our_ints = setup_five_ints();

    // swap same nodes
    List_swap(our_ints, our_ints->first->next, our_ints->first->next);
    mu_assert(*(int*)List_get_value(our_ints, 0) == 1, "Wrong value after (no) swap.");
    mu_assert(*(int*)List_get_value(our_ints, 1) == 2, "Wrong value after (no) swap.");
    mu_assert(*(int*)List_get_value(our_ints, 2) == 3, "Wrong value after (no) swap.");
    mu_assert(*(int*)List_get_value(our_ints, 3) == 4, "Wrong value after (no) swap.");
    mu_assert(*(int*)List_get_value(our_ints, 4) == 5, "Wrong value after (no) swap.");

    // swap first
    List_swap(our_ints, our_ints->first, our_ints->last->prev);
    //List_print_int(our_ints);
    mu_assert(*(int*)List_get_value(our_ints, 0) == 4, "Wrong value after (first) swap.");
    mu_assert(*(int*)List_get_value(our_ints, 3) == 1, "Wrong value after (first) swap.");

    // reset
    List_clear_destroy(our_ints);
    our_ints = setup_five_ints();
    // swap last
    List_swap(our_ints, our_ints->last, our_ints->last->prev->prev);
    //printf("\n\n");
    //List_print_int(our_ints);
    mu_assert(*(int*)List_get_value(our_ints, 2) == 5, "Wrong value after (last) swap.");
    mu_assert(*(int*)List_get_value(our_ints, 3) == 4, "Wrong value after (last) swap.");
    mu_assert(*(int*)List_get_value(our_ints, 4) == 3, "Wrong value after (last) swap.");

    
    // swap inner
    List_swap(our_ints, our_ints->first->next, our_ints->last->prev);
    //List_print_int(our_ints);
    mu_assert(*(int*)List_get_value(our_ints, 1) == 4, "Wrong value after (last) swap.");
    mu_assert(*(int*)List_get_value(our_ints, 3) == 2, "Wrong value after (last) swap.");

    List_destroy(our_ints);
    return NULL;
}

char *all_tests()
{
    mu_suite_start();

    mu_run_test(test_create);
    mu_run_test(test_get);
    mu_run_test(test_push_pop);
    mu_run_test(test_unshift);
    mu_run_test(test_remove);
    mu_run_test(test_shift);
    mu_run_test(test_swap);
    // this must be called last, for sure
    mu_run_test(test_destroy);

    return NULL;
}

RUN_TESTS(all_tests);
