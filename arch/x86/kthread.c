/**
 * @file arch/x86/kthread.c
 *
 * @author Hiroyuki Chishiro
 */
/*
 * Kernel threads
 *
 * Copyright (C) 2010 Ahmed S. Darwish <darwish.07@gmail.com>
 */

#include <mcube/mcube.h>

/*
 * Allocate a unique thread ID
 */
uint64_t kthread_alloc_pid(void)
{
  static uint64_t pids;

  return atomic_inc(&pids);
}

/*
 * Create a new kernel thread running given function
 * code, and attach it to the runqueue.
 *
 * NOTE! given function must never exit!
 */
void kthread_create(void (* /* __noreturn */ func)(void))
{
  struct proc *proc;
  struct irq_ctx *irq_ctx;
  char *stack;
  char *stack_start;

  if (!(proc = kmalloc(sizeof(*proc)))) {
    panic("Error: cannot allocate memory %lu\n", sizeof(*proc));
  }

  proc_init(proc);

  /* New thread stack, moving down */
  if (!(stack_start = (char *) kmalloc(STACK_SIZE))) {
    panic("Error: cannot allocate memory %lu\n", sizeof(STACK_SIZE));
  }

  stack = stack_start + STACK_SIZE;

  /* Reserve space for our IRQ stack protocol */
  irq_ctx = (struct irq_ctx *)(stack - sizeof(*irq_ctx));
  irq_ctx_init(irq_ctx);

  /*
   * Values for the code to-be-executed once scheduled.
   * They will get popped and used automatically by the
   * processor at ticks handler `iretq'.
   *
   * Set to-be-executed code's %rsp to the top of the
   * newly allocated stack since this new code doesn't
   * care about the values currently 'pushed'; only
   * the ctontext switching code does.
   */
  irq_ctx->cs = KERNEL_CS;
  irq_ctx->rip = (uintptr_t)func;
  irq_ctx->ss = 0;
  irq_ctx->rsp = (uintptr_t)stack;
  irq_ctx->rflags = default_rflags().raw;

  /* For context switching code, which runs at the
   * ticks handler context, give a stack that respects
   * our IRQ stack protocol */
  proc->pcb.rsp = (uintptr_t)irq_ctx;

  /* Push the now completed proc to the runqueu */
  sched_enqueue(proc);
}
