/**
 * @file include/x86/lapic.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_X86_LAPIC_H__
#define	__MCUBE_X86_LAPIC_H__

#define LAPIC_LOCAL_APIC_ID (LAPIC_START + 0x20)
#define LAPIC_LOCAL_APIC_VERSION (LAPIC_START + 0x30)
#define LAPIC_TASK_PRIORITY (LAPIC_START + 0x80)
#define LAPIC_PROCESSOR_PRIORITY (LAPIC_START + 0xa0)
#define LAPIC_EOI (LAPIC_START + 0xb0)
#define LAPIC_LOGICAL_DESTINATION (LAPIC_START + 0xd0)
#define LAPIC_DESTINATION_FORMAT (LAPIC_START + 0xe0)
#define LAPIC_SPURIOUS_INT_VECTOR (LAPIC_START + 0xf0)

#define LAPIC_INSERVICE (LAPIC_START + 0x100) 
/* [LAPIC_START + 0x170, LAPIC_START + 0x17f] reserved */
#define LAPIC_TRIGGER_MODE (LAPIC_START + 0x180)
/* [LAPIC_START + 0x1f0, LAPIC_START + 0x1ff] reserved */
#define LAPIC_INT_REQ (LAPIC_START + 0x200)
/* [LAPIC_START + 0x270, LAPIC_START + 0x27f] reserved */

#define LAPIC_ERROR_STATUS (LAPIC_START + 0x280)
/* LVT CMCI Register (FEE0 02F0H) - Specifies interrupt delivery when an overflow condition of corrected 
machine check error count reaching a threshold value occurred in a machine check bank supporting CMCI (see 
Section 15.5.1, CMCMCI Local APIC Interface */
#define LAPIC_LVT_CMCI (LAPIC_START + 0x2f0)
#define LAPIC_INT_COMMAND_LOW (LAPIC_START + 0x300)
#define LAPIC_INT_COMMAND_HIGH (LAPIC_START + 0x310)
/* LVT Timer Register (FEE0 0320H) - Specifies interrupt delivery when the APIC timer signals an interrupt 
	 (see Section 10.5.4, APIC Timer) */
#define LAPIC_LVT_TIMER (LAPIC_START + 0x320)
/* LVT Thermal Monitor Register (FEE0 0330H) - Specifies interrupt delivery when the thermal sensor 
generates an interrupt (see Section 14.5.2, Thermal Monitor). This LVT entry is implementation specific, not architectural. If implemented, it will always be at base address FEE0 0330H. */
#define LAPIC_LVT_THERMAL_SENSOR (LAPIC_START + 0x330)
/* LVT Performance Counter Register (FEE0 0340H) - Specifies interrupt delivery when a performance 
counter generates an interrupt on overflow (see Section 18.12.5.8, Generating an Interrupt on Overflow).
This LVT entry is implementation specific, not architectural. If implemented, it is not guaranteed to be at base 
address FEE0 0340H. */
#define LAPIC_LVT_PERF_MONITORING_COUNTER (LAPIC_START + 0x340)
/* LVT LINT0 Register (FEE0 0350H) - Specifies interrupt delivery when an interrupt is signaled at the LINT0 
	 pin. */
#define LAPIC_LVT_LINT0 (LAPIC_START + 0x350)
/* LVT LINT1 Register (FEE0 0360H) - Specifies interrupt delivery when an interrupt is signaled at the LINT1 
	 pin. */
#define LAPIC_LVT_LINT1 (LAPIC_START + 0x360)
/* LVT Error Register (FEE0 0370H) - Specifies interrupt delivery when the APIC detects an internal error 
	 (see Section 10.5.3, Error Handling) */
#define LAPIC_LVT_ERROR (LAPIC_START + 0x370)
#define LAPIC_INIT_COUNT (LAPIC_START + 0x380)
#define LAPIC_CUR_COUNT (LAPIC_START + 0x390)
#define LAPIC_DIV_CONFIG (LAPIC_START + 0x3e0)

#define LAPIC_REG_OFFSET 16


/* 18-17: Timer Mode Bits 18:17 selects the timer mode (see Section 10.5.4): 
	 (00b) one-shot mode using a count-down value,
	 (01b) periodic mode reloading a count-down value,
	 (10b) TSC-Deadline mode using absolute target value in IA32_TSC_DEADLINE MSR (see 
	 Section 10.5.4.1),
	 (11b) is reserved. */
