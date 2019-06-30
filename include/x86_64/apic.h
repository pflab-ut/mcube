/**
 * @file include/x86_64/apic.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_64_APIC_H__
#define __MCUBE_X86_64_APIC_H__

/*
 * Local APIC definitions, 8259A PIC ports, ..
 *
 * Copyright (C) 2009 Ahmed S. Darwish <darwish.07@gmail.com>
 */

#ifndef __ASSEMBLY__


/*
 * APIC Base Address Model Specific Register (MSR).
 */

/**
 * @def MSR_APICBASE
 * @brief MSR for APIC base.
 */
#define MSR_APICBASE 0x0000001b

/* 63-52: reserved. */

/**
 * @def MSR_APICBASE_ADDRMASK
 * @brief MSR for address mask.
 */
#define MSR_APICBASE_ADDRMASK 0x000ffffffffff000ULL

/**
 * @def MSR_APICBASE_ENABLE
 * @brief MSR for enabling APIC base.
 */
#define MSR_APICBASE_ENABLE (1UL << 11)

/* 10-9: reserved. */

/**
 * @def MSR_APICBASE_BSP
 * @brief MSR for boot strap processor.
 */
#define MSR_APICBASE_BSP (1UL << 8)

/* 7-0: reserved. */

/**
 * @fn static inline uint64_t msr_apicbase_getaddr(void)
 * @brief get MSR address.
 *
 * @return MSR address.
 */
static inline uint64_t msr_apicbase_getaddr(void)
{
  uint64_t msr = rdmsr(MSR_APICBASE);
  return (msr & MSR_APICBASE_ADDRMASK);
}

/**
 * @fn static inline void msr_apicbase_setaddr(uint64_t addr)
 * @brief set MSR address.
 *
 * @param addr Address.
 */
static inline void msr_apicbase_setaddr(uint64_t addr)
{
  uint64_t msr = rdmsr(MSR_APICBASE);
  msr &= ~MSR_APICBASE_ADDRMASK;
  addr &= MSR_APICBASE_ADDRMASK;
  msr |= addr;
  wrmsr(MSR_APICBASE, msr);
}

/**
 * @fn static inline void msr_apicbase_enable(void)
 * @brief enable MSR.
 */
static inline void msr_apicbase_enable(void)
{
  uint64_t tmp;

  tmp = rdmsr(MSR_APICBASE);
  tmp |= MSR_APICBASE_ENABLE;
  wrmsr(MSR_APICBASE, tmp);
}

/*
 * APIC mempory-mapped registers. Offsets are relative to the
 * the Apic Base Address and are aligned on 128-bit boundary.
 */

/**
 * @def APIC_ID
 * @brief APIC ID Register.
 */
#define APIC_ID 0x20

/**
 * @union apic_id
 * @brief APIC ID.
 */
union apic_id {
  struct {
    uint32_t
    /**
     * Reserved.
     */
    reserved: 24,
              /**
               * APIC ID.
               */
              id: 8;
  } __packed /** packed. */;
  /**
   * Raw.
   */
  uint32_t raw;
};

/**
 * @def APIC_LVR
 * @brief APIC Version Register.
 */
#define APIC_LVR 0x30

/**
 * @def APIC_TPR
 * @brief APIC Task Priority Register.
 */
#define APIC_TPR 0x80

/**
 * @union apic_tpr
 * @brief APIC task priority register.
 */
union apic_tpr {
  struct {
    /**
     * APIC task priority register.
     */
    uint32_t
    /**
     * Subclass.
     */
    subclass: 4,
              /**
               * Priority.
               */
              priority: 4,
              /**
               * Reserved.
               */
              reserved: 24;
  } __packed /** packed. */;
  /**
   * Value.
   */
  uint32_t value;
};

/**
 * @def APIC_APR
 * @brief APIC Arbitration Priority Register.
 */
#define APIC_APR 0x90

/**
 * @def APIC_PPR
 * @brief APIC Processor Priority Register.
 */
#define APIC_PPR 0xa0

/**
 * @def APIC_EOI
 * @brief APIC End of Interrupt Register.
 */
#define APIC_EOI 0xb0

/**
 * @def APIC_RRR
 * @brief APIC Remote Read Register.
 */
#define APIC_RRR 0xc0

/**
 * @def APIC_LDR
 * @brief APIC Logical Desitination Register.
 */
#define APIC_LDR 0xd0

