/**
 * @file include/mcube/irq.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_IRQ_H__
#define __MCUBE_MCUBE_IRQ_H__

#ifndef __ASSEMBLY__

#define hw_interrupt_type irq_chip

void wait_until_next_interrupt(void);

void init_irq(void);
static void enable_interrupt(void);
static void disable_interrupt(void);

struct irq_desc;
typedef void (*irq_flow_handler_t)(unsigned int irq,
																	 struct irq_desc *desc);
typedef int irqreturn_t;

#define IRQ_NONE    0
#define IRQ_HANDLED 1
#define IRQ_RETVAL(x)   ((x) != 0)


/**
 * IRQ line status.
 *
 * Bits 0-7 are reserved for the IRQF_* bits in linux/interrupt.h
 *
 * IRQ types
 */
#define IRQ_TYPE_NONE 0x0  /* Default, unspecified type */
#define IRQ_TYPE_EDGE_RISING (0x1 << 0) /* Edge rising type */
#define IRQ_TYPE_EDGE_FALLING (0x1 << 1) /* Edge falling type */
#define IRQ_TYPE_EDGE_BOTH (IRQ_TYPE_EDGE_FALLING | IRQ_TYPE_EDGE_RISING)
#define IRQ_TYPE_LEVEL_HIGH (0x1 << 2) /* Level high type */
#define IRQ_TYPE_LEVEL_LOW (0x1 << 3) /* Level low type */
#define IRQ_TYPE_SENSE_MASK 0xf /* Mask of the above */
#define IRQ_TYPE_PROBE (0x1 << 4) /* Probing in progress */

/**
 * Internal flags.
 */
#define IRQ_INPROGRESS (0x1 << 8) /* IRQ handler active - do not enter! */
#define IRQ_DISABLED (0x1 << 9) /* IRQ disabled - do not enter! */
#define IRQ_PENDING (0x1 << 10) /* IRQ pending - replay on enable */
#define IRQ_REPLAY (0x1 << 11) /* IRQ has been replayed but not acked yet */
#define IRQ_AUTODETECT (0x1 << 12) /* IRQ is being autodetected */
#define IRQ_WAITING (0x1 << 13) /* IRQ not yet seen - for autodetection */
#define IRQ_LEVEL (0x1 << 14) /* IRQ level triggered */
#define IRQ_MASKED (0x1 << 15) /* IRQ masked - shouldn't be seen again */
#define IRQ_PER_CPU (0x1 << 16) /* IRQ is per CPU */
#define IRQ_NOPROBE (0x1 << 17) /* IRQ is not valid for probing */
#define IRQ_NOREQUEST (0x1 << 18) /* IRQ cannot be requested */
#define IRQ_NOAUTOEN (0x1 << 19) /* IRQ will not be enabled on request irq */
#define IRQ_WAKEUP (0x1 << 20) /* IRQ triggers system wakeup */
#define IRQ_MOVE_PENDING (0x1 << 21) /* need to re-target IRQ destination */
#define IRQ_NO_BALANCING (0x1 << 22) /* IRQ is excluded from balancing */


/**
 * @brief Hardware interrupt chip descriptor
 *
 * The irq_chip structure has hardware interrpt request.
 */
struct irq_chip {
	/** Name for /proc/interrupts. */
	const char  *name;
	/** Start up the interrupt (defaults to ->enable if NULL). */
	unsigned int    (*startup)(unsigned int irq);
	/** Shut down the interrupt (defaults to ->disable if NULL). */
	void        (*shutdown)(unsigned int irq);
	/** Enable the interrupt (defaults to chip->unmask if NULL). */
	void        (*enable)(unsigned int irq);
	/** Disable the interrupt (defaults to chip->mask if NULL). */
	void        (*disable)(unsigned int irq);

	/** Start of a new interrupt. */
	void        (*ack)(unsigned int irq);
	/** Mask an interrupt source. */
	void        (*mask)(unsigned int irq);
	/** Ack and mask an interrupt source. */
	void        (*mask_ack)(unsigned int irq);
	/** Unmask an interrupt source. */
	void        (*unmask)(unsigned int irq);
	/** End of interrupt - chip level. */
	void        (*eoi)(unsigned int irq);
	/** End of interrupt - flow level. */
	void        (*end)(unsigned int irq);
	/** Set the CPU affinity on SMP machines. */
	void        (*set_affinity)(unsigned int irq, uint64_t dest);
	/** Resend an IRQ to the CPU. */
	int     (*retrigger)(unsigned int irq);
	/** Set the flow type (IRQ_TYPE_LEVEL/etc.) of an IRQ. */
	int     (*set_type)(unsigned int irq, unsigned int flow_type);
	/** Enable/disable power-management wake-on of an IRQ. */
	int     (*set_wake)(unsigned int irq, unsigned int on);

