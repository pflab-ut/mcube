/**
 * @file arch/arm/init.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


__noreturn void shell(void)
{
  int c;

  while (true) {
    c = uart_getc(0);

    if (c == '\r') {
      uart_putc('\n', 0);
    } else {
      uart_putc(c, 0);
    }
  }
}

void clear_bss(void)
{
  extern unsigned long __bss_begin, __bss_end;
  //  printk("&__bss_begin = 0x%lx &__bss_end = 0x%lx\n", &__bss_begin, &__bss_end);
  memset64(&__bss_begin, 0, (&__bss_end - &__bss_begin) / sizeof(unsigned long));
}

void init_arch(void)
{
  clear_bss();
  init_cpu();
  init_uart();
  init_timer(TICK_USEC);
  init_dmac();
  init_kmalloc();
#if CONFIG_ARCH_ARM_RASPI3
  init_rand_raspi3();
#endif /* CONFIG_ARCH_ARM_RASPI3 */
  init_irq();
  init_socket();
}

void init_arch_ap(void)
{
  //  init_timer(TICK_USEC);
  init_irq();
}

void exit_arch(void)
{
}

void exit_arch_ap(void)
{
}
\
