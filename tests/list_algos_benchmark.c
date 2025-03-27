/* Re-implement by hand what the LLM already did, for practise's sake.
 *
 * TBC: is there an easy way to make this list treatment generic?
 *  for now I push ints, during deep_copy I cast to int,
 *  List_print needs the int..
 * */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../src/lcthw/list.h"
#include "../src/lcthw/wait.h"

#define TEST_REPETITIONS 50
#define LIST_ELEMENTS 500
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
  double diff;

  start = clock();
  func(list);
  end = clock();

  return (double)(end-start)/CLOCKS_PER_SEC;
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

// dummy functions
List *algo1(List *list) {
  List_print_int(list);
  return list;
}

List *algo2(List *list) {
  List_print_int(list);
  wait_ms(1);
  return list;
}

int main(int argc, char *argv[]) {
  printf("list_algos_benchmark started\n");
  //printf("CLOCKS_PER_SEC: %d\n", CLOCKS_PER_SEC);


  // create and populate a list
  List *origlist = List_create();
  int i = 0;
  for (i = 0; i < LIST_ELEMENTS; i++) {
    int *val = malloc(sizeof(int));
    *val = 1 + rand() / ((RAND_MAX + 1u) / NORM_RAND_MAX);
    List_push(origlist, (int *)val);
  }
  // for debug
  List_print_int(origlist);

  double duration1 = 0.0;
  double duration2 = 0.0;
  for (i = 0; i < TEST_REPETITIONS; i++) {
    // copy the list
    List *list1 = List_deep_copy(origlist);
    // call the algorithm under test
    duration1 += execute_measure(algo1, list1);

    // copy the list
    List *list2 = List_deep_copy(origlist);
    // call the algorithm under test2
    duration2 += execute_measure(algo2, list2);
  }

  printf("Duration algo 1: %f\n", duration1);
  printf("Duration algo 2: %f\n", duration2);

  return 0;
}