#define LAPIC_LVT_TIMER_PERIODIC (0x1 << 17)
/* 16: Mask Interrupt mask: (0) enables reception of the interrupt and (1) inhibits reception of the inter-
	 rupt. When the local APIC handles a performance-monitoring counters interrupt, it automati-
	 cally sets the mask flag in the LVT performance counter register. This flag is set to 1 on reset. 
	 It can be cleared only by software. */
#define LAPIC_LVT_ENABLE_MASK (0x1 << 16)
/* 15: Trigger Mode Selects the trigger mode for the local LINT0 and LINT1 pins: (0) edge sensitive
	 and (1) level sensitive. This flag is only used when the delivery mode is Fixed. When the delivery mode is
 	 NMI, SMI, or INIT, the trigger mode is always edge sensitive. When the delivery mode is 
	 ExtINT, the trigger mode is always level sensitive. The timer and error interrupts are always 
	 treated as edge sensitive. 
	 If the local APIC is not used in conjunction with an I/O APIC and fixed delivery mode is 
	 selected; the Pentium 4, Intel Xeon, and P6 family processors will always use level-sensitive 
	 triggering, regardless if edge-sensitive triggering is selected.
	 Software should always set the trigger mode in the LVT LINT1 register to 0 (edge sensitive). 
	 Level-sensitive interrupts are not supported for LINT1. */
#define LAPIC_LVT_LINT_TRIGGER_MODE_LEVEL (0x1 << 15)
/* 14: Remote IRR Flag (Read Only)
	 For fixed mode, level-triggered interrupts; this flag is set when the local APIC accepts the 
	 interrupt for servicing and is reset when an EOI command is received from the processor. The 
	 meaning of this flag is undefined for edge-triggered interrupts and other delivery modes. */
#define LAPIC_LVT_LINT_REMOTE_IRR (0x1 << 14)
/* 13: Interrupt Input Pin Polarity
	 Specifies the polarity of the corresponding interrupt pin: (0) active high or (1) active low. */
#define LAPIC_LVT_LINT_INTERRUPT_INPUT_PIN_POLARITY (0x1 << 13)
/* 12: Delivery Status (Read Only)
	 Indicates the interrupt delivery status, as follows:
	 0 (Idle) There is currently no activity for this interrupt source, or the previous in-
	 terrupt from this source was delivered to the processor core and accepted.
	 1 (Send Pending)
	 Indicates that an interrupt from this source has been delivered to the processor core
   but has not yet been accepted (see Section 10.5.5, Local Interrupt Acceptance). */
#define LAPIC_LVT_LINT_DELIVERY_STATUS_SEND_PENDING (0x1 << 12)
/* 10-8: Delivery Mode Specifies the type of interrupt to be sent to the processor.
	 Some delivery modes will only 
	 operate as intended when used in conjunction with a specific trigger mode. The allowable 
	 delivery modes are as follows:
	 000 (Fixed) Delivers the interrupt specified in the vector field.
	 010 (SMI) Delivers an SMI interrupt to the processor core through the processor's lo-
	 cal SMI signal path. When using this delivery mode, the vector field should 
	 be set to 00H for future compatibility.
	 100 (NMI) Delivers an NMI interrupt to the processor. The vector information is ig-
	 nored. 
	 101 (INIT) Delivers an INIT request to the processor core, which causes the processor 
	 to perform an INIT. When using this delivery mode, the vector field should 
	 be set to 00H for future compatibility. Not supported for the LVT CMCI reg-
	 ister, the LVT thermal monitor register, or the LVT performance counter 
	 register.
	 110 Reserved; not supported for any LVT register.
	 111 (ExtINT) Causes the processor to respond to the interrupt as if the interrupt origi-
	 nated in an externally connected (8259A-compatible) interrupt controller. 
	 A special INTA bus cycle corresponding to ExtINT, is routed to the external 
	 controller. The external controller is expected to supply the vector informa-
	 tion. The APIC architecture supports only one ExtINT source in a system, 
	 usually contained in the compatibility bridge. Only one processor in the 
	 system should have an LVT entry configured to use the ExtINT delivery 
	 mode. Not supported for the LVT CMCI register, the LVT thermal monitor 
	 register, or the LVT performance counter register. */