/**
 * @union apic_ldr
 * @brief APIC logical destination register information.
 */
union apic_ldr {
  struct {
    uint32_t
    /**
     * Reserved.
     */
    reserved: 24,
              /**
               * Logical ID.
               */
              logical_id: 8;
  } __packed /** packed. */;

  /**
   * Value.
   */
  uint32_t value;
};


/**
 * @def APIC_DFR
 * @brief APIC Destination Format Register.
 */
#define APIC_DFR 0xe0

/**
 * @union apic_dfr
 * @brief APIC destination format register information.
 */
union apic_dfr {
  struct {
    uint32_t
    /**
     * Reserved.
     */
    reserved: 28,
              /**
               * APIC model.
               */
              apic_model: 4;
  } __packed /** packed. */;

  /**
   * Value.
   */
  uint32_t value;
};

/**
 * @def APIC_SPIV
 * @brief Spurious interrupt vector register.
 */
#define APIC_SPIV 0xf0

/**
 * @union apic_spiv
 * @brief APIC spurious interrupt vector information.
 */
union apic_spiv {
  struct {
    uint32_t
    /**
     * Vector.
     */
    vector: 8,
            /**
             * APIC enable.
             */
            apic_enable: 1,
            /**
             * Focus.
             */
            focus: 1,
            /**
             * Reserved.
             */
            reserved: 22;
  } __packed /** packed. */ ;

  /**
   * Value.
   */
  uint32_t value;
};


/**
 * @def APIC_ESR
 * @brief APIC Error Status Register.
 */
#define APIC_ESR 0x280

/**
 * @def APIC_ICRL
 * @brief APIC Interrupt Command Register Low [31:0].
 */
#define APIC_ICRL 0x300

/**
 * @def APIC_ICRH
 * @brief APIC Interrupt Command Register High [63:32].
 */
#define APIC_ICRH 0x310

/**
 * @union apic_icr
 * @brief APIC interrupt command register information.
 */
union apic_icr {
  struct {
    uint32_t
    /**
     * Vector.
     */
    vector: 8,
            /**
             * Delivery mode.
             */
            delivery_mode: 3,
            /**
             * Destination mode.
             */
            dst_mode: 1,
            /**
             * Delivery status.
             */
            delivery_status: 1,
            /**
             * Reserved.
             */
            reserved0: 1,
            /**
             * Level.
             */
            level: 1,
            /**
             * Trigger.
             */
            trigger: 1,
            /**
             * Reserved.
             */
            reserved1: 2,
            /**
             * Destination shorthand.
             */
            dst_shorthand: 2,
            /**
             * Reserved.
             */
            reserved2: 12;

    uint32_t
    /**
     * Reserved.
     */
    reserved3: 24,
               /**
                * Destination.
                */
               dst: 8;
  } __packed /** packed. */ ;

  /* Writing the low word of the ICR causes the
   * Inter-Process Interrupt (IPI) to be sent */
  struct {
    /**
     * Value low.
     */
    uint32_t value_low;

    /**
     * Value high.
     */
    uint32_t value_high;
  } __packed /** packed. */;

  /**
   * Value.
   */
  uint64_t value;
};

/*
 * Local Vector Table entries
 */

/**
 * @def APIC_LVTT
 * @brief APIC Timer LVT Entry.
 */
#define APIC_LVTT 0x320

/**
 * @union apic_lvt_timer
 * @brief APIC local vector table timer information.
 */
union apic_lvt_timer {
  struct {
    uint32_t
    /**
     * Vector.
     */
    vector: 8,
            /**
             * Reserved.
             */
            reserved0: 4,
            /**
             * Delivery status (read-only).
             */
            delivery_status: 1,
            /**
             * Reserved.
             */
            reserved1: 3,
            /**
             * Mask.
             */
            mask: 1,
            /**
             * Timer mode.
             */
            timer_mode: 1,
            /**
             * Reserved.
             */
            reserved2: 14;
  } __packed /** packed. */;

  /**
   * Value.
   */
  uint32_t value;
};

/**
 * @def APIC_LVTTHER
 * @brief Thermal LVT entry.
 */
#define APIC_LVTTHER 0x330

/**
 * @union apic_lvt_thermal
 * @brief APIC local vector table thermal information.
 */
