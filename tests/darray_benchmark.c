/*
 * Benchmark, but for the DArray vs Linked List
 *
 * A) compare getting a random value from both Data Structures
 * B) compare shifting a value at front
 * 
 * Note:
 * I copied a bit to separate methods for DArray / List
 *  -> it could be solved via void pointers, but this seemed to be clearer
 *  and the copied logic is just a few lines of code
 * */

/*
 * TODO
 *  decide WHAT to test
 *  -goal: compare linked list to DArray
 *      several comparisons needed
 *      e.g. random get    container.get(location)
 *      e.g. shift    (insert at front of container)
 *      e.g. walk through all items
 *
 *  (later: give task to Claude and see what it spits out)
 * */
#include <stdio.h>
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
  double diff;

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
  double diff;

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
      int *val = malloc(sizeof(int));
      *val = rand_int(NORM_RAND_MAX);
      DArray_push(origlist, (int *)val);
    }
    return origlist;
}

/*
 * Deep copy a dynamic array to a new dynamic array
 * */
DArray *DArray_deep_copy(DArray *list) {
  DArray *newlist = DArray_create(sizeof(int), INITIAL_MAX);

  for (int i = 0; i < list->end; i++) {
    int *newval = (int *)malloc(sizeof(int));
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
    int *newval = (int *)malloc(sizeof(int));
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

DArray *shift1(DArray *list) { return list; }

DArray *shift2(DArray *list) { return list; }

DArray *walk1(DArray *list) { return list; }

DArray *walk2(DArray *list) { return list; }


int main(int argc, char *argv[]) {
  printf("darray vs list benchmark started\n");

  int i = 0;
  int factor = 1;

  // A) random get
  for (i = 0; i < TEST_REPETITIONS; i++) {
    // create and populate a list
    //   but make it bigger for each repetition
    DArray *origlist = create_randomized_DArray(factor);
    int random_index = rand_int_in_upper_half(factor);
    printf("factor: %d, random_index: %d\n", factor, random_index);

    double duration1 = 0.0;
    double duration2 = 0.0;

    DArray *darray1 = DArray_deep_copy(origlist);
    duration1 += execute_measure(random_get1, darray1, random_index);
    DArray_clear_destroy(darray1);

    List *llist2 = DArray_copy_to_list(origlist);
    duration2 += execute_measure_ll(random_get2, llist2, random_index);
    List_clear_destroy(llist2);

    printf("Duration random get dynamic array: %f\n", duration1);
    printf("Duration random get linked list: %f\n", duration2);
    printf("\n");

    //B) compare shifting a value at front

    /*
      printf("Duration random get 1: %f\n", duration1);
      printf("Duration random get 2: %f\n", duration2);

      for (i = 0; i < TEST_REPETITIONS; i++) {
        DArray *list1 = DArray_deep_copy(origlist);
        duration1 += execute_measure(shift1, list1);
        DArray_clear_destroy(list1);

        DArray *list2 = DArray_deep_copy(origlist);
        duration2 += execute_measure(shift2, list2);

        DArray_clear_destroy(list2);
      }

      printf("Duration shift 1: %f\n", duration1);
      printf("Duration shift 2: %f\n", duration2);

      for (i = 0; i < TEST_REPETITIONS; i++) {
        DArray *list1 = DArray_deep_copy(origlist);
        // call the algorithm under test
        duration1 += execute_measure(walk1, list1);
        DArray_clear_destroy(list1);

        DArray *list2 = DArray_deep_copy(origlist);
        // call the algorithm under test2
        duration2 += execute_measure(walk2, list2);

        DArray_clear_destroy(list2);
      }
    */
    DArray_clear_destroy(origlist);
    factor *= FACTOR;
  }
  return 0;
}
