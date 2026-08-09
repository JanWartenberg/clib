#ifndef JCLIB_BSTREE_H
#define JCLIB_BSTREE_H

typedef int (*BSTree_compare)(void *a, void *b);

typedef struct BSTreeNode {
  void *key;
  void *data;

  struct BSTreeNode *left;
  struct BSTreeNode *right;
  struct BSTreeNode *parent;
} BSTreeNode;

typedef struct BSTree {
  int count;
  BSTree_compare compare;
  BSTreeNode *root;
} BSTree;

/**
 * traverse_cb:
 * - Called for each node during traversal.
 * - Return 0 to continue traversal.
 * - Return non-zero to stop traversal immediately.
 *
 * Traversal order: left subtree, right subtree, node.
 */
typedef int (*BSTree_traverse_cb)(BSTreeNode *node);

BSTree *BSTree_create(BSTree_compare compare);
void BSTree_destroy(BSTree *map);

int BSTree_set(BSTree *map, void *key, void *data);
void *BSTree_get(BSTree *map, void *key);

/**
 * BSTree_traverse:
 * Walks the tree and calls traverse_cb for each node.
 *
 * Returns:
 * - 0 if traversal completed successfully
 * - non-zero if traversal was stopped by traverse_cb
 */
int BSTree_traverse(BSTree *map, BSTree_traverse_cb traverse_cb);

void *BSTree_delete(BSTree *map, void *key);

#endif
