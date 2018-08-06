/**
 * @file include/x86/hpet.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_X86_HPET_H__
#define	__MCUBE_X86_HPET_H__

#define HPET0_START 0xfed00000
//#define HPET1_START 0xfed80000


/* 0x000-0x007: General Capabilities and ID Register Read Only Offset */
#define GENERAL_CAP_ID_LOW (HEPT0_START + 0x0)
#define GENERAL_CAP_ID_HIGH (HPET0_START + 0x4)
#define GENERAL_CAP_ID_64 (HPET0_START + 0x0)

/* 63-32: Main Counter Tick Period.
	 This read-only field indicates the period at which 
	 the counter increments in femptoseconds (10^-15 seconds).  A value of 0 in this 
	 field is not permitted.  The value in this field must be less than or equal to 
	 05F5E100h (10^8 femptoseconds = 100 nanoseconds).  The resolution must be in 
	 femptoseconds (rather than picoseconds) in order to achieve a resolution of 50 
	 ppm.  */
#define GENERAL_CAP_ID_COUNTER_CLK_PERIOD 0xffffffff00000000
/* 31-16: This read-only field will be the same as what would be assigned if this logic was a 
	 PCI function. */
#define GENERAL_CAP_ID_VENDOR_ID 0x00000000ffff0000
/* 15: LegacyReplacement Route Capable:  If this bit is a 1, it indicates that the 
	 hardware supports the LegacyReplacement Interrupt Route option. */
#define GENERAL_CAP_ID_LEG_RT_CAP 0x0000000000008000 
/* 14: Reserved */
/* 13: Counter Size:   
	 This bit is a 0 to indicate that the main counter is 32 bits wide (and cannot operate in 64-bit mode).
	 This bit is a 1 to indicate that the main counter is 64 bits wide
	 (although this does not preclude it from being operated in a 32-bit mode). */
#define GENERAL_CAP_ID_COUNT_SIZE_CAP 0x0000000000002000 
/* 12-8: Number of Timers:  This indicates the number of timers in this block.  The 
	 number in this field indicates the last timer (i.e.  if there are three timers, the value 
	 will be 02h, four timers will be 03h, five timers will be 04h, etc.).  */
#define GENERAL_CAP_ID_NUM_TIM_CAP 0x0000000000001f00 
/* 7-0: This indicates which revision of the function is implemented.
	 The value must NOT be 0x00. */
#define GENERAL_CAP_ID_REV_ID 0x00000000000000ff


/* 0x008-0x00f: Reserved */

/* 0x010-0x017: General Configuration Register Read/Write */
#define GENERAL_CONFIG_LOW (HPET0_START + 0x10)
#define GENERAL_CONFIG_HIGH (HPET0_START + 0x14)
#define GENERAL_CONFIG_64 (HPET0_START + 0x10)

/* 63-2: Reserved */
/* 1: LegacyReplacement Route:   
	 - 0 = Doesn't support Legacy Replacement Route 
	 - 1 = Supports LegacyReplacement Route  
	 If the ENABLE_CNF bit and the LEG_RT_CNF bit are both set, then the interrupts will be 
	 routed as follows:
   Timer 0 will be routed to IRQ0  in Non-APIC or IRQ2  in the I/O APIC 
   Timer 1 will be routed to IRQ8  in Non-APIC or IRQ8  in the I/O APIC 
   Timer 2-n will be routed as per the routing in the timer n config registers. 
	 If the LegacyReplacement Route bit is set, the individual routing bits for timers 0 and 1 
	 (APIC or FSB) will have no impact.
	 If the LegacyReplacement Route bit is not set, the individual routing bits for each of the 
	 timers are used.  */
#define GENRAL_CONFIG_LEGACY_REPLACEMENT_ROUTE 0x0000000000000002 
/* 0: Overall Enable:
	 This bit must be set to enable any of the timers to generate interrupts.  If 
	 this bit is 0, then the main counter will halt (will not increment) and no interrupts will be 
	 caused by any of these timers. 
	 - 0 = Halt main count and disable all timer interrupts 
	 - 1 = allow main counter to run, and allow timer interrupts if enabled  */
