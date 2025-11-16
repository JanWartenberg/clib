
#include "../src/lcthw/bstrlib.h"
#include "minunit.h"

/*
 * TODO
    bassignblk: Set a bstring to a C string but give the length.
    bconcat: Concatenate one bstring onto another.
    bstricmp: Compare two bstrings returning the same result as strcmp.
    biseq: Tests if two bstrings are equal.
    binstr: Tells if one bstring is in another.
    bfindreplace: Find one bstring in another, then replace it with a third.
    bsplit: Split a bstring into a bstrList.
    bformat: Do a format string, which is super handy.
    blength: Get the length of a bstring.
    bdata: Get the data from a bstring.
    bchar: Get a char from a bstring.

    DONE
    bfromcstr: Create a bstring from a C style constant.
    blk2bstr: Do the same thing, but give the length of the buffer.
    bstrcpy: Copy a bstring.
    bassigncstr: Set a bstring to a C string's contents.
    bdestroy: Destroy a bstring.
    bassign: Set one bstring to another.
 * */

bstring myStr = NULL;

char *test_create() {

  myStr = bfromcstr("Hello");
  mu_assert(myStr != NULL, "bfromcstr returned NULL");
  mu_assert(blength(myStr) == 5, "bstring has unexpected length");
  mu_assert(strcmp(bdata(myStr), "Hello") == 0,
            "bstring has unexpected content");

  return NULL;
}

char *test_bassign() {

  bstring first = bfromcstr("Hello");
  bstring second = bfromcstr("World!");
  int ret;

  ret = bassign(first, second);

  mu_assert(ret == BSTR_OK, "bassign not successful");
  mu_assert(blength(first) == 6, "bstring has unexpected length");
  mu_assert(strcmp(bdata(first), "World!") == 0,
            "bstring has unexpected content");
  mu_assert(bstrcmp(first, second) == 0, "assigned strings not equal");

  bdestroy(first);
  bdestroy(second);
  return NULL;
}

char *test_bassign_null() {
  bstring valid = bfromcstr("I'm fine!");
  int ret;

  // case 1: source NULL
  ret = bassign(NULL, valid);
  mu_assert(ret == BSTR_ERR, "bassign should fail when destination is NULL");

  // case 2: source NULL
  ret = bassign(valid, NULL);
  mu_assert(ret == BSTR_ERR, "bassign should fail when source is NULL");

  // case 3: both NULL
  ret = bassign(NULL, NULL);
  mu_assert(ret == BSTR_ERR, "bassign should fail with both NULL");

  bdestroy(valid);
  return NULL;
}

char *test_bassignblk() {

  bstring first = bfromcstr("Hello");
  char* second = "World!";
  int ret;

  ret = bassignblk(first, second, 3);
  mu_assert(ret == BSTR_OK, "bassign not successful");
  mu_assert(blength(first) == 3, "bstring has unexpected length");
  mu_assert(strcmp(bdata(first), "Wor") == 0,
            "bstring has unexpected content");

  bdestroy(first);
  return NULL;
}

char *test_bassigncstr() {

  bassigncstr(myStr, "Hello assignment");

  mu_assert(myStr != NULL, "bassigncstr returned NULL");
  mu_assert(blength(myStr) == 16, "bstring has unexpected length");
  mu_assert(strcmp(bdata(myStr), "Hello assignment") == 0,
            "bstring has unexpected content");

  return NULL;
}

char *test_blk2bstr() {
  bstring mylocalStr;
  mylocalStr = blk2bstr("HelloWorld!", 5);

  mu_assert(blength(mylocalStr) == 5, "bstring has unexpected length");
  mu_assert(strcmp(bdata(mylocalStr), "Hello") == 0,
            "bstring has unexpected content");

  bdestroy(mylocalStr);
  return NULL;
}

char *test_bstrcpy() {
  int ret;
  bstring mylocalStr1 = bfromcstr("Hello");
  bstring mylocalStr2;
  mylocalStr2 = bstrcpy(mylocalStr1);

  mu_assert(mylocalStr2 != NULL, "bfromcstr returned NULL");
  mu_assert(blength(mylocalStr2) == 5, "bstring has unexpected length");

  ret = bdestroy(mylocalStr2);
  mu_assert(ret == BSTR_OK, "bdestroy not successful");
  mu_assert(mylocalStr1 != NULL, "Wrong variable destroyed");
  mu_assert(blength(mylocalStr1) == 5,
            "Previous bstring has unexpected length");

  ret = bdestroy(mylocalStr1);
  mu_assert(ret == BSTR_OK, "bdestroy not successful");

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

  mu_run_test(test_bassign);
  mu_run_test(test_bassign_null);
  mu_run_test(test_bassignblk);
  mu_run_test(test_bassigncstr);
  mu_run_test(test_blk2bstr);
  mu_run_test(test_bstrcpy);

  mu_run_test(test_destroy);

  return NULL;
}

RUN_TESTS(all_tests);
