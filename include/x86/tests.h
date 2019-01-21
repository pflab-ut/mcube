/**
 * @file include/x86/tests.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_TESTS_H__
#define __MCUBE_X86_TESTS_H__
/*
 * Testcases switches
 *
 * Copyright (C) 2010 Ahmed S. Darwish <darwish.07@gmail.com>
 */

#define    LIST_TESTS    0  /* Linked stack/queue tests */
#define    UNROLLED_TESTS    0  /* Unrolled linked list tests */
#define    HASH_TESTS    0  /* Hash structure tests */
#define    BITMAP_TESTS    0  /* Operations on a bitmap */
#define    STRING_TESTS    0  /* Optimized string methods */
#define    PRINTK_TESTS    0  /* printk(fmt, ...) */
#define    PRINTS_TESTS    0  /* prints(fmt, ...) */
#define    VM_TESTS    0  /* Kernel VM mappings */
#define    PAGEALLOC_TESTS    0  /* Page allocator */
#define    KMALLOC_TESTS    0  /* Dynamic memory allocator */
#define    PIT_TESTS    0  /* PIT timer tests */
#define    APIC_TESTS    0  /* Local APIC timer and IPI */
#define    PERCPU_TESTS    0  /* Per-CPU Data Area tests */
#define    ATOMIC_TESTS    0  /* Atomic-accessors */
#define    SCHED_TESTS    0  /* Scheduler tests */
#define    EXT2_TESTS    0  /* File System tests */
#define    EXT2_SMP_TESTS    0  /* SMP file system tests */
#define    FILE_TESTS    0  /* Unix file operations */

#if  (SCHED_TESTS == 1) && (PIT_TESTS == 1)
#error  Cannot run scheduler test cases with the PIT ones:\
  PIT tests invalidate scheduler timer ticks setup.
#endif


#if  (PRINTK_TESTS == 1) && (PRINTS_TESTS == 1)
#error  Cannot simultaneously run printk() VGA tests\
  with the serial-port ones.
#endif


#endif /* __MCUBE_X86_TESTS_H__ */
