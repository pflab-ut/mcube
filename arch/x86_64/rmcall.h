/**
 * @file arch/x86_64/rmcall.h
 *
 * @author Hiroyuki Chishiro
 */

/*
 * Macros for the real-mode functions which are called using the
 * "rmcall" service from a 32-bit protected mode context.
 *
 * Check `rmcall.S' and `load_ramdisk.S' for further details.
 */

/**
 * @def REL_ADDR(symbol, rmode_func_start)
 * @brief return @a symbol address relative to the setup data segment.
 *
 * __rmode_function_offset: The "real-mode function" start
 *  address offset out of the @a cs base value PMODE16_START.
 *
 * @param symbol Label inside the passed "real-mode function".
 * @param rmode_func_start "Real-mode function" start address.
 */
#define REL_ADDR(symbol, rmode_func_start)    \
  (symbol - rmode_func_start + __rmode_function_offset)

/**
 * @def ABS_ADDR(addr)
 * @brief return the absolute address @a addr value relative to the setup data segment base.
 *
 * @param addr: absolute address within the segment 64-KB region.
 */
#define ABS_ADDR(addr) (addr - PMODE16_START)

