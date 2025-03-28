#include "../src/lcthw/darray.h"
#include "minunit.h"

static DArray *array = NULL;
static int *val1 = NULL;
static int *val2 = NULL;
#define INITIAL_MAX 100

char *test_create() {
  array = DArray_create(sizeof(int), INITIAL_MAX);
  mu_assert(array != NULL, "DArray_create failed");
  mu_assert(array->contents != NULL, "contents are wrong in DArray");
  mu_assert(array->end == 0, "end isn't at the right spot");
  mu_assert(array->element_size == sizeof(int), "element size is wrong");
  mu_assert(array->max == INITIAL_MAX, "wrong max length on initial size");

  return NULL;
}

char *test_destroy() {
  DArray_destroy(array);
  return NULL;
}

char *test_new() {
  val1 = DArray_new(array);
  mu_assert(val1 != NULL, "failed to make a new element");

  val2 = DArray_new(array);
  mu_assert(val2 != NULL, "failed to make a new element");

  return NULL;
}

char *test_set() {
  DArray_set(array, 0, val1);
  DArray_set(array, 1, val2);

  return NULL;
}

char *test_get() {

  mu_assert(DArray_get(array, 0) == val1, "Wrong first value.");
  mu_assert(DArray_get(array, 1) == val2, "Wrong second value.");
  int *ret = DArray_get(array, 2);
  mu_assert(ret == NULL, "Wrong third value (should be NULL).");

  return NULL;
}

char *test_remove() {
  int *val_check = DArray_remove(array, 0);
  mu_assert(val_check != NULL, "Should not get NULL.");
  mu_assert(*val_check == *val1, "Should get the first value");
  mu_assert(DArray_get(array, 0) == NULL, "Should be gone.");
  DArray_free(val_check);

  val_check = DArray_remove(array, 1);
  mu_assert(val_check != NULL, "Should not get NULL.");
  mu_assert(*val_check == *val2, "Should get the second value");
  mu_assert(DArray_get(array, 1) == NULL, "Should be gone.");
  DArray_free(val_check);

  return NULL;
}

char *test_expand_contract() {
  int old_max = array->max;
  mu_assert((unsigned int)array->max == INITIAL_MAX,
            "Wrong start size before expand.");
  DArray_expand(array);

  mu_assert((unsigned int)array->max == old_max + array->expand_rate,
            "Wrong size after expand.");

  DArray_contract(array);
  mu_assert((unsigned int)array->max == array->expand_rate + 1,
            "Should stay at the expand_rate at least.");

  DArray_contract(array);
  mu_assert((unsigned int)array->max == array->expand_rate + 1,
            "Should stay at the expand_rate at least.");

  return NULL;
}

char *test_push_pop() {
  int i = 0;
  for (i = 0; i < 1000; i++) {
    int *val = DArray_new(array);
    *val = i * 333;
    DArray_push(array, val);
  }

  mu_assert(array->max == 1201, "Wrong max size");

  for (i = 999; i >= 0; i--) {
    int *val = DArray_pop(array);
    mu_assert(val != NULL, "Shouldn't get a NULL.");
    mu_assert(*val == i * 333, "Wrong value.");
    DArray_free(val);
  }

  return NULL;
}

/*
 * Note: this catches a bug in Zed's original code (DArray_expand).
 * For simple lists it did not cause an issue, but when benchmarking
 * and creating/clearing lists multiple times, a double free occurs.
 */
char *test_expanded_area_initialized() {
  // Create a dynamic array with a small initial size.
  DArray *test_array = DArray_create(sizeof(int), 10);
  int old_max = test_array->max;

  // fill some of the array to simulate normal usage.
  int i;
  for (i = 0; i < 5; i++) {
    int *val = DArray_new(test_array);
    *val = i;
    DArray_push(test_array, val);
  }

  // Expand the array.
  DArray_expand(test_array);
  int new_max = test_array->max;

  // Check that the new area is properly initialized to NULL.
  for (i = old_max; i < new_max; i++) {
    mu_assert(test_array->contents[i] == NULL,
              "Expanded area not initialized to NULL.");
  }

  DArray_clear_destroy(test_array);
  return NULL;
}

char *test_clear() {
#define INITIAL_MAX2 25
  DArray *array2 = DArray_create(sizeof(float), 25);
  mu_assert(array2->max == INITIAL_MAX2, "Max not as expected.");

  int i = 0;
  for (i = 0; i < INITIAL_MAX2 + DEFAULT_EXPAND_RATE - 1; i++) {
    float *val = DArray_new(array2);
    *val = i * 333.0;
    DArray_push(array2, val);
  }

  mu_assert(array2->end == INITIAL_MAX2 + DEFAULT_EXPAND_RATE - 1,
            "End not as expected.");
  mu_assert(array2->max == INITIAL_MAX2 + DEFAULT_EXPAND_RATE,
            "Max not as expected.");

  DArray_clear(array2);

  mu_assert(array2->end == 0, "End not as expected.");
  mu_assert(array2->max == INITIAL_MAX2 + DEFAULT_EXPAND_RATE,
            "Max not as expected.");
  return NULL;
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_create);
  mu_run_test(test_new);
  mu_run_test(test_set);
  mu_run_test(test_get);
  mu_run_test(test_remove);
  mu_run_test(test_expand_contract);
  mu_run_test(test_push_pop);
  mu_run_test(test_clear);
  mu_run_test(test_destroy);

  return NULL;
}

RUN_TESTS(all_tests);