#define GENERAL_CONFIG_ENABLE 0x0000000000000001 


/* 0x018-0x01f Reserved */

/* 0x020-0x027: General Interrupt Status Register Read/Write Clear */

#define GENERAL_INTERRUPT_STATUS_LOW (HPET0_START + 0x20)
#define GENERAL_INTERRUPT_STATUS_HIGH (HPET0_START + 0x24)
#define GENERAL_INTERRUPT_STATUS_64 (HPET0_START + 0x20)



/* 63-32 Reserved */
/* 31-3: Timer n Interrupt Active (where xx is 31:3):  Same functionality as for Timer 0 */
#define GENERAL_INTERRUPT_STATUS_T31_INTERRUPT_STS 0x0000000080000000
#define GENERAL_INTERRUPT_STATUS_T30_INTERRUPT_STS 0x0000000040000000
#define GENERAL_INTERRUPT_STATUS_T29_INTERRUPT_STS 0x0000000020000000
#define GENERAL_INTERRUPT_STATUS_T28_INTERRUPT_STS 0x0000000010000000
#define GENERAL_INTERRUPT_STATUS_T27_INTERRUPT_STS 0x0000000008000000
#define GENERAL_INTERRUPT_STATUS_T26_INTERRUPT_STS 0x0000000004000000
#define GENERAL_INTERRUPT_STATUS_T25_INTERRUPT_STS 0x0000000002000000
#define GENERAL_INTERRUPT_STATUS_T24_INTERRUPT_STS 0x0000000001000000
#define GENERAL_INTERRUPT_STATUS_T23_INTERRUPT_STS 0x0000000000800000
#define GENERAL_INTERRUPT_STATUS_T22_INTERRUPT_STS 0x0000000000400000
#define GENERAL_INTERRUPT_STATUS_T21_INTERRUPT_STS 0x0000000000200000
#define GENERAL_INTERRUPT_STATUS_T20_INTERRUPT_STS 0x0000000000100000
#define GENERAL_INTERRUPT_STATUS_T19_INTERRUPT_STS 0x0000000000080000
#define GENERAL_INTERRUPT_STATUS_T18_INTERRUPT_STS 0x0000000000040000
#define GENERAL_INTERRUPT_STATUS_T17_INTERRUPT_STS 0x0000000000020000
#define GENERAL_INTERRUPT_STATUS_T16_INTERRUPT_STS 0x0000000000010000
#define GENERAL_INTERRUPT_STATUS_T15_INTERRUPT_STS 0x0000000000008000
#define GENERAL_INTERRUPT_STATUS_T14_INTERRUPT_STS 0x0000000000004000
#define GENERAL_INTERRUPT_STATUS_T13_INTERRUPT_STS 0x0000000000002000
#define GENERAL_INTERRUPT_STATUS_T12_INTERRUPT_STS 0x0000000000001000
#define GENERAL_INTERRUPT_STATUS_T11_INTERRUPT_STS 0x0000000000000800
#define GENERAL_INTERRUPT_STATUS_T10_INTERRUPT_STS 0x0000000000000400
#define GENERAL_INTERRUPT_STATUS_T9_INTERRUPT_STS 0x0000000000000200
#define GENERAL_INTERRUPT_STATUS_T8_INTERRUPT_STS 0x0000000000000100
#define GENERAL_INTERRUPT_STATUS_T7_INTERRUPT_STS 0x0000000000000080
#define GENERAL_INTERRUPT_STATUS_T6_INTERRUPT_STS 0x0000000000000040
#define GENERAL_INTERRUPT_STATUS_T5_INTERRUPT_STS 0x0000000000000020
#define GENERAL_INTERRUPT_STATUS_T4_INTERRUPT_STS 0x0000000000000010
#define GENERAL_INTERRUPT_STATUS_T3_INTERRUPT_STS 0x0000000000000008
/* 2: Timer 2 Interrupt Active:  Same functionality as Timer 0. */
#define GENERAL_INTERRUPT_STATUS_T2_INTERRUPT_STS 0x0000000000000004
/* 1: Timer 1 Interrupt Active:  Same functionality as Timer 0. */
#define GENERAL_INTERRUPT_STATUS_T1_INTERRUPT_STS 0x0000000000000002
/* Timer 0 Interrupt Active:
	 The functionality of this bit depends on whether the edge or 
	 level-triggered mode is used for this timer: 	 
	 If set to level-triggered mode: 
	 This bit defaults to 0.  This bit will be set by hardware if the corresponding timer 
	 interrupt is active.  Once the bit is set, it can be cleared by software writing a 1 
	 to the same bit position.  Writes of 0 to this bit will have no effect.  For example, 
	 if the bit is already set a write of 0 will not clear the bit. 
	 If set to edge-triggered mode: 
	 This bit should be ignored by software.  Software should always write 0 to this bit.  */
