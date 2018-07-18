/**
 * @file include/mcube/semaphore.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_SEMAPHORE_H__
#define __MCUBE_MCUBE_SEMAPHORE_H__

#ifndef	__ASSEMBLY__


/**
 * @brief Semaphore
 *
 * The sem_struct structure has semaphore information.
 */
struct sem_struct {
	/** Semaphore counter. */
	volatile short counter;
	/** Ceiling for Priority Ceiling Protocol. */
	unsigned int ceil;
	/** Wait queue for semaphore. */
	struct thread_struct *ewq;
	/** Owner of semaphore. */
	struct thread_struct *owner;
	/** Original priority. */
	uint64_t org_prio;
};

typedef struct sem_struct sem_struct;

#define	INIT_COUNT_SEM(nres) (struct sem_struct) {	\
		.counter	= nres,																\
			.ceil	= 0,																		\
			.ewq		= &idle_th[0],												\
			.owner		= NULL,															\
			.org_prio	= 0,																\
			}

#define	INIT_SEM_MUTEX	INIT_COUNT_SEM(1)

/* true: if down, false: if still up */
/* reading the counter directly in user land will fail due to the data cache */
static inline int sem_is_down(struct sem_struct *sem)
{
	return (sem->counter <= 0);
}

extern unsigned int nr_resources;

void __sem_down(struct sem_struct *);
void __sem_up(struct sem_struct *);

void init_sem(struct sem_struct *, unsigned int, unsigned int);
void sem_down(struct sem_struct *);
void sem_up(struct sem_struct *);

void init_ipc(void);

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_SEMAPHORE_H__ */
