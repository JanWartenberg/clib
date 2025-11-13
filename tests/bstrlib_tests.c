
#include "../src/lcthw/bstrlib.h"
#include "minunit.h"

bstring myStr = NULL;

char *test_create() {

  myStr = bfromcstr("Hello");
  mu_assert(myStr != NULL, "bfromcstr returned NULL");
  mu_assert(blength(myStr) == 5, "bstring has expected length");
  mu_assert(strcmp(bdata(myStr), "Hello") == 0, "bstring has unexpected content");

  return NULL;
}

char *test_destroy() {
  bdestroy(myStr);
  myStr = NULL;

  mu_assert(1, "Destroy should not crash");

  return NULL;
}



char *all_tests() {
  mu_suite_start();
  mu_run_test(test_create);



  mu_run_test(test_destroy);

  return NULL;
}

RUN_TESTS(all_tests);