#define GENERAL_INTERRUPT_STATUS_T0_INTERRUPT_STS 0x0000000000000001 


/* 0x028-0x0ef: Reserved */

/* 0x0f0-0x0f7: Main Counter Value Register Read/Write */
#define MAIN_COUNTER (HPET0_START + 0x0f0)
#define MAIN_COUNTER_LOW MAIN_COUNTER
#define MAIN_COUNTER_HIGH (MAIN_COUNTER + 0x4)
#define MAIN_COUNTER_64 MAIN_COUNTER

/* 1.  Software can access the various bytes in this register using 32-bit or 64-bit accesses.  32-bit 
	 accesses can be done to offset 0F0h or 0F4h.  64-bit accesses can be done to 0F0h.  32-bit 
	 accesses must not be done starting at: 0F1h, 0F2h, 0F3h, 0F5h, 0F6h, or 0F7h. 
	 2.  Writes to this register should only be done while the counter is halted. 
	 3.  Reads to this register return the current value of the main counter. 
	 4.  32-bit counters will always return 0 for the upper 32-bits of this register. 
	 5.  If 32-bit software attempts to read a 64-bit counter, it must be aware of timer rollover.  See 
	 section 2.4.7 for details on safely accessing 64-bit counters using software or hardware which 
	 only supports 32-bit accesses.  It is strongly recommended that 32-bit software only operate the 
	 timer in 32-bit mode. 
	 6.  Reads to this register are monotonic.  No two consecutive reads will return the same value, 
	 except when the reads happen at less than the resolution of the counter or if the counter has 
	 rolled over.  (See section 2.4.1) */
/* 63-0 Main counter value */
#define MAIN_COUNTER_VAL 0xffffffffffffffff

/* 0x0f8-0x0ff: Reserved */


/* 0x100-0x107h Timer N Configuration and Capability Register Read/Write */
#define TIMER_CONFIG_CAP(n) (HPET0_START + 0x100 + (n) * 0x20)
#define TIMER_CONFIG_CAP_LOW(n) TIMER_CONFIG_CAP(n)
#define TIMER_CONFIG_CAP_HIGH(n) (TIMER_CONFIG_CAP(n) + 0x4)
#define TIMER_CONFIG_CAP_64(n) TIMER_CONFIG_CAP(n)

/* 63-32: Timer n Interrupt Routing Capability: (where n is the timer number:  00 to 
	 31)  This 32-bit read-only field indicates to which interrupts in the I/O (x) APIC 
	 this timer's interrupt can be routed.  This is used in conjunction with the 
	 Tn_INT_ROUTE_CNF field. 
	 Each bit in this field corresponds to a particular interrupt.  For example, if this 
	 timer's interrupt can be mapped to interrupts 16, 18, 20, 22, or 24, then bits 16, 
	 18, 20, 22, and 24 in this field will be set to 1.  All other bits will be 0. */
