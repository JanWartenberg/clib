#include "list.h"
#include "dbg.h"

List *List_create()
{
    List *list = calloc(1, sizeof(List));
    // include a default of 0
    // Zed didn't have this, probably he was lucky with his compiler
    list->count = 0;
    return list;
}

void List_destroy(List * list)
{
    LIST_FOREACH(list, first, next, cur) {
        if (cur->prev) {
            free(cur->prev);
        }
    }

    free(list->last);
    free(list);
}

void List_clear(List * list)
{
    LIST_FOREACH(list, first, next, cur) {
        free(cur->value);
    }
}

void List_clear_destroy(List * list)
{
    List_clear(list);
    List_destroy(list);
}

void List_push(List * list, void *value)
{
    ListNode *node = calloc(1, sizeof(ListNode));
    check_mem(node);

    node->value = value;
    node->next = NULL;

    if (list->last == NULL) {
        list->first = node;
        list->last = node;
    } else {
        list->last->next = node;
        node->prev = list->last;
        list->last = node;
    }

    list->count++;

error:
    return;
}

void *List_pop(List * list)
{
    ListNode *node = list->last;
    return node != NULL ? List_remove(list, node) : NULL; 
}

void List_unshift(List * list, void *value)
{
    ListNode *node = calloc(1, sizeof(ListNode));
    check_mem(node);
    node->value = value;

    if(list->first != NULL) {
        node->next = list->first;
        list->first->prev = node;
        list->first = node;
    } else {
        list->first = node;
        list->last = node;
    }

    list->count++;

error:
    return;
}

void *List_shift(List * list)
{
    ListNode *node = list->first;
    return node != NULL ? List_remove(list, node) : NULL;
}

void *List_remove(List * list, ListNode * node)
{
    void *result = NULL;

    check(list->first && list->last, "List is empty.");
    check(node, "node can't be NULL");

    if (node == list->first && node == list->last) {
        list->first = NULL;
        list->last = NULL;
    } else if (node == list->first) {
        list->first = node->next;
        check(list->first != NULL,
                "Invalid list, somehow got a first that is NULL.");
        list->first->prev = NULL;
    } else if (node == list->last) {
        list->last = node->prev;
        check(list->last != NULL,
                "Invalid list, somehow got a next that is NULL.");
        list->last->next = NULL;
    } else {
        ListNode *after = node->next;
        ListNode *before = node->prev;
        after->prev = before;
        before->next = after;
    }

    list->count--;
    result = node->value;
    free(node);

error:
    return result;
}

void List_swap_values(ListNode *node1, ListNode *node2) {
    void * tmp = node1->value;
    node1->value = node2->value;
    node2->value = tmp;
}

void List_swap(List * list, ListNode * node1, ListNode * node2) {
    // simply smushing the prev/next around
    // this assumes that both nodes are part of the same list
    // okay.. Zed ONLY exchanged the values... m(
    //  even "worse": only with swapping values the bubble sort works
    //  while updating the pointers, the iterating logic gets screwed over 

    check(node1 != NULL, "First node to swap is NULL.");
    check(node2 != NULL, "Second node to swap is NULL.");

    //   A <-> B <-> C <-> D <-> E
    // corner cases: Swapping  first, or last or first and last??
    if (node1 == node2) { return; }

    if (node1 == list->first) {
        list->first = node2;
    }
    if (node1 == list->last) {
        list->last = node2;
    }
    if (node2 == list->first) {
        list->first = node1;
    }
    if (node2 == list->last) {
        list->last = node1;
    }
    // Swap the next and prev pointers of A and B
    ListNode *temp = node1->next;
    node1->next = node2->next;
    node2->next = temp;

    temp = node1->prev;
    node1->prev = node2->prev;
    node2->prev = temp;

    // Adjust pointers of nodes around A and B
    if (node1->next) {
        node1->next->prev = node1;
    } else {
        list->last = node1;
    }
    if (node1->prev) {
        node1->prev->next = node1;
    } else {
        list->first = node1;
    }
    if (node2->next) {
        node2->next->prev = node2;
    } else {
        list->last = node2;
    }
    if (node2->prev) {
        node2->prev->next = node2;
    } else {
        list->first = node2;
    }

error:
    return;
}


/*
 * for debugging / testing purposes
 */
void List_print(List * list) {
    // introduced this for debugging
    // TBH it relies on value being string
    ListNode *node = list->first;

    printf("Count: %d\n", List_count(list));
    if (node == NULL) {
        printf("List is empty\n");
    }

    LIST_FOREACH(list, first, next, cur) {
        printf("Value: %s\n", (char *)cur->value);
    }
}

void List_print_int(List * list) {
    // introduced this for debugging
    // TBH it relies on value being int
    ListNode *node = list->first;

    printf("Count: %d\n", List_count(list));
    if (node == NULL) {
        printf("List is empty\n");
    }

    LIST_FOREACH(list, first, next, cur) {
        printf("Value: %d\n", *(int *)cur->value);
    }
}

void* List_get_value(List * list, int n) {
    // for testing only, of course O(n) and slow
    check(n <= List_count(list), "Index out of Bounds.");
    check(list != NULL, "List is NULL!");

    int count = 0;
    LIST_FOREACH(list, first, next, cur) {
        if (n == count) {
            return cur->value;
        }
        count++;
    }

error:
    return NULL;
}
