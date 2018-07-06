/**
 * @file include/x86/regs.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_REGS_H__
#define __MCUBE_X86_REGS_H__

#ifndef __ASSEMBLY__


#define	SEPARATE_SIZE	0

#define	STACK_ALIGN(x)	(((x) + 7) & -8)

/**
 * @brief Context register
 *
 * The context_regs structure has context register information.
 */
struct context_regs {
	/** Backlink in x86. */
	uint64_t backlink;
	/** ESP0 in x86. */
	uint64_t rsp0;
	/** SS0 in x86. */
	uint64_t rss0;
	/** ESP1 in x86. */
	uint64_t rsp1;
	/** SS1 in x86. */
	uint64_t rss1;
	/** ESP2 in x86. */
	uint64_t rsp2;
	/** SS2 in x86. */
	uint64_t rss2;
	/** CR3 in x86. */
	uint64_t cr3;
	/** EIP in x86. */
	uint64_t rip;
	/** EFLAGS in x86. */
	uint64_t rflags;
	/** EAX in x86. */
	uint64_t rax;
	/** ECX in x86. */
	uint64_t rcx;
	/** EDX in x86. */
	uint64_t rdx;
	/** EBX in x86. */
	uint64_t rbx;
	/** ESP in x86. */
	uint64_t rsp;
	/** EBP in x86. */
	uint64_t rbp;
	/** ESI in x86. */
	uint64_t rsi;
	/** EDI in x86. */
	uint64_t rdi;
	/** ES in x86. */
	uint64_t res;
	/** CS in x86. */
	uint64_t rcs;
	/** SS in x86. */
	uint64_t rss;
	/** DS in x86. */
	uint64_t rds;
	/** FS in x86. */
	uint64_t rfs;
	/** GS in x86. */
	uint64_t rgs;
	/** LDTR in x86. */
	uint64_t ldtr;
	/** IO map in x86. */
	uint64_t iomap;
};

typedef struct context_regs context_regs;

/**
 * @brief Full register
 *
 * The full_regs structure has full register information.
 */
struct full_regs {
	/** Context register in x86. */
	struct context_regs cregs;
};

typedef struct full_regs full_regs;

/** Context top (Stack top - sizeof(struct full_regs)). */
static inline struct full_regs *get_context_top(struct thread_struct *th)
{
  return (struct full_regs *)(th->stack_top - sizeof(struct full_regs));
}


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_REGS_H__ */
