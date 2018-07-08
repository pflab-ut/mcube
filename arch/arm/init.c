/**
 * @file arch/arm/init.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


void uart_puts(const char* str)
{
  int i;
  for (i = 0; str[i] != '\0'; i ++) {
    uart_putc((unsigned char) str[i], 0);
  }
}

void shell(void)
{
  int c;
  while (1) {
    c = uart_getc(0);
    if (c == '\r') {
      uart_putc('\n', 0);
    } else {
      uart_putc(c, 0);
    }
  }
}

void process(char *array)
{
  while (1) {
    uart_puts(array);
    //    delay(100);
  }
}


void kernel_main(void)
{
  unsigned long reg, sp;
  int ret;
  //  asm volatile("mrc p15, 0, r0, c0, c1, 0");
  //  asm volatile("MRC p15,0,reg,c0,c0,5");
  //  asm volatile("mrs %0, MIDR_EL1" : "=r"(reg));
  if (get_cpu_id() == 0) {
#if 1
    main(0, NULL);
#else
    init_arch();
    printk("Hello, kernel World!\n");
    
    asm volatile("mrs %0, mpidr_el1" : "=r"(reg));
    printk("0x%lx\n", reg);
    asm volatile("mov %0, sp" : "=r"(sp));
    printk("sp = 0x%lx\n", sp);

    printk("CurrentEL = 0x%lx\n", get_current_el());

    start_timer(0);
    ret = copy_process((unsigned long) &process, (unsigned long) "12345");
    if (ret != 0) {
      printk("Error while starting process1\n");
      return;
    }
    ret = copy_process((unsigned long) &process, (unsigned long) "abcde");
    if (ret != 0) {
      printk("Error while starting process 2");
      return;
    }
    while (1) {
      //printk("get_cntv_tval_el0() = %lu\n", get_cntv_tval_el0());
      //      wfi();
      do_sched();
    }
#endif
  } else {
    for (;;)
      ;
  }
  
}


void init_arch(void)
{
  init_uart();
  init_irq();
  init_cpu();
  init_timer(TICK_USEC);
  init_mm();
}

void exit_arch(void)
{
}
