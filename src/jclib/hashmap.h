#ifndef JCLIB_HASHMAP_H
#define JCLIB_HASHMAP_H

#include "darray.h"
#include <stdint.h>

// number of buckets for the overall hashmap
#define DEFAULT_NUMBER_OF_BUCKETS 100
// size of DArray, of one bucket - to handle a few collisions
#define INITIAL_BUCKET_SIZE 5

typedef int (*Hashmap_compare)(void *a, void *b);
typedef uint32_t (*Hashmap_hash)(void *key);

typedef struct Hashmap {
  DArray *buckets;
  Hashmap_compare compare;
  Hashmap_hash hash;

  size_t bucket_count; // current
  size_t count;        // current number of key_value pairs
  float load_limit;
} Hashmap;

typedef struct HashmapNode {
  void *key;
  void *data;
  uint32_t hash;
} HashmapNode;

typedef int (*Hashmap_traverse_cb)(HashmapNode *node);

Hashmap *Hashmap_create(Hashmap_compare compare, Hashmap_hash);
void Hashmap_destroy(Hashmap *map);
int Hashmap_resize(Hashmap *map, size_t new_size);

int Hashmap_set(Hashmap *map, void *key, void *data);
void *Hashmap_get(Hashmap *map, void *key);

int Hashmap_traverse(Hashmap *map, Hashmap_traverse_cb traverse_cb);

void *Hashmap_delete(Hashmap *map, void *key);

#endif
