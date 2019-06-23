/**
 * @file include/mcube/semaphore.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_SEMAPHORE_H__
#define __MCUBE_MCUBE_SEMAPHORE_H__

#ifndef __ASSEMBLY__


/**
 * @struct sem_struct
 * @brief Semaphore
 */
struct sem_struct {
  /**
   * Semaphore counter.
   */
  volatile short counter;

  /**
   * Ceiling for Priority Ceiling Protocol.
   */
  unsigned long ceil;

  /**
   * Wait queue for semaphore.
   */
  struct thread_struct *ewq;

  /**
   * Owner of semaphore.
   */
  struct thread_struct *owner;

  /**
   * Original priority.
   */
  unsigned long org_prio;
};

/**
 * @typedef sem_struct
 * @brief Typedef of @struct sem_struct.
 */
typedef struct sem_struct sem_struct;

/**
 * @def INIT_COUNT_SEM(nres, cpu)
 * @brief initialize counting semaphore.
 *
 * @param nres Number of resources.
 * @param cpu CPU ID.
 */
#define INIT_COUNT_SEM(nres, cpu) {             \
    .counter  = nres,                           \
      .ceil  = 0,                               \
      .ewq    = &kernel_th[cpu],                \
      .owner    = NULL,                         \
      .org_prio  = 0,                           \
      }


/**
 * @def INIT_SEM_MUTEX
 * @brief initialize semaphore mutex.
 */
#define INIT_SEM_MUTEX INIT_COUNT_SEM(1)

/**
 * @fn static inline bool sem_is_down(struct sem_struct *sem)
 * @brief  reading the counter directly in user land will fail due to the data cache.
 *
 * @param sem Semaphore.
 * @return True if semaphore is down, and false if semaphore is still up.
 */
static inline bool sem_is_down(struct sem_struct *sem)
{
  return sem->counter <= 0;
}

/**
 * @var nr_resources
 * @brief Number of resources.
 */
extern unsigned long nr_resources;

/**
 * @fn void init_sem(struct sem_struct *sem, unsigned long nr_rsrcs, unsigned long ceil)
 * @brief initialize semaphore.
 *
 * @param sem Semaphore,
 * @param nr_rsrcs Number of resources.
 * @param ceil Ceil.
 */
void init_sem(struct sem_struct *sem, unsigned long nr_rsrcs,
              unsigned long ceil);

/**
 * @fn void sem_down(struct sem_struct *sem)
 * @brief down semaphore.
 *
 * @param sem Semaphore.
 */
void sem_down(struct sem_struct *sem);

/**
 * @fn void sem_up(struct sem_struct *sem)
 * @brief up semaphore.
 *
 * @param sem Semaphore.
 */
void sem_up(struct sem_struct *sem);

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_SEMAPHORE_H__ */
