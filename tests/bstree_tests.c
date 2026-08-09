#include "../src/jclib/bstree.h"
#include "../src/jclib/bstrlib.h"
#include "minunit.h"
#include <time.h>

BSTree *map = NULL;
static int traverse_called = 0;
struct tagbstring test1 = bsStatic("test data 1");
struct tagbstring test2 = bsStatic("test data 2");
struct tagbstring test3 = bsStatic("test data 3");
struct tagbstring expect1 = bsStatic("THE VALUE 1");
struct tagbstring expect2 = bsStatic("THE VALUE 2");
struct tagbstring expect3 = bsStatic("THE VALUE 3");

/* test helper functions */
static int traverse_good_cb(BSTreeNode *node) {
  debug("KEY: %s", bdata((bstring)node->key));
  traverse_called++;
  return 0;
}

static int traverse_fail_cb(BSTreeNode *node) {
  debug("KEY: %s", bdata((bstring)node->key));
  traverse_called++;

  if (traverse_called == 2) {
    return 1;
  } else {
    return 0;
  }
}

static int traverse_count_cb(BSTreeNode *node) {
  (void)node;
  traverse_called++;
  return 0;
}

static int traverse_trace_cb(BSTreeNode *node) {
  debug("VISIT key=%s parent=%p left=%p right=%p",
        bdata((bstring)node->key),
        (void*)node->parent, (void*)node->left, (void*)node->right);
  traverse_called++;
  return 0;
}


/* actual test cases */
char *test_create() {
  map = BSTree_create(NULL);
  mu_assert(map != NULL, "Failed to create map.");

  return NULL;
}

char *test_destroy() {
  BSTree_destroy(map);

  return NULL;
}

char *test_get_set() {
  int rc = BSTree_set(map, &test1, &expect1);
  mu_assert(rc == 0, "Failed to set &test1");
  bstring result = BSTree_get(map, &test1);
  mu_assert(result == &expect1, "Wrong value for test1.");

  rc = BSTree_set(map, &test2, &expect2);
  mu_assert(rc == 0, "Failed to set test2");
  result = BSTree_get(map, &test2);
  mu_assert(result == &expect2, "Wrong value for test2.");

  rc = BSTree_set(map, &test3, &expect3);
  mu_assert(rc == 0, "Failed to set test3");
  result = BSTree_get(map, &test3);
  mu_assert(result == &expect3, "Wrong value for test3.");

  return NULL;
}

char *test_traverse() {
  traverse_called = 0;
  int rc = BSTree_traverse(map, traverse_good_cb);
  mu_assert(rc == 0, "Failed to traverse.");
  mu_assert(traverse_called == 3, "Wrong count traverse.");

  traverse_called = 0;
  rc = BSTree_traverse(map, traverse_fail_cb);
  mu_assert(rc == 1, "Failed to traverse.");
  mu_assert(traverse_called == 2, "Wrong count traverse for fail.");

  return NULL;
}

char *test_delete() {
  bstring deleted = (bstring)BSTree_delete(map, &test1);
  mu_assert(deleted != NULL, "Got NULL on delete.");
  mu_assert(deleted == &expect1, "Should get test1");
  bstring result = BSTree_get(map, &test1);
  mu_assert(result == NULL, "Should delete.");

  deleted = (bstring)BSTree_delete(map, &test1);
  mu_assert(deleted == NULL, "Should get NULL on delete");

  deleted = (bstring)BSTree_delete(map, &test2);
  mu_assert(deleted != NULL, "Got NULL on delete.");
  mu_assert(deleted == &expect2, "Should get test2");
  result = BSTree_get(map, &test2);
  mu_assert(result == NULL, "Should delete.");

  deleted = (bstring)BSTree_delete(map, &test3);
  mu_assert(deleted != NULL, "Got NULL on delete.");
  mu_assert(deleted == &expect3, "Should get test3");
  result = BSTree_get(map, &test3);
  mu_assert(result == NULL, "Should delete.");

  // test deleting non-existent stuff
  deleted = (bstring)BSTree_delete(map, &test3);
  mu_assert(deleted == NULL, "Should get NULL");

  return NULL;
}

char *test_fuzzing() {
  BSTree *store = BSTree_create(NULL);
  int i = 0;
  int j = 0;
  bstring numbers[100] = {NULL};
  bstring data[100] = {NULL};
  srand((unsigned int)time(NULL));

  for (i = 0; i < 100; i++) {
    int num = rand();
    numbers[i] = bformat("%d", num);
    data[i] = bformat("data %d", num);
    BSTree_set(store, numbers[i], data[i]);
  }

  for (i = 0; i < 100; i++) {
    bstring value = BSTree_delete(store, numbers[i]);
    mu_assert(value == data[i], "Failed to delete the right number.");

    mu_assert(BSTree_delete(store, numbers[i]) == NULL, "Should get nothing.");

    for (j = i + 1; j < 99 - i; j++) {
      bstring value = BSTree_get(store, numbers[j]);
      mu_assert(value == data[j], "Failed to get the right number.");
    }

    bdestroy(value);
    bdestroy(numbers[i]);
  }

  BSTree_destroy(store);

  return NULL;
}

char *test_set_replaces_duplicates() {
  // fresh tree
  BSTree *local = BSTree_create(NULL);
  mu_assert(local != NULL, "Failed to create local map.");

  // same key, different data
  struct tagbstring key = bsStatic("DUPLICATE KEY");
  struct tagbstring data1 = bsStatic("DATA 1");
  struct tagbstring data2 = bsStatic("DATA 2");

  int rc = BSTree_set(local, &key, &data1);
  mu_assert(rc == 0, "Failed to set data1.");

  rc = BSTree_set(local, &key, &data2);
  mu_assert(rc == 0, "Failed to set data2.");

  // verify: get returns the latest data
  bstring result = BSTree_get(local, &key);
  mu_assert(result == &data2,
            "Expected set to replace data for duplicate key.");

  // verify: only one node exists for this key
  traverse_called = 0;
  rc = BSTree_traverse(local, traverse_count_cb);
  mu_assert(rc == 0, "Failed to traverse.");

  mu_assert(traverse_called == 1,
            "Duplicate key inserted: expected exactly 1 node in the tree.");

  BSTree_destroy(local);
  return NULL;
}

char *test_traverse_trace() {
  traverse_called = 0;
  BSTree_traverse(map, traverse_trace_cb);
  mu_assert(traverse_called == 3, "Expected 3 nodes");
  return NULL;
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_create);
  mu_run_test(test_get_set);
  mu_run_test(test_set_replaces_duplicates);
  mu_run_test(test_traverse);
  mu_run_test(test_traverse_trace);
  mu_run_test(test_delete);
  mu_run_test(test_destroy);
  mu_run_test(test_fuzzing);

  return NULL;
}

RUN_TESTS(all_tests);
