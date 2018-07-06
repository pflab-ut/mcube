/**
 * @file include/axis/regs.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AXIS_REGS_H__
#define __MCUBE_AXIS_REGS_H__

/** Register length. */
#define REG_LENGTH 32
/** Number of general purpose registers. */
#define NR_GENERAL_PURPOSE_REGS 32

#ifndef __ASSEMBLY__


/**
 * @brief Context register
 *
 * The context_regs structure has context register information.
 */
struct gpr_regs {
  /** General purpose registers in AXIS. */
	uint32_t regs[NR_GENERAL_PURPOSE_REGS];
};

typedef struct context_regs context_regs;

/** Number of special registers. */
#define NR_SPECIAL_REGS 9

/**
 * @brief Special register
 *
 * The special_regs structure has special register information.
 */
struct special_regs {
  /** Special registers in AXIS. */
	uint32_t regs[NR_SPECIAL_REGS];
};

typedef struct special_regs special_regs;

/** Number of readable system registers. */
#define NR_SYSTEM_REGS 4

/** Number of readable common system registers. */
#define NR_COMMON_SYSTEM_REGS 1

/**
 * @brief System register
 *
 * The system_regs structure has system register information.
 */
struct system_regs {
  /** Special registers of each core in AXIS. */
	uint32_t regs[NR_INTRA_KERNEL_CPUS][NR_SYSTEM_REGS];
  /** Common special registers. */
  uint32_t common_regs[NR_COMMON_SYSTEM_REGS];
};

typedef struct special_regs special_regs;


/**
 * @brief Full register
 *
 * The full_regs structure has full register information.
 */
struct full_regs {
	/** General purpose registers in AXIS. */
	struct gpr_regs gpr_regs;
  /** Special registers in AXIS. */
  struct special_regs spe_regs;
  /** System registers in AXIS. */
  struct system_regs sys_regs;
};

typedef struct full_regs full_regs;

/** Context top (Stack top - sizeof(struct full_regs)). */
static inline struct full_regs *get_context_top(struct thread_struct *th)
{
  return (struct full_regs *)(th->stack_top - sizeof(struct full_regs));
}

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_REGS_H__ */
