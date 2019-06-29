/**
 * @file include/aarch64/task.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AARCH64_TASK_H__
#define __MCUBE_AARCH64_TASK_H__

#ifndef __ASSEMBLY__

/**
 * @def PSR_MODE_EL0t
 * @brief PSR mode EL0t.
 */
#define PSR_MODE_EL0t 0x00000000

/**
 * @def PSR_MODE_EL1t
 * @brief PSR mode EL1t.
 */
#define PSR_MODE_EL1t 0x00000004

/**
 * @def PSR_MODE_EL1h
 * @brief PSR mode EL1h.
 */
#define PSR_MODE_EL1h 0x00000005

/**
 * @def PSR_MODE_EL2t
 * @brief PSR mode EL2t.
 */
#define PSR_MODE_EL2t 0x00000008

/**
 * @def PSR_MODE_EL2h
 * @brief PSR mode EL2h.
 */
#define PSR_MODE_EL2h 0x00000009

/**
 * @def PSR_MODE_EL3t
 * @brief PSR mode EL3t.
 */
#define PSR_MODE_EL3t 0x0000000c

/**
 * @def PSR_MODE_EL3h
 * @brief PSR mode EL3h.
 */
#define PSR_MODE_EL3h 0x0000000d


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_AARCH64_TASK_H__ */