union apic_lvt_thermal {
  struct {
    unsigned
    /**
     * Vector.
     */
    vector: 8,
            /**
             * Delivery mode.
             */
            delivery_mode: 3,
            /**
             * Reserved.
             */
            reserved0: 1,
            /**
             * Delivery status.
             */
            delivery_status: 1,
            /**
             * Reserved.
             */
            reserved1: 3,
            /**
             * Mask.
             */
            mask: 1,
            /**
             * Reserved.
             */
            reserved2: 15;
  } __packed /** packed. */;

  /**
   * Value.
   */
  uint32_t value;
};

/**
 * @def APIC_LVTPC
 * @brief APIC Performance Counter LVT Entry.
 */
#define APIC_LVTPC 0x340

/**
 * @union apic_lvt_perfc
 * @brief APIC LVT performance counter information.
 */
union apic_lvt_perfc {
  struct {
    unsigned
    /**
     * Vector.
     */
    vector: 8,
            /**
             * Delivery mode.
             */
            delivery_mode: 3,
            /**
             * Reserved.
             */
            reserved0: 1,
            /**
             * Delivery status.
             */
            delivery_status: 1,
            /**
             * Reserved.
             */
            reserved1: 3,
            /**
             * Mask.
             */
            mask: 1,
            /**
             * Reserved.
             */
            reserved2: 15;
  } __packed /** packed. */;

  /**
   * Value.
   */
  uint32_t value;
};

/**
 * @def APIC_LVT0
 * @brief APIC Local Interrupt 0 LVT Entry.
 */
#define APIC_LVT0 0x350

/**
 * @def APIC_LVT1
 * @brief APIC Local Interrupt 1 LVT Entry.
 */
#define APIC_LVT1 0x360

/**
 * @union apic_lvt_lint
 * @brief APIC LVT local interrupt information.
 */
union apic_lvt_lint {
  struct {
    unsigned
    /**
     * Vector.
     */
    vector: 8,
            /**
             * Delivery mode.
             */
            delivery_mode: 3,
            /**
             * Reserved.
             */
            reserved0: 1,
            /**
             * Delivery status.
             */
            delivery_status: 1,
            /**
             * Reserved.
             */
            reserved1: 1,
            /**
             * Remote IRR.
             */
            remote_irr: 1,
            /**
             * Trigger.
             */
            trigger: 1,
            /**
             * Mask.
             */
            mask: 1,
            /**
             * Reserved.
             */
            reserved2: 15;
  } __packed /** packed. */;

  /**
   * Value.
   */
  uint32_t value;
};

/**
 * @def APIC_LVTERR
 * @brief Error LVT Entry.
 */
#define APIC_LVTERR 0x370

/**
 * @union apic_lvt_error
 * @brief APIC LVT error information.
 */
union apic_lvt_error {
  struct {
    unsigned
    /**
     * Vector etc.
     */
    vector: 8,
            /**
             * Delivery mode.
             */
            delivery_mode: 3,
            /**
             * Reserved.
             */
            reserved0: 1,
            /**
             * Delivery status.
             */
            delivery_status: 1,
            /**
             * Reserved.
             */
            reserved1: 3,
            /**
             * Mask.
             */
            mask: 1,
            /**
             * Reserved
             */
            reserved2: 15;
  } __packed /** packed. */;

  /**
   * Value.
   */
  uint32_t value;
};

/**
 * @def APIC_TIMER_INIT_CNT
 * @brief APIC Timer Initial Count register.
 */
#define APIC_TIMER_INIT_CNT 0x380

/**
 * @def APIC_TIMER_CUR_CNT
 * @brief APIC Timer Current Count register.
 */
#define APIC_TIMER_CUR_CNT 0x390

/**
 * @def APIC_DCR
 * @brief APIC Timer Divide Configuration register.
 */
#define APIC_DCR 0x3e0

/**
 * @union apic_dcr
 * @brief APIC divide configuration register information.
 */
union apic_dcr {
  struct {
    uint32_t
    /**
     * Divisor.
     */
    divisor: 4, /* NOTE! bit-2 MUST be zero */
             /**
              *  Reserved.
              */
             reserved0: 28;
  } __packed /** packed. */;

  /**
   * Value.
   */
  uint32_t value;
};

