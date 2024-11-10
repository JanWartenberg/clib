#include "list_algos.h"
#include "dbg.h"
#include "list.h"

int List_bubble_sort(List * list, List_compare cmpfunc) {
    check(list != NULL, "List to be sorted must exist");

    if (List_count(list) <= 1) {
        return 0;   // already sorted
    }

    int sorted = 1;
    do {
        sorted = 1;
        LIST_FOREACH(list, first, next, cur) {
            if (cur->next) {
                if (cmpfunc(cur->value, cur->next->value) > 0) {
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

List* List_merge_sort(List * list, List_compare passedfunc) {
    check(list != NULL, "List to be sorted must exist");

/* 
 * TODO
 *
 * */

error:
    return list;
}