	/* Currently used only by UML, might disappear one day. */
#ifdef CONFIG_IRQ_RELEASE_METHOD
	/** Release function solely used by UML. */
	void        (*release)(unsigned int irq, void *dev_id);
#endif
	/*
	 * For compatibility, ->typename is copied into ->name.
	 * Will disappear.
	 */
	/** Obsoleted by name, kept as migration helper. */
	const char  *typename;
};

typedef struct irq_chip irq_chip;

/**
 * @brief Interrupt descriptor
 *
 * The irq_desc structure has interrupt request descriptor information.
 */
struct irq_descriptor {
	/** Highlevel irq-events handler [if NULL, __do_irq()]. */
	irq_flow_handler_t  handle_irq;
	/** Low level interrupt hardware access. */
	struct irq_chip     *chip;
	/** MSI descriptor. */
	struct msi_desc     *msi_desc;
	/** Per-IRQ data for the irq_chip methods. */
	void            *handler_data;

	/**
	 * Platform-specific per-chip private data for the chip methods,
	 * to allow shared chip implementations.
	 */
	void            *chip_data;
	/** The irq action chain. */
	struct irqaction    *action;
	/** Status information. */
	unsigned int        status;
	/** Disable-depth, for nested irq_disable() calls. */
	unsigned int        depth;
	/** Enable depth, for multiple set_irq_wake() callers. */
	unsigned int        wake_depth;
	/** Stats field to detect stalled irqs. */
	unsigned int        irq_count;
	/** Stats field for spurious unhandled interrupts. */
	unsigned int        irqs_unhandled;
	/** Locking for SMP. */
	spinlock     lock;
	/** Flow handler name for /proc/interrupts output. */
	const char      *name;
}/* ____cacheline_aligned */;

typedef struct irq_descriptor irq_descriptor;


extern struct full_regs * __irq_regs;


static inline struct full_regs *get_irq_regs(void)
{
	return __irq_regs;
}

static inline struct full_regs *set_irq_regs(struct full_regs *new_regs)
{
	struct full_regs *old_regs, **pp_regs = &__irq_regs;

	old_regs = *pp_regs;
	*pp_regs = new_regs;
	return old_regs;
}


typedef irqreturn_t (*irq_handler_t)(int, void *);


/*
 * These correspond to the IORESOURCE_IRQ_* defines in
 * linux/ioport.h to select the interrupt line behaviour.  When
 * requesting an interrupt without specifying a IRQF_TRIGGER, the
 * setting should be assumed to be "as already configured", which
 * may be as per machine or firmware initialisation.
 */
#define IRQF_TRIGGER_NONE   0x00000000
#define IRQF_TRIGGER_RISING 0x00000001
#define IRQF_TRIGGER_FALLING    0x00000002
#define IRQF_TRIGGER_HIGH   0x00000004
#define IRQF_TRIGGER_LOW    0x00000008
#define IRQF_TRIGGER_MASK   (IRQF_TRIGGER_HIGH | IRQF_TRIGGER_LOW | \
														 IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING)
#define IRQF_TRIGGER_PROBE  0x00000010

/*
 * These flags used only by the kernel as part of the
 * irq handling routines.
 *
 * IRQF_DISABLED - keep irqs disabled when calling the action handler
 * IRQF_SAMPLE_RANDOM - irq is used to feed the random generator
 * IRQF_SHARED - allow sharing the irq among several devices
 * IRQF_PROBE_SHARED - set by callers when they expect sharing mismatches to occur
 * IRQF_TIMER - Flag to mark this interrupt as timer interrupt
 * IRQF_PERCPU - Interrupt is per cpu
 * IRQF_NOBALANCING - Flag to exclude this interrupt from irq balancing
 */
#define IRQF_DISABLED       0x00000020
#define IRQF_SAMPLE_RANDOM  0x00000040
#define IRQF_SHARED     0x00000080
#define IRQF_PROBE_SHARED   0x00000100
#define IRQF_TIMER      0x00000200
#define IRQF_PERCPU     0x00000400
#define IRQF_NOBALANCING    0x00000800



/**
 * @brief Interrupt request action
 *
 * The irqaction structure has interrupt request action information.
 */
struct irqaction {
	/** Handler. */
	irq_handler_t handler;
	/** Flags. */
	unsigned long flags;
	/** Mask. */
	unsigned long mask;
	/** Name. */
	const char *name;
	/** Device ID. */
	void *dev_id;
	/** Pointer to next action. */
	struct irqaction *next;
	/** IRQ. */
	int irq;
	/** Pointer to directory entry. */
	struct proc_dir_entry *dir;
};

typedef struct irqaction irqaction;

#define CPU_MASK_NONE 0



/* Function Declarations */


asmlinkage int do_irq(unsigned long irq, struct full_regs *regs);
unsigned int __do_irq(unsigned long irq);
int setup_irq(unsigned int irq, struct irqaction *new);



#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_IRQ_H__ */
