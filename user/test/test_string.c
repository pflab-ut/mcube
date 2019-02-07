/**
 * @file user/test/test_string.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

#if CONFIG_ARCH_X86

static void test_strnlen(const char *str, int len, int expected_len, bool print)
{
  int res;

  res = strnlen(str, len);
  if (res != expected_len) {
    panic("_STRING - strnlen(\"%s\", %d) returned %d, while %d "
          "is expected", (print) ? str : "<binary>", len, res,
          expected_len);
  }

  print_uart("_STRING - strnlen(\"%s\", %d) = %d. Success!\n",
             (print) ? str : "<binary>", len, res);
}

#define _ARRAY_LEN  100
static uint8_t _arr[_ARRAY_LEN];

static void test_memcpy_overlaps(void)
{
  memset(_arr, 0x55, _ARRAY_LEN);

  /* Should succeed */

  memcpy(_arr, _arr + 20, 10);    /* Regular */
  memcpy(_arr + 20, _arr, 10);    /* Regular */
  memcpy(_arr, _arr + 20, 20);    /* Regular, bounds */
  memcpy(_arr + 20, _arr, 20);    /* Regular, bounds */

  memcpy_forward(_arr, _arr + 20, 10);  /* Regular */
  memcpy_forward(_arr + 20, _arr, 10);  /* Regular */
  memcpy_forward(_arr, _arr + 20, 20);  /* Regular, bounds */
  memcpy_forward(_arr + 20, _arr, 20);  /* Regular, bounds */
  memcpy_forward(_arr, _arr + 10, 20);  /* Good overlap */
  memcpy_forward(_arr, _arr + 10, 11);  /* Good overlap, bounds */

  /* Should fail */

  /* memcpy(_arr, _arr + 20, 40);    /\* Overlap *\/ */
  /* memcpy(_arr + 20, _arr, 40);    /\* Overlap *\/ */
  /* memcpy(_arr, _arr + 20, 21);    /\* Overlap, bounds *\/ */
  /* memcpy(_arr + 20, _arr, 21);    /\* Overlap, bounds *\/ */

  /* memcpy_forward(_arr, _arr + 7, 10);  /\* Good overlap, but < 8-byte *\/ */
  /* memcpy_forward(_arr + 20, _arr, 40);  /\* Bad overlap *\/ */
  /* memcpy_forward(_arr + 20, _arr, 21);  /\* Bad overlap, bounds *\/ */
}

bool test_string(void)
{
  char *str;
  int i;

  /* Strnlen() tests */

  for (i = 0; i <= 10; i++) {
    test_strnlen("", i, 0, true);  /* Bounds, 1 */
  }
  
  i = 1;
  str = kmalloc(1024);
  for (char ch = 'A'; ch <= 'Z'; ch++, i++) {
    str[i - 1] = ch;
    str[i] = '\0';
    test_strnlen(str, 0, 0, true);  /* Bounds, 2 */
  }

  i = 1;
  for (char ch = 'A'; ch <= 'Z'; ch++, i++) {
    str[i - 1] = ch;
    str[i] = '\0';
    test_strnlen(str, 1024, i, 1);  /* Test it as a regular strlen */
  }

  for (i = 0; i <= 'Z' - 'A' + 1; i++) {
    test_strnlen(str, i, i, true);  /* The the 'n' part of strnlen */
  }
  
  kfree(str);
  memset(_arr, 0x01, _ARRAY_LEN);
  for (i = 0; i <= _ARRAY_LEN; i++) {
    test_strnlen((char *)_arr, i, i, 0);   /* Without NULL suffix! */
  }
  
  /* Memcpy() tests */

  test_memcpy_overlaps();
  return true;
}


#else

bool test_string(void)
{
  return true;
}


#endif
