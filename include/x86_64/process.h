/**
 * @file include/x86_64/process.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_64_PROCESS_H__
#define __MCUBE_X86_64_PROCESS_H__

/*
 * Processes and their related definitions
 *
 * Copyright (C) 2010 Ahmed S. Darwish <darwish.07@gmail.com>
 *
 * Please also check comments on top of the timer handler (idt.S)
 */

#ifndef __ASSEMBLY__


/**
 * @struct irq_ctx
 * @brief IRQ 'stack protocol'.
 *
 * Stack view for any code executed at IRQ context
 * (.e.g. context-switching code, all handlers, ..)
 */
struct irq_ctx {
  /*
   * ABI scratch registers. Pushed by us on IRQ
   * handlers entry to freely call C code from
   * those handlers without corrupting state.
   */

  /**
   * 0x0 (%rsp).
   */
  uint64_t r11;

  /**
   * 0x8 (%rsp).
   */
  uint64_t r10;

  /**
   * 0x10(%rsp).
   */
  uint64_t r9;

  /**
   * 0x18(%rsp).
   */
  uint64_t r8;

  /**
   * 0x20(%rsp).
   */
  uint64_t rsi;

  /**
   * 0x28(%rsp).
   */
  uint64_t rdi;

  /**
   * 0x30(%rsp).
   */
  uint64_t rdx;

  /**
   * 0x38(%rsp).
   */
  uint64_t rcx;

  /**
   * 0x40(%rsp).
   */
  uint64_t rax;

  /*
   * Regs pushed by CPU directly before invoking IRQ handlers.
   *
   * Those are %RIP and stack of the _interrupted_
   * code, where all handlers including the ticks
   * context-switching code will 'resume' to.
   */

  /**
   * 0x48(%rsp).
   */
  uint64_t rip;

  /**
   * 0x50(%rsp).
   */
  uint64_t cs;

  /**
   * 0x58(%rsp).
   */
  uint64_t rflags;

  /**
   * 0x60(%rsp).
   */
  uint64_t rsp;

  /**
   * 0x68(%rsp).
   */
  uint64_t ss;
};

/**
 * @fn static inline void irq_ctx_init(struct irq_ctx *ctx)
 * @brief initialize IRQ context.
 *
 * @param ctx IRQ context.
 */
static inline void irq_ctx_init(struct irq_ctx *ctx)
{
  memset64(ctx, 0xdeadfeeddeadfeed, sizeof(*ctx));
}

/**
 * @struct pcb
 * @brief Process control block.
 *
 * Process Control Block (PCB) holds machine-dependent state that
 * get swapped during a context switch.
 *
 * We already save ABI-mandated scratch registers upon ticks
 * handler entry, so we don't save them again here. They will
 * get their values restored at the end of the handler.
 *
 * The stack value here was either:
 * [1] filled by a previous context switch
 * [2] manually set at thread creation time
 *
 * In ALL ways, below stack MUST respect our IRQ stack protocol.
 * After moving to a new process (current = next), the context
 * switch code will set its stack with this value, and such code
 * runs at IRQ context of the timer handler.
 *
 * At [1], we respect the protocol by definition. In [2], we
 * set the %rsp value with an emulated stack that respects our
 * protocol.
 */
struct pcb {
  /**
   * RBP.
   */
  uint64_t rbp;

  /**
   * RBX.
   */
  uint64_t rbx;

  /**
   * R12.
   */
  uint64_t r12;

  /**
   * R13.
   */
  uint64_t r13;

  /**
   * R14.
   */
  uint64_t r14;

  /**
   * R15.
   */
  uint64_t r15;

  /**
   * RSP.
   */
  uint64_t rsp;
};


/**
 * @fn static inline void pcb_init(struct pcb *pcb)
 * @brief initialize PCB.
 *
 * @param pcb PCB.
 */
static inline void pcb_init(struct pcb *pcb)
{
  memset64(pcb, 0xdeadfeeddeadfeed, sizeof(*pcb));
}

/*
 * EACH thread must have a unique stack resident in memory to let
 * the kernel freely call functions and service interrupts. Thus,
 * keep them small to avoid wasting too much physical RAM.
 */