/* Timer Divide Register divisor; only APIC_DCR_1 was tested. */
enum {
  APIC_DCR_2   = 0x0,    /* Divide by 2   */
  APIC_DCR_4   = 0x1,    /* Divide by 4   */
  APIC_DCR_8   = 0x2,    /* Divide by 8   */
  APIC_DCR_16  = 0x3,    /* Divide by 16  */
  APIC_DCR_32  = 0x8,    /* Divide by 32  */
  APIC_DCR_64  = 0x9,    /* Divide by 64  */
  APIC_DCR_128 = 0xa,    /* Divide by 128 */
  APIC_DCR_1   = 0xb,    /* Divide by 1!  */
};

/*
 * APIC registers field values
 */

/* TPR priority and subclass */
enum {
  APIC_TPR_DISABLE_IRQ_BALANCE = 0,/* Disable hardware IRQ balancing */
};

/* Logical Destination Mode model (DFR) */
enum {
  APIC_MODEL_CLUSTER = 0x0,  /* Hierarchial cluster */
  APIC_MODEL_FLAT    = 0xf,  /* Unique APIC ID for up to 8 cores */
};

/* Delivery mode for IPI and LVT entries */
enum {
  APIC_DELMOD_FIXED = 0x0,  /* deliver to core in vector field */
  APIC_DELMOD_LOWPR = 0x1,  /* to lowest cpu among dst cores */
  APIC_DELMOD_SMI   = 0x2,  /* deliver SMI; vector should be zero */
  APIC_DELMOD_NMI   = 0x4,  /* deliver NMI; vector ignored */
  APIC_DELMOD_INIT  = 0x5,  /* IPI INIT; vector should be zero */
  APIC_DELMOD_START = 0x6,  /* Startup IPI; core starts at 0xVV000 */
  APIC_DELMOD_ExtINT = 0x7, /* Get IRQ vector by PIC's INTA cycle */
};

/* IPI destination mode */
enum {
  APIC_DSTMOD_PHYSICAL = 0x0,
  APIC_DSTMOD_LOGICAL  = 0x1,
};

/* Trigger mode for IPI, LINT0, and LINT1
 * This's only used when delivery mode == `fixed'.
 * NMI, SMI, and INIT are always edge-triggered */
enum {
  APIC_TRIGGER_EDGE  = 0x0,
  APIC_TRIGGER_LEVEL = 0x1,
};

/* Destination shorthands for IPIs
 * When in use, writing the ICR low-word is enough */
enum {
  APIC_DST_SHORTHAND_NONE = 0x0,
  APIC_DST_SHORTHAND_SELF = 0x1,
  APIC_DST_SHORTHAND_ALL_AND_SELF = 0x2,
  APIC_DST_SHORTHAND_ALL_BUT_SELF = 0x3,
};

/* Intereupt level for IPIs */
enum {
  APIC_LEVEL_DEASSERT = 0x0,  /* 82489DX Obsolete. _Never_ use */
  APIC_LEVEL_ASSERT   = 0x1,  /* Always use assert */
};

/* Delivery status for IPI and LVT entries */
enum {
  APIC_DELSTATE_IDLE    = 0,  /* No IPI action, or last IPI acked */
  APIC_DELSTATE_PENDING = 1,  /* Last IPI not yet acked */
};

/* LVT entries mask bit */
enum {
  APIC_UNMASK = 0x0,
  APIC_MASK   = 0x1,
};

/* APIC timer modes */
enum {
  APIC_TIMER_ONESHOT  = 0x0,  /* Trigger timer as one shot */
  APIC_TIMER_PERIODIC = 0x1,  /* Trigger timer monotonically */
};

/* APIC entries hardware-reset values, Intel-defined */
enum {
  APIC_TPR_RESET  = 0x00000000,  /* priority & priority subclass = 0 */
  APIC_LDR_RESET  = 0x00000000,  /* destination logical id = 0 */
  APIC_DFR_RESET  = UINT32_MAX,  /* Flat model, reserved bits all 1s */
  APIC_SPIV_RESET = 0x000000ff,  /* vector=ff, apic disabled, rsrved=0 */
  APIC_LVT_RESET  = 0x00010000,  /* All 0s, while setting the mask bit */
};

/*
 * APIC register accessors
 */


/**
 * @def APIC_PHBASE
 * @brief APIC Physical.
 */
#define APIC_PHBASE 0xfee00000

/**
 * @def APIC_MMIO_SPACE
 * @brief APIC memory-mapped I/O base.
 */
#define APIC_MMIO_SPACE PAGE_SIZE  /* 4-KBytes */

/**
 * @fn void *apic_vrbase(void)
 * @brief APIC virtual base.
 * @return APIC virtual base.
 */