#define TIMER_CONFIG_CAP_INTERRUPT_ROUTING_CAPABILITY 0xffffffff00000000
/* 31-16: Reserved */
/* 15: Timer n FSB Interrupt Delivery:  (where n is the timer number:  00 to 31).  If 
	 this read-only bit is 1, then the hardware supports a direct front-side bus delivery 
	 of this timer's interrupt.  */
#define TIMER_CONFIG_CAP_FSB_INTERRUPT_DELIVERY 0x0000000000008000 
/* 14: Timer n FSB Interrupt Enable: (where n is the timer number: 00 to 31).  If the 
	 Tn_FSB_INT_DEL_CAP bit is set for this timer, then the software can set the 
	 Tn_FSB_EN_CNF bit to force the interrupts to be delivered directly as FSB 
	 messages, rather than using the I/O (x) APIC.  In this case, the 
	 Tn_INT_ROUTE_CNF field in this register will be ignored.  The 
	 Tn_FSB_ROUTE register will be used instead. */
#define TIMER_CONFIG_CAP_FSB_INTERRUPT_ENABLE 0x0000000000004000
/* 13-9: Timer n Interrupt Route:  (where n is the timer number:  00 to 31).  This 5-bit 
	 read/write field indicates the routing for the interrupt to the I/O APIC.  A 
	 maximum value of 32 interrupts are supported.  Default is 00h  Software writes 
	 to this field to select which interrupt in the I/O (x) will be used for this timer's 
	 interrupt.  If the value is not supported by this prarticular timer, then the value 
	 read back will not match what is written. The software must only write valid 
	 values. 
	 Note:  If the LegacyReplacement Route bit is set, then Timers 0 and 1 will have 
	 a different routing, and this bit field has no effect for those two timers.
	 Note:  If the Tn_FSB_INT_DEL_CAP bit is set, then the interrupt will be 
	 delivered directly to the FSB, and this bit field has no effect. */
#define TIMER_CONFIG_CAP_INTERRUPT_ROUTE 0x0000000000000200
/* 8: Timer n 32-bit Mode: (where n is the timer number: 00 to 31).  Software can 
	 set this read/write bit to force a 64-bit timer to behave as a 32-bit timer.  This is 
	 typically needed if the software is not willing to halt the main counter to read or 
	 write a particular timer, and the software is not capable of doing an atomic 64-
	 bit read to the timer.  If the timer is not 64 bits wide, then this bit will always be 
	 read as 0 and writes will have no effect. */
#define TIMER_CONFIG_CAP_32MODE 0x0000000000000100
/* 7: Reserved */
/* 6: Timer n Value Set: (where n is the timer number: 00 to 31).  Software uses this 
	 read/write bit only for timers that have been set to periodic mode. By writing 
	 this bit to a 1, the software is then allowed to directly set a periodic timer's 
	 accumulator.   
	 Software does not have to write this bit back to 0 (it automatically clears).
	 Software should not write a 1 to this bit position if the timer is set to non-
	 periodic mode.  */
#define TIMER_CONFIG_CAP_VAL_SET 0x0000000000000040
/* 5: Timer n Size:  (where n is the timer number:  00 to 31).  This read-only field 
	 indicates the size of the timer.   1 = 64-bits,   0 = 32-bits.  */
#define TIMER_CONFIG_CAP_SIZE 0x0000000000000020
/* 4: Timer n Periodic Interrupt Capable:  (where n is the timer number:  00 to 
	 31).  If this read-only bit is 1, then the hardware supports a periodic mode for 
	 this timer's interrupt.  */
