#ifndef JCLIB_LIST_ALGOS_H
#define JCLIB_LIST_ALGOS_H

#include "list.h"

/* typedef long int List_compare;
a function pointer type that matches the signature of strcmp

originally:
    typedef int (*List_compare)(const char *, const char *);
but with void pointers we are generic

to compare linked lists of string -> use strcmp
to compate linked lists of int -> compare_int
*/
typedef int (*List_compare)(const void *, const void *);
int compare_int(const void *, const void *);

int List_bubble_sort(List *, List_compare);
List *List_merge_sort(List *, List_compare);
List *List_merge(List *, List *, List_compare);

#endif
