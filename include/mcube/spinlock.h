/**
 * @file include/mcube/spinlock.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_SPINLOCK_H__
#define __MCUBE_MCUBE_SPINLOCK_H__

/**
 * @def SPIN_UNLOCKED
 * @brief Spin is unlocked.
 */
#define SPIN_UNLOCKED 0x1

/**
 * @def SPIN_LOCKED
 * @brief Spin is locked.
 */
#define SPIN_LOCKED 0x0

#ifndef __ASSEMBLY__

/**
 * @struct spinlock
 * @brief Spinlock information.
 *
 * Careful! Spinlocks, ironically enough, are globals and thus
 * must be themselves protected against concurrent SMP access!
 */
struct spinlock {
  /**
   * Value.
   */
  uint32_t val;

  /*
   * Interrupts get disabled when entering spin-protected
   * critical regions; original state of CPU's 'IF' flag
   * is saved here ... to get restored upon lock release.
   */
  /**
   * Flags.
   */
  union rflags flags;
};

/**
 * @typedef spinlock_t
 * @brief Typedef of @struct spinlock.
 */
typedef struct spinlock spinlock_t;

/**
 * @def INIT_SPINLOCK
 * @brief initialize spinlock.
 */
#define INIT_SPINLOCK (spinlock_t) {.val = SPIN_UNLOCKED, .flags.raw = 0}


void spin_init(spinlock_t *lock);

static inline void spin_lock(spinlock_t *lock);
static inline bool spin_trylock(spinlock_t *lock);
static inline void spin_unlock(spinlock_t *lock);

extern spinlock_t pbuf_lock;
extern spinlock_t sbuf_lock;
extern spinlock_t kmalloc_lock;
extern spinlock_t socket_lock;


void init_spinlock(void);


#endif /* __ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_SPINLOCK_H__ */