/**
 * @def STACK_SIZE
 * @brief Alias to @c PAGE_SIZE.
 */
#define STACK_SIZE PAGE_SIZE

/**
 * @struct process
 * @brief Process descriptor; one for each process.
 */
struct process {
  /**
   * Process ID.
   */
  uint64_t pid;

  /**
   * Hardware state (for ctxt switch).
   */
  struct pcb pcb;

  /**
   * Current process state.
   */
  int state;

  /**
   * for the runqueue lists.
   */
  struct list_node pnode;

  /**
   * # ticks running on the CPU.
   */
  clock_t runtime;

  /**
   * Timestamp runqueue entrance.
   */
  clock_t enter_runqueue_ts;

  /**
   * Inode# of Current Working Dir.
   */
  uint64_t working_dir;

  /**
   * File Descriptor Table.
   */
  struct unrolled_head fdtable;

  struct {      /* Scheduler statistics .. */
    /**
     * Overall runtime (in ticks).
     */
    clock_t runtime_overall;

    /**
     * # got chosen from the runqueue.
     */
    uint dispatch_count;

    /**
     * Overall wait in runqueue (ticks).
     */
    clock_t rqwait_overall;

    /**
     * # runtime ticks at priority i.
     */
    clock_t prio_map[MAX_PRIO + 1];

    /**
     * cause of a higher-priority thread.
     */
    uint preempt_high_prio;

    /**
     * cause of timeslice end.
     */
    uint preempt_slice_end;
  } stats /** stats. */;
};


/**
 * @fn void process_init(struct process *process)
 * @brief initialize process.
 *
 * @param process Process.
 */
void process_init(struct process *process);

#endif  /* !_ASSEMBLY */

/*
 * Process Control Block register offsets
 *
 * `offsetof(struct pcb, rax)' is a C expression that can't be
 * referenced at ASM files. We manually calculate those here.
 *
 * Those offsets validity is verified at compile time.
 *
 * FreeBSD neatly solves this problem by producing a C object
 * containing:
 *
 *  char PCB_RAX[offsetof(struct pcb, rax)];
 *
 * for each PCB_* macro below. Afterwards, it let a .sh script
 * extract the symbol size (read: the expression value) using
 * binutils nm. Thus, dynamically producing below ASM-friendly
 * definitions.
 *
 * The trick is worth a mention, but not worth the effort.
 */

/**
 * @def PCB_RBP
 * @brief Offset of PCB RBP.
 */
#define PCB_RBP 0x0

/**
 * @def PCB_RBX
 * @brief Offset of PCB RBX.
 */
#define PCB_RBX 0x8

/**
 * @def PCB_R12
 * @brief Offset of PCB R12.
 */
#define PCB_R12 0x10

/**
 * @def PCB_R13
 * @brief Offset of PCB R13.
 */
#define PCB_R13 0x18

/**
 * @def PCB_R14
 * @brief Offset of PCB R14.
 */
#define PCB_R14 0x20

/**
 * @def PCB_R15
 * @brief Offset of PCB R15.
 */
#define PCB_R15 0x28

/**
 * @def PCB_RSP
 * @brief Offset of PCB RSP.
 */
#define PCB_RSP 0x30

/**
 * @def PCB_SIZE
 * @brief Offset of PCB size.
 */
#define PCB_SIZE (PCB_RSP + 0x8)

/*
 * Process Descriptor offsets.
 */

/**
 * @def PD_PID
 * @brief Offset of PD PID.
 */
#define PD_PID 0x0

/**
 * @def PD_PCB
 * @brief Offset of PD PCB.
 */
#define PD_PCB 0x8

/*
 * IRQ stack protocol offsets.
 */

/**
 * @def IRQCTX_R11
 * @brief Offset of IRQ context R11.
 */
#define IRQCTX_R11 0x0

/**
 * @def IRQCTX_R10
 * @brief Offset of IRQ context R10.
 */
#define IRQCTX_R10 0x8

/**
 * @def IRQCTX_R9
 * @brief Offset of IRQ context R9.
 */
#define IRQCTX_R9 0x10