#define LAPIC_LVT_DELIVERY_MODE_EXTINT (0x7 << 8)
#define LAPIC_LVT_DELIVERY_MODE_SIPI (0x6 << 8)
#define LAPIC_LVT_DELIVERY_MODE_INIT (0x5 << 8)
#define LAPIC_LVT_DELIVERY_MODE_NMI (0x4 << 8)
/* 0x3 reserved */
#define LAPIC_LVT_DELIVERY_MODE_SMI (0x2 << 8)
#define LAPIC_LVT_DELIVERY_MODE_LOWEST_PRIORITY (0x1 << 8)
#define LAPIC_LVT_DELIVERY_MODE_FIXED (0x0 << 8)
/* 7-0: Vector Interrupt vector number. */



#define LAPIC_SPURIOUS_INT_VECTOR_ENABLE_APIC (0x1 << 8)
#define LAPIC_SPURIOUS_INT_VECTOR_ENABLE_APIC (0x1 << 8)

/* 63-56: Destination Specifies the target processor or processors.
	 This field is only used when the destination 
	 shorthand field is set to 00B. If the destination mode is set to physical, then bits 56 through 59 
	 contain the APIC ID of the target processor for Pentium and P6 family processors and bits 56 
	 through 63 contain the APIC ID of the target processor the for Pentium 4 and Intel Xeon 
	 processors. If the destination mode is set to logical, the interpretation of the 8-bit destination 
	 field depends on the settings of the DFR and LDR registers of the local APICs in all the proces-
	 sors in the system (see Section 10.6.2, Determining IPI Destination). */
#define LAPIC_INT_COMMAND_HIGH_DESTINATION_MASK 0xff000000
/* 55-20: reserved */
/* 19-18: Destination Shorthand
	 Indicates whether a shorthand notation is used to specify the destination of the interrupt and, 
	 if so, which shorthand is used. Destination shorthands are used in place of the 8-bit destina-
	 tion field, and can be sent by software using a single write to the low doubleword of the ICR. 
	 Shorthands are defined for the following cases: software self interrupt, IPIs to all processors 
	 in the system including the sender, IPIs to all processors in the system excluding the sender.
	 00: (No Shorthand)
	 The destination is specified in the destination field.
	 01: (Self) The issuing APIC is the one and only destination of the IPI. This destination 
	 shorthand allows software to interrupt the processor on which it is execut-
	 ing. An APIC implementation is free to deliver the self-interrupt message 
	 internally or to issue the message to the bus and snoop it as with any 
	 other IPI message.
	 10: (All Including Self)
	 The IPI is sent to all processors in the system including the processor send-
	 ing the IPI. The APIC will broadcast an IPI message with the destination 
	 field set to FH for Pentium and P6 family processors and to FFH for Pentium 
	 4 and Intel Xeon processors.
	 11: (All Excluding Self)
	 The IPI is sent to all processors in a system with the exception of the pro-
	 cessor sending the IPI. The APIC broadcasts a message with the physical 
	 destination mode and destination field set to 0xFH for Pentium and P6 
	 family processors and to 0xFFH for Pentium 4 and Intel Xeon processors. 
	 Support for this destination shorthand in conjunction with the lowest-pri-
	 ority delivery mode is model specific. For Pentium 4 and Intel Xeon proces-
	 sors, when this shorthand is used together with lowest priority delivery 
	 mode, the IPI may be redirected back to the issuing processor. */
#define LAPIC_INT_COMMAND_LOW_DESTINATION_ALLBUT (0x3 << 18)
#define LAPIC_INT_COMMAND_LOW_DESTINATION_ALL (0x2 << 18)
#define LAPIC_INT_COMMAND_LOW_DESTINATION_SELF (0x1 << 18)
/* 17-16: reserved */
/* 15: Trigger Mode Selects the trigger mode when using the INIT level de-assert delivery mode: edge (0)
	 or level (1). It is ignored for all other delivery modes. (This flag has no meaning in Pentium 4
	 and Intel Xeon processors, and will always be issued as a 0.) */
#define LAPIC_INT_COMMAND_LOW_TRIGGER_MODE_LEVEL (0x1 << 15)
/* 14: Level For the INIT level de-assert delivery mode this flag must be set to 0; for all other delivery 
modes it must be set to 1. (This flag has no meaning in Pentium 4 and Intel Xeon processors, 
and will always be issued as a 1.) */
#define LAPIC_INT_COMMAND_LOW_LEVEL_ASSERT (0x1 << 14)
/* 13: reserved */
/* 12: Delivery Status (Read Only) Indicates the IPI delivery status, as follows:
	 0 (Idle) Indicates that this local APIC has completed sending any previous IPIs.
	 1 (Send Pending)
	 Indicates that this local APIC has not completed sending the last IPI. */
