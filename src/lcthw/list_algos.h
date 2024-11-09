#ifndef lcthw_List_algos_h
#define lcthw_List_algos_h

#include "list.h"

//typedef long int List_compare;
// a function pointer type that matches the signature of strcmp
typedef int (*List_compare)(const char *, const char *);

int List_bubble_sort(List *, List_compare);
List* List_merge_sort(List *, List_compare);

#endif