void *apic_vrbase(void);    /* Virtual */


/**
 * @fn static inline void apic_write(uint32_t reg, uint32_t val)
 * @brief write value to APIC register.
 *
 * @param reg Register.
 * @param val Value.
 */
static inline void apic_write(uint32_t reg, uint32_t val)
{
  void *vaddr;

  vaddr = apic_vrbase();
  vaddr = (char *) vaddr + reg;
  mmio_out32(vaddr, val);
}

/**
 * @fn static inline uint32_t apic_read(uint32_t reg)
 * @brief read value from APIC register.
 *
 * @param reg Register.
 * @return Value.
 */
static inline uint32_t apic_read(uint32_t reg)
{
  void *vaddr;

  vaddr = apic_vrbase();
  vaddr = (char *) vaddr + reg;
  return mmio_in32(vaddr);
}

/**
 * @enum irq_dst
 * @brief IRQ destination information.
 */
enum irq_dst {
  IRQ_BROADCAST,      /* Interrupt all cores */
  IRQ_BOOTSTRAP,      /* Interrupt BSP only */
  IRQ_SINGLE,      /* Interrupt a specific core */
};

/**
 * @fn void apic_init(void)
 * @brief initialize APIC.
 */
void apic_init(void);

/**
 * @fn void apic_local_regs_init(void)
 * @brief initialize APIC local registers.
 */
void apic_local_regs_init(void);

/**
 * @fn uint8_t apic_bootstrap_id(void)
 * @brief get APIC bootstrap ID.
 *
 * @return APIC bootstrap ID.
 */
uint8_t apic_bootstrap_id(void);

/**
 * @fn void apic_udelay(uint64_t us)
 * @brief Microsecond delay n APIC.
 *
 * @param us Microsecond.
 */
void apic_udelay(uint64_t us);

/**
 * @fn void apic_monotonic(uint64_t us, uint8_t vector)
 * @brief APIC monotonic.
 *
 * @param us Microsecond.
 * @param vector Vector.
 */
void apic_monotonic(uint64_t us, uint8_t vector);

/**
 * @fn void apic_send_ipi(int dst_apic_id, int delivery_mode, int vector)
 * @brief send Inter-Processor Interrupt (IPI) in APIC.
 *
 * @param dst_apic_id Destination ID.
 * @param delivery_mode Delivery mode.
 * @param vector Vector.
 */
void apic_send_ipi(int dst_apic_id, int delivery_mode, int vector);

/**
 * @fn void apic_broadcast_ipi(int delivery_mode, int vector)
 * @brief broadcast Inter-Processor Interrupt (IPI) in APIC.
 *
 * @param delivery_mode Delivery mode.
 * @param vector Vector.
 */
void apic_broadcast_ipi(int delivery_mode, int vector);

/**
 * @fn bool apic_ipi_acked(void)
 * @brief poll the delivery status bit till the latest IPI is acked
 * by the destination core, or timeout. As advised by Intel,
 * this should be checked after sending each IPI.
 *
 * FIXME: fine-grained timeouts using micro-seconds.
 *
 * @return True in case of delivery success.
 */
bool apic_ipi_acked(void);


/**
 * @fn void __apic_timer_handler(void)
 * @brief handle APIC periodic mode in C.
 */
void __apic_timer_handler(void);

/**
 * @fn void apic_timer_handler(void)
 * @brief handle APIC periodic mode in assembler.
 */
void apic_timer_handler(void);

/**
 * @fn void init_apic_timer(unsigned long tick_us, uint8_t vector)
 * @brief initialize APIC timer.
 *
 * @param tick_us Tick[us].
 * @param vector Vector.
 */
void init_apic_timer(unsigned long tick_us, uint8_t vector);

/**
 * @fn void start_apic_timer(void)
 * @brief start APIC timer.
 */
void start_apic_timer(void);

/**
 * @fn void stop_apic_timer(void)
 * @brief stop APIC timer.
 */
void stop_apic_timer(void);

/**
 * @var apic_ticks_count
 * @brief APIC ticks counter.
 */
extern volatile int apic_ticks_count;

#else

/**
 * @def APIC_PHBASE
 * @brief APIC physical base.
 */
#define APIC_PHBASE 0xfee00000

/**
 * @def APIC_EOI
 * @brief APIC End of Interrupt Register.
 */
#define APIC_EOI 0xb0

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_64_APIC_H__ */

