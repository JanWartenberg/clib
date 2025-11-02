#include "darray_algos.h"
#include <math.h>
#include <stdlib.h>

int DArray_qsort(DArray *array, DArray_compare cmp) {
  // use the stdlib qsort
  qsort(array->contents, DArray_count(array), sizeof(void *), cmp);
  return 0;
}

/********
 * Heap sort incl. helper functions
 **** */
// static inline helper
// have typecheckin as advantage over macros
static inline int left(int i) { return 2 * i + 1; }
static inline int right(int i) { return 2 * i + 2; }
static inline int parent(int i) { return (i - 1) / 2; }

void heapify_down(DArray *array, int root_idx, int end_idx,
                  DArray_compare cmp) {
  check(array != NULL, "array is NULL");
  check(cmp != NULL, "compare function missing");
  check(root_idx >= 0 && root_idx < end_idx, "root_idx out of range");
  check(end_idx <= array->end, "end_idx out of range: %d vs %d", end_idx,
        array->end);
  int child;

  // while root has at least one child
  while (left(root_idx) < end_idx) {
    child = left(root_idx);

    // if there is a right child
    // and that child is greater
    if ((child + 1 < end_idx) &&
        cmp(&array->contents[child], &array->contents[child + 1]) < 0) {
      // then proceed with the right child
      child++;
    }

    if (cmp(&array->contents[root_idx], &array->contents[child]) < 0) {
      DArray_swap(array, root_idx, child);
      // repeat to continue heapifying down
      root_idx = child;
    } else {
      // root holds the largest element
      return;
    }
  }
error:
  return;
}

void heapify(DArray *array, DArray_compare cmp) {
  // initialize start to first leaf node
  // find parent of last element
  int start = parent(DArray_count(array) - 1) + 1;

  while (start > 0) {
    // go to last non-heap node
    start--;
    // heapify down, from start on
    // such that all nodes below are in heap order
    heapify_down(array, start, array->end, cmp);
  }
}

// reimplemented, since my stdlib does not offer this
int DArray_heapsort(DArray *array, DArray_compare cmp) {
  heapify(array, cmp);

  int end = array->end;
  while (end > 1) {
    end--;
    DArray_swap(array, end, 0);
    heapify_down(array, 0, end, cmp);
  }

  return 0;
}

/********
 * Merge sort incl. helper functions
 **** */
DArray *merge(DArray *left, DArray *right, DArray_compare cmp) {
  check(left != NULL, "left array is NULL");
  check(right != NULL, "right array is NULL");

  DArray *result =
      DArray_create(left->element_size, DArray_len(left) + DArray_len(right));
  check_mem(result);

  int left_i = 0;
  int right_i = 0;
  while (left_i < DArray_len(left) && right_i < DArray_len(right)) {
    if (cmp(&left->contents[left_i], &right->contents[right_i]) <= 0) {
      DArray_push(result, left->contents[left_i]);
      left_i++;
    } else {
      DArray_push(result, right->contents[right_i]);
      right_i++;
    }
  }

  // Either left or right may have elements left; consume them.
  // (Only one of the following loops will actually be entered.)
  while (left_i < DArray_len(left)) {
    DArray_push(result, left->contents[left_i]);
    left_i++;
  }
  while (right_i < DArray_len(right)) {
    DArray_push(result, right->contents[right_i]);
    right_i++;
  }

  DArray_destroy(left);
  DArray_destroy(right);
  return result;
error:
  return NULL;
}

DArray *mergesort(DArray *array, DArray_compare cmp) {
  check(array != NULL, "array is NULL");
  check(cmp != NULL, "compare function missing");

  int len = DArray_len(array);
  if (len <= 1) {
    // base case
    // 1-element list already sorted
    return array;
  }

  // Recursive case. First, divide the list into equal-sized sublists
  // consisting of the first half and second half of the list.
  // This assumes lists start at index 0.
  float mid = len / 2.0;
  DArray *left = DArray_create(array->element_size, ceil(mid));
  DArray *right = DArray_create(array->element_size, ceil(mid));

  for (int i = 0; i < len; i++) {
    if (i < len / 2)
      DArray_push(left, array->contents[i]);
    else
      DArray_push(right, array->contents[i]);
  }

  left = mergesort(left, cmp);
  right = mergesort(right, cmp);

  return merge(left, right, cmp);
error:
  return NULL;
}

// we dont have this in stdlib
int DArray_mergesort(DArray *array, DArray_compare cmp) {
  DArray *result = mergesort(array, cmp);

  // NOTE: Result creates a new array,
  // we have to copy it to *array, where the outer function expects the sorted
  // update
  array->end = result->end;
  array->max = result->max;
  memcpy(array->contents, result->contents, result->end * sizeof(void *));
  DArray_destroy(result);

  return 0;
}
