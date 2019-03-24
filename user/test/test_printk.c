/**
 * @file user/test/test_printk.c
 *
 * @author Hiroyuki Chishiro
 */
/*
 * Copyright (C) 2009-2010 Ahmed S. Darwish <darwish.07@gmail.com>
 */

#include <mcube/mcube.h>


/*
 * Minimal printk test cases
 */

#define PRINT_UART_TESTS 1

#if PRINT_UART_TESTS
#define printk(fmt, ...)  print_uart(fmt, ##__VA_ARGS__)
#define putchar_colored(c, col)  serial_putc(c)
#define putchar(c)    serial_putc(c)
#endif /* PRINTS_TESTS */

static void printk_test_int(void)
{
  printk("(-10, 10): ");

  for (int i = -10; i <= 10; i++) {
    printk("%d ", i);
  }

  printk("\n");

  printk("(INT64_MIN, 0xINT64_MIN + 10): ");
  int64_t start = (INT64_MAX * -1) - 1;

  for (int64_t i = start; i <= start + 10; i++) {
    printk("%ld ", i);
  }

  printk("\n");
}

static void printk_test_hex(void)
{
  printk("(0x0, 0x100): ");

  for (int i = 0; i <= 0x100; i++) {
    printk("0x%x ", i);
  }

  printk("\n");

  printk("(0xUINT64_MAX, 0xUINT64_MAX - 0x10): ");

  for (uint64_t i = UINT64_MAX; i >= UINT64_MAX - 0x10; i--) {
    printk("0x%lx ", i);
  }

  printk("\n");
}

static void printk_test_string(void)
{
  const char *test1, *test2, *test3;

  printk("(a, d): ");
  printk("a");
  printk("b");
  printk("c");
  printk("d");
  printk("\n");

  printk("(a, z): ");

  for (char c = 'a'; c <= 'z'; c++) {
    printk("%c ", c);
  }

  printk("\n");

  test1 = "Test1";
  test2 = "Test2";
  test3 = NULL;
  printk("Tests: %s %s %s\n", test1, test2, test3);
}

/*
 * Test printk reaction to NULL and incomplete
 * C printf expressions
 */
static char tmpbuf[100];
__unused static void printk_test_format(void)
{
  const char *fmt;
  int len;

  for (int i = 0; i < 0x1000; i++) {
    printk("");
  }

  /* This code is expected to panic.
   * Modify loop counter manually */
  fmt = "%d %x %ld";
  len = strlen(fmt);

  for (int i = 0; i <= len; i++) {
    printk("[%d] ", i);

    strncpy(tmpbuf, fmt, len);
    tmpbuf[i] = 0;
    printk(tmpbuf, 1, 2, 3);

    printk("\n");
  }
}

static void printk_test_colors(void)
{

  printk("Colored text: ");
#if PRINT_UART_TESTS
  serial_putc('A');
  serial_putc('A');
  serial_putc('A');
  serial_putc('A');
  serial_putc('A');
  serial_putc('A');
  serial_putc('A');
  serial_putc('A');
  serial_putc('A');
  serial_putc('A');
  serial_putc('A');
  serial_putc('A');
  serial_putc('A');
  serial_putc('A');
  serial_putc('A');
  serial_putc('A');
#else
  uint8_t color = VGA_COLOR(VGA_BLACK, 0);
  putchar_colored('A', color | VGA_BLACK);
  putchar_colored('A', color | VGA_BLUE);
  putchar_colored('A', color | VGA_GREEN);
  putchar_colored('A', color | VGA_CYAN);
  putchar_colored('A', color | VGA_RED);
  putchar_colored('A', color | VGA_MAGNETA);
  putchar_colored('A', color | VGA_BROWN);
  putchar_colored('A', color | VGA_LIGHT_GRAY);
  putchar_colored('A', color | VGA_GRAY);
  putchar_colored('A', color | VGA_LIGHT_BLUE);
  putchar_colored('A', color | VGA_LIGHT_GREEN);
  putchar_colored('A', color | VGA_LIGHT_CYAN);
  putchar_colored('A', color | VGA_LIGHT_RED);
  putchar_colored('A', color | VGA_LIGHT_MAGNETA);
  putchar_colored('A', color | VGA_YELLOW);
  putchar_colored('A', color | VGA_WHITE);
#endif
  printk("\n");
}

bool test_printk(void)
{
  printk_test_int();
  printk_test_hex();
  printk_test_string();
  //  printk_test_format();
  printk_test_colors();
  return true;
}