#define TIMER_CONFIG_CAP_PERIODIC_INTERRUPT_CAPABLE 0x0000000000000010
/* 3: Timer n Type:  (where n is the timer number:  00 to 31). 
	 If the corresponding Tn_PER_INT_CAP bit is 0, then this bit will always 
	 return 0 when read and writes will have no impact.  
	 If the corresponding Tn_PER_INT_CAP bit is 1, then this bit is read/write, and 
	 can be used to enable the timer to generate a periodic interrupt.  
	 -  Writing a 1 to this bit enables the timer to generate a periodic 
	 interrupt. 
	 -  Writing a 0 to this bit enables the timer to generate a non-periodic 
	 interrupt.  */
#define TIMER_CONFIG_CAP_TYPE 0x0000000000000008
/* 2: Timer n Interrupt Enable:  (where n is the timer number:  00 to 31).  This 
	 read/write bit must be set to enable timer n to cause an interrupt when the timer 
	 event fires. 
	 Note: If this bit is 0, the timer will still operate and generate appropriate status 
	 bits, but will not cause an interrupt. */
#define TIMER_CONFIG_CAP_INTERRUPT_ENABLE 0x0000000000000004
/* 1: Timer n Interrupt Type:  (where n is the timer number:  00 to 31) 
	 - 0 = The timer interrupt is edge triggered. This means that an edge-
	 type interrupt is generated. If another interrupt occurs, another edge 
	 will be generated.  
	 - 1 = The timer interrupt is level triggered. This means that a level-
	 triggered interrupt is generated. The interrupt will be held active until 
	 it is cleared by writing to the bit in the General Interrupt Status
	 Register. If another interrupt occurs before the interrupt is cleared, the 
	 interrupt will remain active. */
#define TIMER_CONFIG_CAP_INTERRUPT_TYPE 0x0000000000000002
/* 0: Reserved */


/* 0x108-0x10f: Timer N Comparator Value Register Read/Write */
/* 1.  Software  can  access  the  various  bytes  in  this  register  using  32-bit  or  64-bit  accesses.
	 32-bit  accesses  can  be  done  to  offset  1x8h  or  1xCh.    64-bit  accesses  can  be  done  to  1x8h.
	 32-bit  accesses must not be done to offsets 1x9h, 1xAh, 1xBh, 1xDh, 1xEh, or 1xFh. 
	 2.  Reads to this register return the current value of the comparator. 
	 3.  If the timer is configured to non-periodic mode: 
	 -   Writes to this register load the value against which the main counter should be compared 
	 for this timer. 
	 -  When the main counter equals the value last written to this register, the corresponding 
	 interrupt can be generated (if so enabled). 
	 -  The value in this register does not change based on the interrupt being generated. 
	 4.  If the timer is configured to periodic mode: 
	 -   When the main counter equals the value last written to this register, the corresponding 
	 interrupt can be generated (if so enabled). 
	 -  After the main counter equals the value in this register, the value in this register is increased 
	 by the value last written to the register. 
	 
	 For example, if the value written to the register is 00000123h, then 
	 
	 1.  An interrupt will be generated when the main counter reaches 00000123h. 
	 2.  The value in this register will then be adjusted by the hardware to 00000246h. 
	 3.  Another interrupt will be generated when the main counter reaches 00000246h 
	 4.  The value in this register will then be adjusted by the hardware to 00000369h 
	 
	 -  As each periodic interrupt occurs, the value in this register will increment.  When the 
	 incremented value is greater than the maximum value possible for this register 
	 (FFFFFFFFh for a 32-bit timer or FFFFFFFFFFFFFFFFh for a 64-bit timer), the value will 
	 wrap around through 0.  For example, if the current value in a 32-bit timer is FFFF0000h 
	 and the last value written to this register is 20000, then after the next interrupt the value 
	 will change to 00010000h 
	 5.  Default value for each timer is all 1s for the bits that are implemented.  For example, a 32-bit 
	 timer  will  have a  default  value of 00000000FFFFFFFFh. A 64-bit timer will have a default 
	 value of FFFFFFFFFFFFFFFFh. */
