/* Re-implement by hand what the LLM already did, for practise's sake.
 *
 * TBC: is there an easy way to make this list treatment generic?
 *  for now I push ints, during deep_copy I cast to int,
 *  List_print needs the int..
 *  -> short answer: it is possible via void pointers
 *  -> long answer: it depends when this is feasible or not
 * */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../src/jclib/list.h"
#include "../src/jclib/list_algos.h"

#define TEST_REPETITIONS 100
#define LIST_ELEMENTS 1000
#define NORM_RAND_MAX 100

typedef List *(algo_func)(List *list);

/*
 * execute a list function and return the duration in seconds
 *
 * TBC time.h  vs. sys/time
 * time.h is OS independent
 * but subtle differences unclear
 *
 * returns seconds:
 * time(&start);
 *
 * */
double execute_measure(algo_func func, List *list) {
  time_t start, end;

  start = clock();
  func(list);
  end = clock();

  return (double)(end - start) / CLOCKS_PER_SEC;
}

List *List_deep_copy(List *list) {
  List *newlist = List_create();

  LIST_FOREACH(list, first, next, cur) {
    int *newval = (int *)malloc(sizeof(int));
    *newval = *(int *)cur->value;
    List_push(newlist, newval);
  }

  return newlist;
}

// wrapper function for the function under test
List *random_get1(List *list) {
  int rc = List_bubble_sort(list, (List_compare)compare_int);
  return list;
}

// wrapper function for the function under test
List *random_get2(List *list) {
  List *newlist = List_merge_sort(list, (List_compare)compare_int);
  return newlist;
}

int main() {
  printf("list_algos_benchmark started\n");
  // printf("CLOCKS_PER_SEC: %d\n", CLOCKS_PER_SEC);

  // create and populate a list
  List *origlist = List_create();
  int i = 0;
  for (i = 0; i < LIST_ELEMENTS; i++) {
    int *val = malloc(sizeof(int));
    *val = 1 + rand() / ((RAND_MAX + 1u) / NORM_RAND_MAX);
    List_push(origlist, (int *)val);
  }
  // for debug
  // List_print_int(origlist);

  double duration1 = 0.0;
  double duration2 = 0.0;
  for (i = 0; i < TEST_REPETITIONS; i++) {
    List *list1 = List_deep_copy(origlist);
    // call the algorithm under test
    duration1 += execute_measure(random_get1, list1);
    List_clear_destroy(list1);

    List *list2 = List_deep_copy(origlist);
    // call the algorithm under test2
    duration2 += execute_measure(random_get2, list2);

    // for crosschecking: sorting works 
    /*
    if (i == TEST_REPETITIONS - 1) {
      List *ret = algo2(list2);
      List_print_int(ret);
    }*/

    List_clear_destroy(list2);
  }

  printf("Duration algo 1: %f\n", duration1);
  printf("Duration algo 2: %f\n", duration2);

  return 0;
}