/**
 * @def IRQCTX_R8
 * @brief Offset of IRQ context R8.
 */
#define IRQCTX_R8 0x18

/**
 * @def IRQCTX_RSI
 * @brief Offset of IRQ context RSI.
 */
#define IRQCTX_RSI 0x20

/**
 * @def IRQCTX_RDI
 * @brief Offset of IRQ context RDI.
 */
#define IRQCTX_RDI 0x28

/**
 * @def IRQCTX_RDX
 * @brief Offset of IRQ context RDX.
 */
#define IRQCTX_RDX 0x30

/**
 * @def IRQCTX_RCX
 * @brief Offset of IRQ context RCX.
 */
#define IRQCTX_RCX 0x38

/**
 * @def IRQCTX_RAX
 * @brief Offset of IRQ context RAX.
 */
#define IRQCTX_RAX 0x40

/**
 * @def IRQCTX_RIP
 * @brief Offset of IRQ context RIP.
 */
#define IRQCTX_RIP 0x48

/**
 * @def IRQCTX_CS
 * @brief Offset of IRQ context CS.
 */
#define IRQCTX_CS 0x50

/**
 * @def IRQCTX_RFLAGS
 * @brief Offset of IRQ context rflags.
 */
#define IRQCTX_RFLAGS 0x58

/**
 * @def IRQCTX_RSP
 * @brief Offset of IRQ context RSP.
 */
#define IRQCTX_RSP 0x60

/**
 * @def IRQCTX_SS
 * @brief Offset of IRQ context SS.
 */
#define IRQCTX_SS 0x68

/**
 * @def IRQCTX_SIZE
 * @brief Offset of IRQ context size.
 */
#define IRQCTX_SIZE (IRQCTX_SS + 0x8)

#ifndef __ASSEMBLY__

/**
 * @fn static inline void pcb_validate_offsets(void)
 * @brief verify offsets manually calculated above.
 */
static inline void pcb_validate_offsets(void)
{
  compiler_assert(PCB_RBP  == offsetof(struct pcb, rbp));
  compiler_assert(PCB_RBX  == offsetof(struct pcb, rbx));
  compiler_assert(PCB_R12  == offsetof(struct pcb, r12));
  compiler_assert(PCB_R13  == offsetof(struct pcb, r13));
  compiler_assert(PCB_R14  == offsetof(struct pcb, r14));
  compiler_assert(PCB_R15  == offsetof(struct pcb, r15));
  compiler_assert(PCB_RSP  == offsetof(struct pcb, rsp));
  compiler_assert(PCB_SIZE == sizeof(struct pcb));

  compiler_assert(PD_PID == offsetof(struct process, pid));
  compiler_assert(PD_PCB == offsetof(struct process, pcb));

  compiler_assert(IRQCTX_R11 == offsetof(struct irq_ctx, r11));
  compiler_assert(IRQCTX_R10 == offsetof(struct irq_ctx, r10));
  compiler_assert(IRQCTX_R9  == offsetof(struct irq_ctx, r9));
  compiler_assert(IRQCTX_R8  == offsetof(struct irq_ctx, r8));
  compiler_assert(IRQCTX_RSI == offsetof(struct irq_ctx, rsi));
  compiler_assert(IRQCTX_RDI == offsetof(struct irq_ctx, rdi));
  compiler_assert(IRQCTX_RDX == offsetof(struct irq_ctx, rdx));
  compiler_assert(IRQCTX_RCX == offsetof(struct irq_ctx, rcx));
  compiler_assert(IRQCTX_RAX == offsetof(struct irq_ctx, rax));
  compiler_assert(IRQCTX_RIP == offsetof(struct irq_ctx, rip));
  compiler_assert(IRQCTX_CS  == offsetof(struct irq_ctx, cs));
  compiler_assert(IRQCTX_RSP == offsetof(struct irq_ctx, rsp));
  compiler_assert(IRQCTX_SS  == offsetof(struct irq_ctx, ss));
  compiler_assert(IRQCTX_RFLAGS == offsetof(struct irq_ctx, rflags));
  compiler_assert(IRQCTX_SIZE == sizeof(struct irq_ctx));
}

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_64_PROCESS_H__ */
