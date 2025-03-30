/*
 * Benchmark, but for the DArray vs Linked List
 *
 * A) compare getting a value from both Data Structures from random location
 *      e.g. random get    container.get(location)
 * B) compare unshifting a value at front
 *      e.g. unshift    (insert at front of container)
 * C) compare appending a value at the end
 *      e.g. append     (insert at end of container)
 *
 * Note:
 * I copied a bit to separate methods for DArray / List
 *  -> it could be solved via void pointers, but this seemed to be clearer
 *  and the copied logic is just a few lines of code
 *
 *
 *  (Also gave task to Claude and see what it spits out. -> terrifyingly good)
 * */
#include <limits.h>
#include <stdlib.h>
#include <time.h>

#include "../src/lcthw/darray.h"
#include "../src/lcthw/list.h"
#include "../src/lcthw/rand.h"
#include "minunit.h"

#define TEST_REPETITIONS 7
#define LIST_ELEMENTS 20
#define FACTOR 10
#define NORM_RAND_MAX 100
#define INITIAL_MAX 100

typedef DArray *(algo_func)(DArray *list, int);
typedef List *(ll_func)(List *list, int);

/*
 * execute a dynamic array function and return the duration in seconds
 * */
double execute_measure(algo_func func, DArray *list, int randomidx) {
  time_t start, end;

  start = clock();
  func(list, randomidx);
  end = clock();

  return (double)(end - start) / CLOCKS_PER_SEC;
}

/*
 * execute a linked list function and return the duration in seconds
 * */
double execute_measure_ll(ll_func func, List *list, int randomidx) {
  time_t start, end;

  start = clock();
  func(list, randomidx);
  end = clock();

  return (double)(end - start) / CLOCKS_PER_SEC;
}

DArray *create_randomized_DArray(int factor) {
  DArray *origlist = DArray_create(sizeof(int), INITIAL_MAX);

  printf("Creating init list with %d number of elements.\n",
         factor * LIST_ELEMENTS);
  for (int i = 0; i < factor * LIST_ELEMENTS; i++) {
    int *val = calloc(1, sizeof(int));
    *val = rand_int(NORM_RAND_MAX);
    DArray_push(origlist, val);
  }
  return origlist;
}

/*
 * Deep copy a dynamic array to a new dynamic array
 * */
DArray *DArray_deep_copy(DArray *list) {

  DArray *newlist = DArray_create(sizeof(int), INITIAL_MAX);

  for (int i = 0; i < list->end; i++) {
    int *newval = (int *)calloc(1, sizeof(int));
    *newval = *(int *)DArray_get(list, i);
    DArray_push(newlist, newval);
  }
  return newlist;
}

/*
 * Copy contents of dynamic array to a linked list
 * */
List *DArray_copy_to_list(DArray *list) {
  List *newlist = List_create();

  for (int i = 0; i < list->end; i++) {
    int *newval = (int *)calloc(1, sizeof(int));
    *newval = *(int *)DArray_get(list, i);
    List_push(newlist, newval);
  }
  return newlist;
}

DArray *random_get1(DArray *list, int idx) {
  DArray_get(list, idx);
  return list;
}

List *random_get2(List *list, int idx) {
  List_get_value(list, idx);
  return list;
}

DArray *unshift1(DArray *list, int idx) {
  (void)idx;

  int *newval = (int *)calloc(1, sizeof(int));
  *newval = rand_int(INT_MAX);
  DArray_insert_begin(list, newval);
  return list;
}

List *unshift2(List *list, int idx) {
  (void)idx;

  int *newval = (int *)calloc(1, sizeof(int));
  *newval = rand_int(INT_MAX);
  List_unshift(list, newval);
  return list;
}

DArray *insert1(DArray *list, int idx) {
  (void)idx;

  int *newval = (int *)calloc(1, sizeof(int));
  *newval = rand_int(INT_MAX);
  DArray_push(list, newval);
  return list;
}

List *insert2(List *list, int idx) {
  (void)idx;

  int *newval = (int *)calloc(1, sizeof(int));
  *newval = rand_int(INT_MAX);
  List_push(list, newval);
  return list;
}

void execute_test_run(algo_func algo1, ll_func algo2, const char *msg1,
                      const char *msg2) {
  int i = 0;
  int factor = 1;

  for (i = 0; i < TEST_REPETITIONS; i++) {
    // create and populate a list
    //   but make it bigger for each repetition
    DArray *origlist = create_randomized_DArray(factor);
    int random_index = rand_int_in_upper_half(factor);
    printf("factor: %d, random_index: %d\n", factor, random_index);

    double duration1 = 0.0;
    double duration2 = 0.0;

    DArray *darray1 = DArray_deep_copy(origlist);
    duration1 += execute_measure(algo1, darray1, random_index);
    DArray_clear_destroy(darray1);

    List *llist2 = DArray_copy_to_list(origlist);
    duration2 += execute_measure_ll(algo2, llist2, random_index);
    List_clear_destroy(llist2);

    printf("%s %f\n", msg1, duration1);
    printf("%s %f\n", msg2, duration2);
    printf("\n");

    DArray_clear_destroy(origlist);
    factor *= FACTOR; // increase at end, to start with 1
  }
}

int main() {
  printf("darray vs list benchmark started\n");

  // A) random get
  execute_test_run(
      random_get1, random_get2,
      "Duration random get dynamic array:", "Duration random get linked list:");

  // B) compare unshifting a value at front
  execute_test_run(unshift1, unshift2,
                   "Duration insert at front of dynamic array:",
                   "Duration insert at front linked list:");

  // C) compare appending
  execute_test_run(insert1, insert2, "Duration insert at end of dynamic array:",
                   "Duration insert at end of linked list:");

  return 0;
}
