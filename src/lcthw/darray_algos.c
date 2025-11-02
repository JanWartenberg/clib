#include "darray_algos.h"
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

/*
 // we dont have this in stdlib
int DArray_mergesort(DArray *array, DArray_compare cmp) {
  return mergesort(array->contents, DArray_count(array), sizeof(void *), cmp);
}
*/
