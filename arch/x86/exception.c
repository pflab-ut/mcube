/**
 * @file arch/x86/exception.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>
//============================================================================
/// @file       exception.c
/// @brief      CPU exceptions.
//
//  Copyright 2016 Brett Vickers.
//  Use of this source code is governed by a BSD-style license
//  that can be found in the MonkOS LICENSE file.
//============================================================================

static const char *exceptionstr[] = {
  "#DE: Divide by zero exception",
  "#DB: Debug exception",
  "Non-maskable interrupt",
  "#BP: Breakpoint exception",
  "#OF: Overflow exception",
  "#BR: BOUND Range exceeded exception",
  "#UD: Invalid opcode exception",
  "#NM: Device not available exception",
  "#DF: Double fault exception",
  "Coprocessor segment overrun",
  "#TS: Invalid TSS exception",
  "#NP: Segment not present exception",
  "#SS: Stack fault exception",
  "#GP: General protection exception",
  "#PF: Page fault exception",
  "Unknown exception",
  "#MF: x87 FPU floating-point error",
  "#AC: Alignment check exception",
  "#MC: Machine-check exception",
  "#XM: SIMD floating-point exception",
  "#VE: Virtualization exception",
};


static void dump_context(int id, const interrupt_context_t *context)
{
  char buf[640];
  printk("INT: %lx   Error: %lx\n\n",
         context->interrupt, context->error);
  printk("CS:RIP: %lx:%lx             SS:RSP: %lx:%lx\n\n",
         context->cs, context->retaddr, context->ss, context->rsp);


  dump_registers(&context->regs);

  dump_cpuflags(context->rflags);

  printk("Stack:\n");
  void *stack = (void *)context->rsp;
  dump_memory(buf, sizeof(buf), stack, 8 * 16, DUMPSTYLE_ADDR);
  printk("%s", buf);
}

static void hang(void)
{
  for (;;) {
    disable_interrupt();
    halt();
  }
}

static void isr_fatal(const interrupt_context_t *context)
{
  int i = context->interrupt;

  const char *exstr = i < arrsize(exceptionstr)
    ? exceptionstr[i] : "Unknown exception.";

  tty_activate(0);
  tty_set_textcolor(0, TEXTCOLOR_WHITE, TEXTCOLOR_RED);
  tty_clear(0);
  printk("%s\n\n", exstr);

  dump_context(0, context);

  hang();
}

static void isr_breakpoint(const interrupt_context_t *context)
{
  (void)context;

  printk("Breakpoint hit.\n");
}

void init_exception(void)
{
  for (int i = 0; i < 32; i++) {
    set_isr(i, isr_fatal); // fatal for now. temporary.
  }
  set_isr(0xff, isr_fatal);

  set_isr(EXCEPTION_BREAKPOINT, isr_breakpoint);
}
