#include "list_algos.h"
#include "dbg.h"
#include "list.h"
#include <stdio.h>

int compare_int(const void* a, const void *b)
{
  const int *ia = (int*) a;
  const int *ib = (int*) b;
  return *ia - *ib;
}

int List_bubble_sort(List * list, List_compare cmp) {
    check(list != NULL, "List to be sorted must exist");

    if (List_count(list) <= 1) {
        return 0;   // already sorted
    }

    int sorted = 1;
    do {
        sorted = 1;
        LIST_FOREACH(list, first, next, cur) {
            if (cur->next) {
                if (cmp(cur->value, cur->next->value) > 0) {
                    //List_swap(list, cur, cur->next);
                    List_swap_values(cur, cur->next);
                    sorted = 0;
                }
            }
        }
    } while (!sorted);

    return 0;
error:
    return 1;
}

List* List_merge_sort(List * list, List_compare cmp) {
    check(list != NULL, "List to be sorted must exist");

    // init  (copy to result array)
    List *result = List_create();

    // recursive base case
    if (list->count <= 1) {
        return list;
    }

    // do the split
    int middle = list->count / 2;
    List* left = List_create();
    List* right = List_create();

    LIST_FOREACH(list, first, next, cur) {
        if (middle > 0) {
            List_push(left, cur->value);
        } else {
            List_push(right, cur->value);
        }
        middle--;
    }

   // recursive func  (split_merge)
   List* left_sorted = List_merge_sort(left, cmp);
   List* right_sorted = List_merge_sort(right, cmp);
   // merge back
   result = List_merge(left_sorted, right_sorted, cmp);

   if (left_sorted != left && left != NULL)
       List_destroy(left);
   if (right_sorted != right && right != NULL)
       List_destroy(right);
   if(left_sorted != NULL)
       List_destroy(left_sorted);
   if(right_sorted != NULL)
       List_destroy(right_sorted);

// fallthrough
error:
    return result;
}


List* List_merge(List* left, List* right, List_compare cmp) {
    check(left != NULL, "Partial List (left) to be sorted must exist");
    check(right != NULL, "Partial List (right) to be sorted must exist");
    List* result = List_create();

    int count_sum = left->count + right->count;
    ListNode* left_cur = left->first;
    ListNode* right_cur = right->first;

    for (int i = 0; i < count_sum; i++) {
        // assert that either node still exists
        check(!((left_cur == NULL) && (right_cur == NULL)), "Something went wrong while iterating during merge.");
        
        // if one node points to NULL, push the other
        if(left_cur == NULL) {
            List_push(result, right_cur->value);
            right_cur = right_cur->next;
            continue;
        }
        if(right_cur == NULL) {
            List_push(result, left_cur->value);
            left_cur = left_cur->next;
            continue;
        }
        if(cmp(left_cur->value, right_cur->value) <= 0) {
            // lhs appeards before rhs in lexicographical order
            List_push(result, left_cur->value);
            if (left_cur->next == NULL) left_cur = NULL;
            else left_cur = left_cur->next;
        } else {
            List_push(result, right_cur->value);
            if (right_cur->next == NULL) right_cur = NULL;
            else right_cur = right_cur->next;
        }
    }

    return result;
// fallthrough
error:
    return NULL;
}
