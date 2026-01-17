#include "../src/jclib/bstrlib.h"
#include "../src/jclib/darray.h"
#include "../src/jclib/hashmap.h"
#include "../src/jclib/hashmap_algos.h"
#include "minunit.h"

struct tagbstring test1 = bsStatic("test data 1");
struct tagbstring test2 = bsStatic("test data 2");
struct tagbstring test3 = bsStatic("xest data 3");

char *test_fnv1a() {
  uint32_t hash = Hashmap_fnv1a_hash(&test1);
  mu_assert(hash != 0, "Bad hash.");

  hash = Hashmap_fnv1a_hash(&test2);
  mu_assert(hash != 0, "Bad hash.");

  hash = Hashmap_fnv1a_hash(&test3);
  mu_assert(hash != 0, "Bad hash.");

  return NULL;
}

char *test_adler32() {
  uint32_t hash = Hashmap_adler32_hash(&test1);
  mu_assert(hash != 0, "Bad hash.");

  hash = Hashmap_adler32_hash(&test2);
  mu_assert(hash != 0, "Bad hash.");

  hash = Hashmap_adler32_hash(&test3);
  mu_assert(hash != 0, "Bad hash.");

  return NULL;
}

char *test_djb() {
  uint32_t hash = Hashmap_djb_hash(&test1);
  mu_assert(hash != 0, "Bad hash.");

  hash = Hashmap_djb_hash(&test2);
  mu_assert(hash != 0, "Bad hash.");

  hash = Hashmap_djb_hash(&test3);
  mu_assert(hash != 0, "Bad hash.");

  return NULL;
}

char *test_bad4() {
  uint32_t hash = Hashmap_bad4_hash(&test1);
  mu_assert(hash != 0, "Bad hash.");

  hash = Hashmap_bad4_hash(&test2);
  mu_assert(hash != 0, "Bad hash.");

  hash = Hashmap_bad4_hash(&test3);
  mu_assert(hash != 0, "Bad hash.");

  return NULL;
}

#define BUCKETS 100
#define BUFFER_LEN 20
#define NUM_KEYS BUCKETS * 1000
enum { ALGO_FNV1A, ALGO_ADLER32, ALGO_DJB, ALGO_BAD4 };

int gen_keys(DArray *keys, int num_keys) {
  int i = 0;
  int result = -1; // default to failure condition
  struct bStream *stream = NULL;
  bstring key = bfromcstr("");
  FILE *urand = fopen("/dev/urandom", "r");
  check(urand != NULL, "Failed to open /dev/urandom");
  int rc = 0;

  stream = bsopen((bNread)fread, urand);
  check(stream != NULL, "Failed to open /dev/urandom");

  // FNV1a histogram
  for (i = 0; i < num_keys; i++) {
    rc = bsread(key, stream, BUFFER_LEN);
    check(rc >= 0, "Failed to read from /dev/urandom.");

    DArray_push(keys, bstrcpy(key));
  }

  result = 0; // all good

error: // fallthrough
  if (stream)
    bsclose(stream);
  if (urand)
    fclose(urand);
  if (key)
    bdestroy(key);
  return result;
}

void destroy_keys(DArray *keys) {
  int i = 0;
  for (i = 0; i < NUM_KEYS; i++) {
    bdestroy(DArray_get(keys, i));
  }

  DArray_destroy(keys);
}

static int cmp_int(const void *a, const void *b) {
  int ia = *(const int *)a;
  int ib = *(const int *)b;
  return (ia > ib) - (ia < ib);
}

static void print_summary(const char *name, const int *stats) {
  int sorted[BUCKETS];
  int i = 0;

  for (i = 0; i < BUCKETS; i++) {
    sorted[i] = stats[i];
  }

  qsort(sorted, BUCKETS, sizeof(int), cmp_int);

  fprintf(stderr, "%s\tmin=%d\tq1=%d\tmed=%d\tq3=%d\tmax=%d\n", name, sorted[0],
          sorted[BUCKETS / 4], sorted[BUCKETS / 2], sorted[(3 * BUCKETS) / 4],
          sorted[BUCKETS - 1]);
}

void fill_distribution(int *stats, DArray *keys, Hashmap_hash hash_func) {
  int i = 0;
  uint32_t hash = 0;

  for (i = 0; i < DArray_count(keys); i++) {
    hash = hash_func(DArray_get(keys, i));
    stats[hash % BUCKETS] += 1;
  }
}

char *test_distribution() {
  int i = 0;
  int stats[4][BUCKETS] = {{0}};
  DArray *keys = DArray_create(0, NUM_KEYS);

  mu_assert(gen_keys(keys, NUM_KEYS) == 0, "Failed to generate random keys.");

  fill_distribution(stats[ALGO_FNV1A], keys, Hashmap_fnv1a_hash);
  fill_distribution(stats[ALGO_ADLER32], keys, Hashmap_adler32_hash);
  fill_distribution(stats[ALGO_DJB], keys, Hashmap_djb_hash);
  fill_distribution(stats[ALGO_BAD4], keys, Hashmap_bad4_hash);

  // print summary
  fprintf(stderr, "\nBucket counts (100 buckets, %d keys)\n", NUM_KEYS);
  fprintf(stderr, "FNV\tA32\tDJB\tBAD4\n");
  print_summary("FNV", stats[ALGO_FNV1A]);
  print_summary("A32", stats[ALGO_ADLER32]);
  print_summary("DJB", stats[ALGO_DJB]);
  print_summary("BAD4", stats[ALGO_BAD4]);
  fprintf(stderr, "\n");

  // print all counts (number of keys per bucket for each hash algorithm)
  for (i = 0; i < BUCKETS; i++) {
    fprintf(stderr, "%d\t%d\t%d\t%d\n", stats[ALGO_FNV1A][i],
            stats[ALGO_ADLER32][i], stats[ALGO_DJB][i], stats[ALGO_BAD4][i]);
  }

  destroy_keys(keys);

  return NULL;
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_fnv1a);
  mu_run_test(test_adler32);
  mu_run_test(test_djb);
  mu_run_test(test_bad4);
  mu_run_test(test_distribution);

  return NULL;
}

RUN_TESTS(all_tests);
