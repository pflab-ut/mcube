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
 * @struct gpr_regs
 * @brief General purpose register
 */
struct gpr_regs {
  /** General purpose registers in AXIS. */
	uint32_t regs[NR_GENERAL_PURPOSE_REGS];
};

typedef struct context_regs context_regs;

/** Number of special registers. */
#define NR_SPECIAL_REGS 9

/**
 * @struct special_regs
 * @brief Special register
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
 * @struct system_regs
 * @brief System register
 */
struct system_regs {
  /** Special registers of each core in AXIS. */
	uint32_t regs[NR_CPUS][NR_SYSTEM_REGS];
  /** Common special registers. */
  uint32_t common_regs[NR_COMMON_SYSTEM_REGS];
};

typedef struct special_regs special_regs;


/**
 * @struct full_regs
 * @brief Full register
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



#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_REGS_H__ */
