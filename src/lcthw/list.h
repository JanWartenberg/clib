#ifndef lcthw_List_h
#define lcthw_List_h

#include <stdlib.h>

struct ListNode;

typedef struct ListNode {
  struct ListNode *next;
  struct ListNode *prev;
  void *value;
} ListNode;

typedef struct List {
  int count;
  ListNode *first;
  ListNode *last;
} List;

List *List_create();
void List_destroy(List *list);
void List_clear(List *list);
void List_clear_destroy(List *list);

#define List_count(A) ((A)->count)
#define List_first(A) ((A)->first != NULL ? (A)->first->value : NULL)
#define List_last(A) ((A)->last != NULL ? (A)->last->value : NULL)

void List_push(List *list, void *value);
void *List_pop(List *list);

void List_unshift(List * list, void *value);
void *List_shift(List * list);

void *List_remove(List *list, ListNode *node);

/* Usage:
   LIST_FOREACH(list, first, next, cur) { ... }
*/
#define LIST_FOREACH(L, S, M, V) ListNode *_node = NULL;\
                                                   ListNode *V = NULL;\
for(V = _node = L->S; _node != NULL; V = _node = _node->M)

void List_swap(List *list, ListNode *node1, ListNode *node2);
void List_swap_values(ListNode *node1, ListNode *node2);

// for debugging
void List_print(List *list);
void List_print_int(List * list);
// for swap testing
void* List_get_value(List *list, int n);

#endif
