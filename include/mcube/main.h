/**
 * @file include/mcube/main.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_MAIN_H__
#define __MCUBE_MCUBE_MAIN_H__


#define NR_THREADS 16

/* 4K */
#define KERNEL_STACK_SIZE 0x1000
#define USER_STACK_SIZE 0x1000


#define NR_PRIORITIES NR_THREADS

/* each bitmap length is 32 bit */
#define NR_PRIORITY_BITMAPS CEIL(NR_PRIORITIES, 32)

#ifndef  __ASSEMBLY__

#define inf_loop() do {                                                 \
    print("%s:%s():%d %s\n", __FILE__, __func__, __LINE__, "inf_loop()"); \
    if (call_sys_get_mode_level() != USER_LEVEL) {                      \
      disable_local_irq();                                              \
    }                                                                   \
    for (;;)                                                            \
      ;                                                                 \
  } while (0)


extern unsigned int Debug;

int main(int argc, char *argv[]);

void init_arch(void);
void init_arch_ap(void);
void exit_arch(void);
void exit_arch_ap(void);

void clear_bss(void);


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_MAIN_H__ */
