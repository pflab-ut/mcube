/**
 * @file include/mcube/spinlock.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_SPINLOCK_H__
#define __MCUBE_MCUBE_SPINLOCK_H__


#define SPIN_UNLOCKED 0x1
#define SPIN_LOCKED 0x0

#ifndef __ASSEMBLY__

#define spin_lock_irqsave(lock, flags)
#define spin_unlock_irqrestore(lock, flags)


union x86_rflags {
  uint64_t raw;
  struct {
    uint32_t carry_flag:1,    /* Last math op resulted carry */
      __reserved1_0:1,  /* Always 1 */
      parity_flag:1,    /* Last op resulted even parity */
      __reserved0_0:1,  /* Must be zero */
      auxiliary_flag:1,  /* BCD carry */
      __reserved0_1:1,  /* Must be zero */
      zero_flag:1,    /* Last op resulted zero */
      sign_flag:1,    /* Last op resulted negative */
      trap_flag:1,    /* Enable single-step mode */
      irqs_enabled:1,    /* Maskable interrupts enabled? */
      direction_flag:1,  /* String ops direction */
      overflow_flag:1,  /* Last op MSB overflowed */
      io_privilege:2,    /* IOPL of current task */
      nested_task:1,    /* Controls chaining of tasks */
      __reserved0_2:1,  /* Must be zero */
      resume_flag:1,    /* Debug exceptions related */
      virtual_8086:1,    /* Enable/disable 8086 mode */
      alignment_check:1,  /* Enable alignment checking? */
      virtual:2,    /* Virtualization fields */
      id_flag:1,    /* CPUID supported if writable */
      __reserved0_3:10;  /* Must be zero */
    uint32_t __reserved0_4;    /* Must be zero */
  } __packed;
};


/*
 * Careful! Spinlocks, ironically enough, are globals and thus
 * must be themselves protected against concurrent SMP access!
 */
typedef struct spinlock {
  uint32_t val;

  /*
   * Interrupts get disabled when entering spin-protected
   * critical regions; original state of CPU's 'IF' flag
   * is saved here ... to get restored upon lock release.
   */
  union x86_rflags rflags;
} spinlock_t;


#define INIT_SPINLOCK (spinlock_t) {.val = SPIN_UNLOCKED, .rflags.raw = 0}


void spin_init(spinlock_t *lock);
static inline void spin_lock(spinlock_t *lock);
static inline bool spin_trylock(spinlock_t *lock);
static inline void spin_unlock(spinlock_t *lock);

extern spinlock_t sched_lock;


void init_spinlock(void);

static inline void lock_scheduler(void)
{
}

static inline void unlock_scheduler(void)
{
}


#endif /* __ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_SPINLOCK_H__ */
