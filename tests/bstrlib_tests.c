#include "../src/jclib/bstrlib.h"
#include "minunit.h"

/*
    DONE
    bassignblk: Set a bstring to a C string but give the length.
    bfromcstr: Create a bstring from a C style constant.
    blk2bstr: Do the same thing, but give the length of the buffer.
    bstrcpy: Copy a bstring.
    bassigncstr: Set a bstring to a C string's contents.
    bdestroy: Destroy a bstring.
    bassign: Set one bstring to another.
    bconcat: Concatenate one bstring onto another.
    biseq: Tests if two bstrings are equal.
    binstr: Tells if one bstring is in another.
    bfindreplace: Find one bstring in another, then replace it with a third.
    bsplit: Split a bstring into a bstrList.
    bformat: Do a format string, which is super handy.
    bchar: Get a char from a bstring.

    Implicitly tested:
    blength: Get the length of a bstring.
    bdata: Get the data from a bstring.
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
  char *second = "World!";
  int ret;

  ret = bassignblk(first, second, 3);
  mu_assert(ret == BSTR_OK, "bassign not successful");
  mu_assert(blength(first) == 3, "bstring has unexpected length");
  mu_assert(strcmp(bdata(first), "Wor") == 0, "bstring has unexpected content");

  bdestroy(first);
  return NULL;
}

char *test_bassigncstr() {

  // NOTE: This requires that myStr is "defined and writeable"
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

char *test_concat() {
  bstring mylocalStr1 = bfromcstr("foo");
  bstring mylocalStr2 = bfromcstr("bar");
  int ret;

  ret = bconcat(mylocalStr1, mylocalStr2);

  mu_assert(ret == BSTR_OK, "bconcat not successful");
  mu_assert(blength(mylocalStr1) == 6, "bstring has unexpected length");
  mu_assert(strcmp(bdata(mylocalStr1), "foobar") == 0,
            "bstring has unexpected content");
  mu_assert(blength(mylocalStr2) == 3, "bstring has unexpected length");
  mu_assert(strcmp(bdata(mylocalStr2), "bar") == 0,
            "bstring has unexpected content");

  bdestroy(mylocalStr1);
  bdestroy(mylocalStr2);
  return NULL;
}

char *test_bstricmp() {
  bstring mylocalStr1 = bfromcstr("Hello");
  bstring mylocalStr2 = bfromcstr("Hellp");
  bstring mylocalStr3 = bfromcstr("Hello");

  mu_assert(bstricmp(mylocalStr1, mylocalStr2) == -1, "Comparison wrong");
  mu_assert(bstricmp(mylocalStr1, mylocalStr3) == 0, "Comparison wrong");
  mu_assert(bstricmp(mylocalStr2, mylocalStr1) == 1, "Comparison wrong");

  bdestroy(mylocalStr1);
  bdestroy(mylocalStr2);
  bdestroy(mylocalStr3);
  return NULL;
}

char *test_biseq() {
  bstring mylocalStr1 = bfromcstr("foo");
  bstring mylocalStr2 = bfromcstr("foo");

  mu_assert(biseq(mylocalStr1, mylocalStr2) == 1, "Comparison wrong");

  bassigncstr(mylocalStr2, "bar");

  mu_assert(biseq(mylocalStr1, mylocalStr2) == 0, "Comparison wrong");

  mu_assert(biseq(mylocalStr1, NULL) == BSTR_ERR,
            "Error with biseq of NULL not returned");
  mu_assert(biseq(NULL, mylocalStr2) == BSTR_ERR,
            "Error with biseq of NULL not returned");

  bdestroy(mylocalStr1);
  bdestroy(mylocalStr2);
  return NULL;
}

char *test_binstr() {
  bstring mylocalStr1 = bfromcstr("foo");
  bstring mylocalStr2 = bfromcstr("foobar");
  bstring mylocalStr3 = bfromcstr("doof");
  bstring mylocalStr4 = bfromcstr("oof");
  bstring mylocalStr5 = bfromcstr("somelongstringcontainingfoobaratposition24");

  mu_assert(binstr(mylocalStr2, 0, mylocalStr1) == 0, "foo not in foobar?");
  mu_assert(binstr(mylocalStr3, 0, mylocalStr1) == BSTR_ERR,
            "foo found in doof?");
  mu_assert(binstr(mylocalStr3, 0, mylocalStr4) == 1, "oof not in doof?");

  mu_assert(binstr(NULL, 0, mylocalStr1) == BSTR_ERR,
            "NULL in binstr not an ERR");
  mu_assert(binstr(mylocalStr2, 0, NULL) == BSTR_ERR,
            "NULL in binstr not an ERR");
  mu_assert(binstr(mylocalStr2, 5, mylocalStr1) == BSTR_ERR,
            "wrong position given, but not ERR returned");

  // NOTE: pos starts 0-indexed
  // NOTE: return value is based of original string, regardless of given pos
  mu_assert(binstr(mylocalStr5, 19, mylocalStr1) == 24,
            "Long string search wrong.");
  mu_assert(binstr(mylocalStr5, 24, mylocalStr1) == 24,
            "Long string search wrong.");

  bdestroy(mylocalStr1);
  bdestroy(mylocalStr2);
  bdestroy(mylocalStr3);
  bdestroy(mylocalStr4);
  bdestroy(mylocalStr5);
  return NULL;
}

char *test_bfindreplace() {
  // replace one
  bstring b = bfromcstr("foobar");
  bstring find = bfromcstr("bar");
  bstring repl = bfromcstr("bubble");
  int rc;

  rc = bfindreplace(b, find, repl, 0);
  mu_assert(rc == BSTR_OK, "error occurred during replace");
  mu_assert(strcmp(bdata(b), "foobubble") == 0,
            "bstring has unexpected content");

  // replace many
  bassigncstr(b, "foobar is like a bar with no gin tonic");

  rc = bfindreplace(b, find, repl, 0);
  mu_assert(rc == BSTR_OK, "error occurred during replace");
  mu_assert(strcmp(bdata(b), "foobubble is like a bubble with no gin tonic") ==
                0,
            "bstring has unexpected content");

  // vary pos
  bassign(find, repl);
  bassigncstr(repl, "twitch chat");
  rc = bfindreplace(b, find, repl, 6);
  mu_assert(rc == BSTR_OK, "error occurred during replace");
  mu_assert(strcmp(bdata(b),
                   "foobubble is like a twitch chat with no gin tonic") == 0,
            "bstring has unexpected content");

  bdestroy(b);
  bdestroy(find);
  bdestroy(repl);
  return NULL;
}

char *test_bsplit() {

  // unsplittable
  bstring input = bfromcstr("atom");
  struct bstrList *myList = bsplit(input, '.');

  mu_assert(myList->qty == 1, "should not split");
  mu_assert(bstrcmp(myList->entry[0], input) == 0, "content changed");
  bstrListDestroy(myList);

  // "normal"
  bassigncstr(input, "This. Is. Sparta. Foo.");
  bstring comp1 = bfromcstr("This");
  bstring comp2 = bfromcstr("");

  myList = bsplit(input, '.');

  mu_assert(myList != NULL, "Return of bsplit not a correct struct.");
  mu_assert(myList->qty == 5, "Length of bsplit output wrong.");
  mu_assert(bstricmp(myList->entry[0], comp1) == 0, "First result not 'This'.");
  // Last element is empty string
  mu_assert(bstricmp(myList->entry[myList->qty - 1], comp2) == 0,
            "Last result non-empty.");
  // we dont assert mlen - implementation detail
  bstrListDestroy(myList);

  // swiss cheese
  bassigncstr(input, "...");

  myList = bsplit(input, '.');

  mu_assert(myList->qty == 4, "Should have 4 empty strings.");
  bstrListDestroy(myList);

  // NULL -> NULL
  myList = bsplit(NULL, '.');
  mu_assert(myList == NULL, "bsplit should fail for NULL.");

  bdestroy(input);
  bdestroy(comp1);
  bdestroy(comp2);
  bstrListDestroy(myList);

  return NULL;
}

char *test_bformat() {
  int num = 20;
  char *name = "Jan";
  char plural = 's';
  bstring result;

  result = bformat("%s has %d monkey%c\n", name, num, plural);

  mu_assert(result != NULL, "it should not return NULL");
  mu_assert(strcmp(bdata(result), "Jan has 20 monkeys\n") == 0,
            "Content wrong");

  bstring stringception;

  stringception =
      bformat("We put the bstring <%s> into this one.", bdata(result));
  mu_assert(stringception != NULL, "it should not return NULL");
  mu_assert(blength(stringception) == 55, "wrong length of new bstring");

  bdestroy(result);
  bdestroy(stringception);
  return NULL;
}

char *test_bchar() {
  bstring b = bfromcstr("fool");

  mu_assert(bchar(b, 0) == 'f', "Index 0 wrong");
  mu_assert(bchar(b, 3) == 'l', "Index 3 wrong");

  mu_assert(bchar(b, 5) == '\0', "Wrong return for outside bounds.");
  mu_assert(bchar(b, -1) == '\0', "Wrong return for outside bounds.");

  bdestroy(b);
  return NULL;
}

char *test_destroy() {
  bdestroy(myStr);
  // NOTE: this is important, bdestroy clears the memory
  // but b still points to some address
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
  mu_run_test(test_concat);
  mu_run_test(test_bstricmp);
  mu_run_test(test_biseq);
  mu_run_test(test_binstr);
  mu_run_test(test_bfindreplace);
  mu_run_test(test_bsplit);
  mu_run_test(test_bformat);
  mu_run_test(test_bchar);

  mu_run_test(test_destroy);

  return NULL;
}

RUN_TESTS(all_tests);
