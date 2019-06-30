/**
 * @file kernel/format.c
 *
 * @author Hiroyuki Chishiro
 */
/*
 * printf()-like methods: vsnprintf(), etc
 *
 * Copyright (C) 2009-2010 Ahmed S. Darwish <darwish.07@gmail.com>
 */

#include <mcube/mcube.h>


/**
 * @fn panic_prefix[]
 * @brief Panix prefix.
 */
static char panic_prefix[] = "PANIC: ";


/*
 * We use putchar() as it directly invokes the VGA code.
 * NOTE! Don't use any assert()s in this function!
 */
__noreturn void format_panic(const char *str)
{
  const char *prefix;

  prefix = panic_prefix;

  while (*prefix != 0) {
    putchar(*prefix++);
  }

  while (*str != 0) {
    putchar(*str++);
  }

  halt();
}


const char *parse_arg(const char *fmt, struct format_argdesc *desc)
{
  bool complete;
  unsigned int digit_size;

  format_assert(*fmt == '%');

  complete = false;
  desc->len = INT;
  desc->type = NONE;
  desc->radix = 10;
  desc->pad = PAD_NO;
  desc->digit = INIT_DIGIT;
  desc->float_digit = INIT_FLOAT_DIGIT;

  while (*++fmt) {
    switch (*fmt) {
    case 'l':
      desc->len = LONG;
      break;

    case 'd':
      desc->type = SIGNED;
      complete = true;
      goto out;

    case 'u':
      desc->type = UNSIGNED;
      complete = true;
      goto out;

    case 'x':
      desc->type = UNSIGNED;
      desc->radix = 16;
      complete = true;
      goto out;
#if defined(ENABLE_FPU)

    case 'f':
      desc->type = FLOAT;
      complete = true;
      goto out;
#endif /* ENABLE_FPU */

    case 's':
      desc->type = STRING;
      complete = true;
      goto out;

    case 'c':
      desc->type = CHAR;
      complete = true;
      goto out;

    case '%':
      desc->type = PERCENT;
      complete = true;
      goto out;

    case '0' ... '9':
    case '.':
      if (*fmt == '0') {
        desc->pad = PAD_ZERO;
        fmt++;
      } else {
        desc->pad = PAD_BLANK;
      }

      /* integer part */
      digit_size = 0;

      while ((*fmt >= '0') && (*fmt <= '9')) {
        digit_size = digit_size * 10 + (*fmt - '0');
        fmt++;
      }

      desc->digit = digit_size;
#if defined(ENABLE_FPU)
      /* float part */
      digit_size = 0;

      if (*fmt == '.') {
        fmt++;

        while ((*fmt >= '0') && (*fmt <= '9')) {
          digit_size = digit_size * 10 + (*fmt - '0');
          fmt++;
        }

        if (digit_size > 0) {
          desc->float_digit = digit_size;
        }
      }

#endif /* ENABLE_FPU */
      fmt--;
      break;

    default:
      /* Unknown mark: complete by definition */
      //print("Unknown mark: %d\n", *fmt);
      desc->type = NONE;
      complete = true;
      goto out;
    }
  }

out:

  if (!complete) {
    format_panic("Unknown/incomplete expression");
  }

  /* Bypass last expression char */
  if (*fmt != '\0') {
    fmt++;
  }

  return fmt;
}

