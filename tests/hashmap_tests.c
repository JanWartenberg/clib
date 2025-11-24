#include "../src/jclib/bstrlib.h"
#include "../src/jclib/hashmap.h"
#include "minunit.h"
#include <assert.h>

Hashmap *map = NULL;
static int traverse_called = 0;
struct tagbstring test1 = bsStatic("test data 1");
struct tagbstring test2 = bsStatic("test data 2");
struct tagbstring test3 = bsStatic("test data 3");
struct tagbstring test5 = bsStatic("test data 5");
struct tagbstring expect1 = bsStatic("THE VALUE 1");
struct tagbstring expect2 = bsStatic("THE VALUE 2");
struct tagbstring expect3 = bsStatic("THE VALUE 3");

static int traverse_good_cb(HashmapNode *node) {
  debug("KEY: %s", bdata((bstring)node->key));
  // printf("KEY: %s\n", bdata((bstring)node->key));
  traverse_called++;
  return 0;
}

static int traverse_fail(HashmapNode *node) {
  debug("KEY: %s", bdata((bstring)node->key));
  traverse_called++;

  if (traverse_called == 2) {
    return 1;
  } else {
    return 0;
  }
}

char *test_create() {
  map = Hashmap_create(NULL, NULL);
  mu_assert(map != NULL, "Failed to create map.");

  return NULL;
}

char *test_get_set() {
  int rc = Hashmap_set(map, &test1, &expect1);
  mu_assert(rc == 0, "Setting value to hashmap not working");
  bstring result = Hashmap_get(map, &test1);
  mu_assert(result == &expect1, "Wrong value for test1.");

  rc = Hashmap_set(map, &test2, &expect2);
  mu_assert(rc == 0, "Setting value to hashmap not working");
  result = Hashmap_get(map, &test2);
  mu_assert(result == &expect2, "Wrong value for test2.");

  rc = Hashmap_set(map, &test3, &expect3);
  mu_assert(rc == 0, "Setting value to hashmap not working");
  result = Hashmap_get(map, &test3);
  mu_assert(result == &expect3, "Wrong value for test3.");

  // ----
  // set existing Key
  rc = Hashmap_set(map, &test1, &expect2);
  mu_assert(rc == 0, "Setting value to hashmap twice not working");
  result = Hashmap_get(map, &test1);
  mu_assert(result == &expect2, "Wrong value for test1 (after resetting).");

  // ----
  // set / get NULL
  rc = Hashmap_set(map, &test1, NULL);
  mu_assert(rc == -1, "Setting NULL should not work.");

  rc = Hashmap_set(map, NULL, &expect2);
  mu_assert(rc == -1, "Setting NULL should not work.");

  rc = Hashmap_set(NULL, &test1, &expect2);
  mu_assert(rc == -1, "Setting to NULL map should not work.");

  result = Hashmap_get(NULL, &test3);
  mu_assert(result == NULL, "Getting from NULL map should not work.");
  result = Hashmap_get(map, NULL);
  mu_assert(result == NULL, "Getting NULL key should not work.");

  return NULL;
}

char *test_traverse() {

  int rc = Hashmap_traverse(map, traverse_good_cb);
  mu_assert(rc == 0, "Traversing has an error.");
  mu_assert(traverse_called == 3, "Wrong count traverse.");

  traverse_called = 0;
  rc = Hashmap_traverse(map, traverse_fail);
  mu_assert(rc == 1, "Failed to traverse.");
  mu_assert(traverse_called == 2, "Wrong count traverse for fail case.");

  return NULL;
}

char *test_delete() {
  bstring deleted = (bstring)Hashmap_delete(map, &test1);
  mu_assert(deleted != NULL, "Got NULL on delete.");
  // expect2 only because we change the value later
  mu_assert(deleted == &expect2, "Should get test1");
  bstring result = Hashmap_get(map, &test1);
  mu_assert(result == NULL, "Should delete.");

  deleted = (bstring)Hashmap_delete(map, &test1);
  mu_assert(deleted == NULL, "Got not NULL on deleting the same key twice.");
  result = Hashmap_get(map, &test1);
  mu_assert(result == NULL, "Should delete.");

  deleted = (bstring)Hashmap_delete(map, &test2);
  mu_assert(deleted != NULL, "Got NULL on delete.");
  mu_assert(deleted == &expect2, "Should get test2");
  result = Hashmap_get(map, &test2);
  mu_assert(result == NULL, "Should delete.");

  deleted = (bstring)Hashmap_delete(map, &test3);
  mu_assert(deleted != NULL, "Got NULL on delete.");
  mu_assert(deleted == &expect3, "Should get test3");
  result = Hashmap_get(map, &test3);
  mu_assert(result == NULL, "Should delete.");

  // delete bstring not in the Hashmap
  bstring never_set = bfromcstr("test 4");
  deleted = (bstring)Hashmap_delete(map, never_set);
  result = Hashmap_get(map, never_set);
  mu_assert(result == NULL, "Should delete.");

  // delete static tagbstring not in the Hashmap
  deleted = (bstring)Hashmap_delete(map, &test5);
  mu_assert(deleted == NULL, "Got not NULL on deleting non-existing.");

  return NULL;
}

char *test_destroy() {
  Hashmap_destroy(map);

  return NULL;
}

char *test_empty() {
  Hashmap *map_empty = NULL;
  map_empty = Hashmap_create(NULL, NULL);
  mu_assert(map_empty != NULL, "Failed to create map_empty.");

  bstring deleted = (bstring)Hashmap_delete(map_empty, &test5);

  bstring result = Hashmap_get(map_empty, &test1);
  mu_assert(result == NULL, "Wrong value for get from empty.");

  return NULL;
}

char *all_tests() {
  mu_suite_start();
  mu_run_test(test_create);
  mu_run_test(test_get_set);
  mu_run_test(test_traverse);
  mu_run_test(test_delete);
  mu_run_test(test_destroy);

  mu_run_test(test_empty);

  return NULL;
}

RUN_TESTS(all_tests);