#define TIMER_COMPARATOR(n) (HPET0_START + 0x108 + (n) * 0x20)
#define TIMER_COMPARATOR_LOW(n) TIMER_COMPARATOR(n)
#define TIMER_COMPARATOR_HIGH(n) (TIMER_COMPARATOR(n) + 0x4)
#define TIMER_COMPARATOR_64(n) TIMER_COMPARATOR(n)

/* 0x110-0x117: Timer N FSB Interrupt Route Register Read/Write */
#define TIMER_FSB_INTERRUPT_ROUTE(n) (HPET0_START + 0x110 + (n) * 0x20)
#define TIMER_FSB_INTERRUPT_ROUTE_LOW(n) TIMER_FSB_INTERRUPT_ROUTE(n)
#define TIMER_FSB_INTERRUPT_ROUTE_HIGH(n) (TIMER_FSB_INTERRUPT_ROUTE(n) + 0x4)
#define TIMER_FSB_INTERRUPT_ROUTE_64(n) TIMER_FSB_INTERRUPT_ROUTE(n)

/* 63-32: Software  sets  this  32-bit  field  to  indicate  the  location  that  the  FSB  interrupt 
	 message should be written to.  */
#define TIMER_FSB_INTERRUPT_ADDR
/* 31-0: Software sets this 32-bit field to indicate that value that is written during the FSB 
	 interrupt message */
#define TIMER_FSB_INTERRUPT_VAL


/* 0x118-0x11f: Reserved */

/* Timer 0 will be routed to IRQ0 in Non-APIC or IRQ2 in the I/O APIC 
   Timer 1 will be routed to IRQ8 in Non-APIC or IRQ8 in the I/O APIC 
   Timer 2-n will be routed as per the routing in the timer n config registers. */
/* for 8259 Mapping */
#define HPET_TIMER0_IRQ 0x2
#define HPET_TIMER1_IRQ 0x8
#define HPET_REDIRECTION_OFFSET 0x10
//#define HPET_REDIRECTION_OFFSET 0x30
//#define HPET_REDIRECTION_OFFSET 0x80


//#define NR_HPETS 0x10
#define NR_HPETS 0x20

/* MHz of HPET */
//#define HPET_MHZ 14.31818
#define HPET_MHZ 100
#define HPET_HZ (HPET_MHZ * 1000 * 1000)

#define HPET_TICK_MS(x) (HPET_MHZ * 1000 * (x))
#define HPET_TICK_US(x) (HPET_MHZ * (x))


#ifndef __ASSEMBLY__



/* 1.  The timers are expected to be accurate over any 1 ms period to within 0.05% (500 ppm)of the time 
	 specified in the timer resolution fields.  
	 2.  Within any 100-microsecond period, the timer is permitted to report a time that is up to 2 ticks too 
	 early or too late.
	 Each tick must be less than or equal to 100 ns, so this represents an error of less than 0.2% (2000 ppm).
	 3.  The main counter must be an up-counter. Two consecutive reads to the main counter may return the 
	 same value if the access latency to the timer is less than the clock period that feeds it. For back-back 
	 reads, the 2nd read must never return a value that is less than the 1st read,
	 unless the counter has rolled over and actually reached the same value. */
static inline uint64_t read_hpet_counter(void)
{
#if 1
  uint64_t count;
  count = mmio_in64(MAIN_COUNTER_64);
  return count;
#else
	uint32_t low, high[2];
	do {
		high[0] = mmio_in32(MAIN_COUNTER_HIGH);
		low = mmio_in32(MAIN_COUNTER_LOW);
		high[1] = mmio_in32(MAIN_COUNTER_HIGH);
	} while (high[0] != high[1]); /* roll over? */
	return (uint64_t) low | ((uint64_t) high[1] << 32);
#endif
}

void init_hpet_timer(unsigned long tick_us);
void start_hpet_timer(unsigned int ch);
void stop_hpet_timer(unsigned int ch);

#endif /* !__ASSEMBLY__ */

#endif	/* __MCUBE_X86_HPET_H__ */