#define LAPIC_INT_COMMAND_LOW_DELIVERY_STATUS_SEND_PENDING (0x1 << 12)
/* 11: Destination Mode Selects either physical (0) or logical (1) destination mode
	 (see Section 10.6.2, Determining  IPI Destination) */
#define LAPIC_INT_COMMAND_LOW_DESTINATION_MODE_LOGICAL (0x1 << 11)
/* 10-8: Delivery Mode  Specifies the type of IPI to be sent. This field is also know as the IPI message type field.
	 000 (Fixed) Delivers the interrupt specified in the vector field to the target processor 
	 or processors.
	 001 (Lowest Priority)
	 Same as fixed mode, except that the interrupt is delivered to the proces-
	 sor executing at the lowest priority among the set of processors specified 
	 in the destination field. The ability for a processor to send a lowest priority 
	 IPI is model specific and should be avoided by BIOS and operating system 
	 software.
	 010 (SMI) Delivers an SMI interrupt to the target processor or processors. The vector 
	 field must be programmed to 00H for future compatibility.
	 011 (Reserved)
	 100 (NMI) Delivers an NMI interrupt to the target processor or processors. The vector 
	 information is ignored. 
	 101 (INIT) Delivers an INIT request to the target processor or processors, which 
	 causes them to perform an INIT. As a result of this IPI message, all the 
	 target processors perform an INIT. The vector field must be programmed 
	 to 00H for future compatibility.
	 101 (INIT Level De-assert)
	 (Not supported in the Pentium 4 and Intel Xeon processors.) Sends a syn-
	 chronization message to all the local APICs in the system to set their arbi-
	 tration IDs (stored in their Arb ID registers) to the values of their APIC IDs 
	 (see Section 10.7, System and APIC Bus Arbitration). For this delivery 
	 mode, the level flag must be set to 0 and trigger mode flag to 1. This IPI is sent to all processors, regardless of the value in the destination field or the 
	 destination shorthand field; however, software should specify the all including self shorthand. 
	 110 (Start-Up)
	 Sends a special start-up IPI (called a SIPI) to the target processor or 
	 processors. The vector typically points to a start-up routine that is part of 
	 the BIOS boot-strap code (see Section 8.4, Multiple-Processor (MP) Initialization).
	 IPIs sent with this delivery mode are not automatically retried 
	 if the source APIC is unable to deliver it. It is up to the software to deter-
	 mine if the SIPI was not successfully delivered and to reissue the SIPI if 
	 necessary. */
#define LAPIC_INT_COMMAND_LOW_DELIVERY_MODE_EXTINT (0x7 << 8)
#define LAPIC_INT_COMMAND_LOW_DELIVERY_MODE_SIPI (0x6 << 8)
#define LAPIC_INT_COMMAND_LOW_DELIVERY_MODE_INIT (0x5 << 8)
#define LAPIC_INT_COMMAND_LOW_DELIVERY_MODE_NMI (0x4 << 8)
/* 0x3 reserved */
#define LAPIC_INT_COMMAND_LOW_DELIVERY_MODE_SMI (0x2 << 8)
#define LAPIC_INT_COMMAND_LOW_DELIVERY_MODE_LOWEST_PRIORITY (0x1 << 8)
#define LAPIC_INT_COMMAND_LOW_DELIVERY_MODE_FIXED (0x0 << 8)

/* 7-0: Vector The vector number of the interrupt being sent. */

#ifndef __ASSEMBLY__


extern unsigned long cpu_bus_freq_mhz;
    

extern void enable_lapic(uint8_t vector);
extern void disable_lapic(void);
extern void init_lapic_sipi(void);

extern void init_apic(void);
extern void init_lapic_timer(unsigned long tick_us);
extern void start_lapic_timer(unsigned int ch);
extern void stop_lapic_timer(unsigned int ch);
extern void init_lapic_timer_irq(uint8_t vector, uint8_t timer_flag, uint8_t divisor, uint32_t count);


static inline uint64_t find_lapic_first_bit(void)
{
	int i;
	uint64_t bitmap;
	for (i = 0; i < NR_IRQS / 64; i++) {
		bitmap = mmio_in64((unsigned long) LAPIC_INSERVICE + i * LAPIC_REG_OFFSET);
		if (bitmap) {
			return 64 * i + ffb(bitmap);
		}
	}
	return NR_IRQS;
}

#endif /* !__ASSEMBLY__ */

#endif	/* __MCUBE_X86_LAPIC_H__ */
