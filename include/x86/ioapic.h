/**
 * @file include/x86/ioapic.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_X86_IOAPIC_H__
#define	__MCUBE_X86_IOAPIC_H__

/* memory address 0xfec0xy00 */
/* NOTES: xy are determined by the x and y fields in the APIC Base Address Relocation Register
	 located in the PIIX3. Range for x = 0-Fh and the range for y = 0,4,8,Ch. */


/* I/O Register Select Register */
/* This register selects the IOAPIC Register to be read/written. The data is then read from or written
	 to the selected register through the IOWIN Register. */
#define IO_REG_SELECT IOAPIC_START
/* 31-8: reserved */
/* 7-0: APIC Register Address - R/W.
	 Bits [7:0] specify the IOAPIC register to be read/written via the IOWIN Register. */
#define IO_REG_SELECT_APIC_REG_ADDR 0xff

/* I/O Window Register */
/* This  register  is  used  to  write  to  and  read  from  the  register  selected
	 by  the  IOREGSEL  Register.
	 Readability/writability is determined  by the IOAPIC register that is currently selected. */
#define IO_WIN (IOAPIC_START + 0x10)
/* 31-0: APIC Register Data?R/W.  Memory references to this register are mapped to the APIC register
	 specified by the contents of the IOREGSEL Register. */
#define IO_WIN_APIC_REG_DATA_MASK 0xffffffff

/* IOAPIC Identification Register */
/* This register contains the 4-bit APIC ID. The ID serves as a physical name of the IOAPIC. All APIC devices
using the APIC bus should have a unique APIC ID. The APIC bus arbitration ID for the I/O unit is also writtten during a write to the APICID Register (same data is loaded into both). This register must be programmed with the correct ID value before using the IOAPIC for message transmission. */
#define IOAPIC_ID_OFFSET 0x00
/* 31-28: reserved */
/* 27-24: IOAPIC Identification?R/W.  This 4 bit field contains the IOAPIC identification. */
#define IOAPIC_ID_MASK 0x0f000000
/* 23-0: reserved */

/* IOAPIC Version Register */
/* The  IOAPIC  Version  Register  identifies  the  APIC  hardware  version.  Software  can  use  this  to  provide compatibility between different APIC implementations and their versions. In addition, this register provides the maximum number of entries in the I/O Redirection Table. */
#define IOAPIC_VER_OFFSET 0x01
/* 31-24: reserved */
/* 23-16: Maximum Redirection Entry - RO.  This field contains the entry number (0 being the lowest
entry) of the highest entry in the I/O Redirection Table. The value is equal to the number of
interrupt input pins for the IOAPIC minus one. The range of values is 0 through 239. For this
IOAPIC, the value is 17h. */
#define IOAPIC_VER_MAX_REDIRECTION_ENTRY_MASK 0x00ff0000
/* 15-8: reserved */
/* 7-0: APIC VERSION - RO.  This 8 bit field identifies the implementation version. The version number
	 assigned to the IOAPIC is 11h. */
#define IOAPIC_VER_APIC_VER_MASK 0x000000ff

/* IOAPIC Arbitration Register */
/* The  APICARB  Register  contains  the  bus  arbitration  priority  for  the  IOAPIC.
	 This  register  is  loaded  when  the IOAPIC ID Register is written.
The APIC uses a one wire arbitration to win bus ownership. A rotating priority scheme is used for arbitration.
The winner of the arbitration becomes the lowest priority agent and assumes an arbitration ID of 0.
All other agents, except the agent whose arbitration ID is 15, increment their arbitration IDs by one.
The agent whose  ID  was  15  takes  the  winner's  arbitration  ID  and  increments  it  by  one.
Arbitration  IDs  are  changed (incremented  or  asssumed)  only  for  messages  that  are  transmitted  successfully  (except,  in  the  case  of  low priority  messages  where  Arbitration  ID  is  changed  even  if  message  was  not  successfully  transmitted).
A message is transmitted successfully if no checksum error or acceptance error is reported for that message.
The APICARB Register is always loaded with IOAPIC ID during a "level triggered INIT with de-assert" message. */
#define IOAPIC_ARBITRATION_OFFSET 0x02
/* 31-28: reserved */
/* 27-24: IOAPIC Identification - R/W.  This 4 bit field contains the IOAPIC Arbitration ID. */
#define IOAPIC_ARBITRATION_ID_MASK 0x0f000000
/* 23-0: reserved */


/* IOREDTBL[23:0] - I/O REDIRECTION TABLE REGS */
#define IO_REDIRECTION_TABLE_REG_OFFSET(n) (0x10 + (n) * 0x2)
/* 63-56: Destination Field - R/W.
	 If the Destination Mode of this entry is Physical Mode (bit 11=0), bits
	 [59:56] contain an APIC ID. If Logical Mode is selected (bit 11=1), the Destination Field
	 potentially defines a set of processors. Bits [63:56] of the Destination Field specify the logical
	 destination address.
	 - Destination Mode IOREDTBLx[11]
	 0 = Physical Mode, 1 = Logical Mode 	 
	 - Logical Destination Address
	 IOREDTBLx[59:56] = APIC ID, IOREDTBLx[63:56] = Set of processors */
#define IO_REDIRECTION_TABLE_REG_DESTINATION_FIELD(x) ((x) << 56)
/* 55-17: reserved */
/* 16: Interrupt Mask - R/W.  When this bit is 1, the interrupt signal is masked. Edge-sensitive
	 interrupts signaled on a masked interrupt pin are ignored (i.e., not delivered or held pending).
	 Level-asserts or negates occurring on a masked level-sensitive pin are also ignored and have no
	 side effects. Changing the mask bit from unmasked to masked after the interrupt is accepted by
	 a local APIC has no effect on that interrupt. This behavior is identical to the case where the
	 device withdraws the interrupt before that interrupt is posted to the processor. It is software's
	 responsibility to handle the case where the mask bit is set after the interrupt message has been
	 accepted by a local APIC unit but before the interrupt is dispensed to the processor. When this
	 bit is 0, the interrupt is not masked. An edge or level on an interrupt pin that is not masked
	 results in the delivery of the interrupt to the destination. */
#define IO_REDIRECTION_TABLE_REG_INTERRUPT_MASK (0x1 << 16)
/* 15: Trigger Mode - R/W.  The trigger mode field indicates the type of signal on the interrupt pin that
	 triggers an interrupt. 1=Level sensitive, 0=Edge sensitive. */
#define IO_REDIRECTION_TABLE_REG_TRIGGER_MODE (0x1 << 15)
/* 14: Remote IRR - RO.  This bit is used for level triggered interrupts. Its meaning is undefined for
	 edge triggered interrupts. For level triggered interrupts, this bit is set to 1 when local APIC(s)
	 accept the level interrupt sent by the IOAPIC. The Remote IRR bit is set to 0 when an EOI
	 message with a matching interrupt vector is received from a local APIC. */
#define IO_REDIRECTION_TABLE_REG_REMOTE_IRR (0x1 << 14)
/* 13: Interrupt Input Pin Polarity (INTPOL) - R/W.
	 This bit specifies the polarity of the interrupt signal. 0=High active, 1=Low active. */
#define IO_REDIRECTION_TABLE_REG_INTERRUPT_INPUT_PIN_POLARITY (0x1 << 13)
/* 12: Delivery Status (DELIVS) - RO.  The Delivery Status bit contains the current status of the
	 delivery of this interrupt. Delivery Status is read-only and writes to this bit (as part of a 32 bit
	 word) do not effect this bit. 0=IDLE (there is currently no activity for this interrupt). 1=Send
	 Pending (the interrupt has been injected but its delivery is temporarily held up due to the APIC
	 bus being busy or the inability of the receiving APIC unit to accept that interrupt at that time). */
#define IO_REDIRECTION_TABLE_REG_DELIVERY_STATUS (0x1 << 12)
/* 11: Destination Mode (DESTMOD) - R/W.  This field determines the interpretation of the
	 Destination field. When DESTMOD=0 (physical mode), a destination APIC is identified by its ID.
	 Bits 56 through 59 of the Destination field specify the 4 bit APIC ID. When DESTMOD=1 (logical
	 mode), destinations are identified by matching on the logical destination under the control of the
	 Destination Format Register and Logical Destination Register in each Local APIC.
	 - Destination Mode IOREDTBLx[11]
	 0 = Physical Mode, 1 = Logical Mode 	 
	 - Logical Destination Address
	 IOREDTBLx[59:56] = APIC ID, IOREDTBLx[63:56] = Set of processors */
#define IO_REDIRECTION_TABLE_REG_DESTINATION_MODE (0x1 << 11)
/* 10-8: Delivery Mode (DELMOD) - R/W.  The Delivery Mode is a 3 bit field that specifies how the
	 APICs listed in the destination field should act upon reception of this signal. Note that certain
	 Delivery Modes only operate as intended when used in conjunction with a specific trigger Mode.
	 These restrictions are indicated in the following table for each Delivery Mode.
	 Bits
	 [10:8] Mode Description
	 000 Fixed Deliver the signal on the INTR signal of all processor cores listed in the
	 destination. Trigger Mode for "fixed" Delivery Mode can be edge or level.
	 001 Lowest
	 Priority Deliver the signal on the INTR signal of the processor core that is
	 executing at the lowest priority among all the processors listed in the
	 specified destination. Trigger Mode for "lowest priority". Delivery Mode
	 can be edge or level.
	 010 SMI System Management Interrupt. A delivery mode equal to SMI requires an
	 edge trigger mode. The vector information is ignored but must be
	 programmed to all zeroes for future compatibility.
	 011 Reserved
	 100 NMI Deliver the signal on the NMI signal of all processor cores listed in the
	 destination. Vector information is ignored. NMI is treated as an edge
	 triggered interrupt, even if it is programmed as a level triggered interrupt.
	 For proper operation, this redirection table entry must be programmed to
	 edge triggered interrupt.
	 101 INIT Deliver the signal to all processor cores listed in the destination by
	 asserting the INIT signal. All addressed local APICs will assume their
	 INIT state. INIT is always treated as an edge triggered interrupt, even if
	 programmed otherwise. For proper operation, this redirection table entry
	 must be programmed to edge triggered interrupt.
	 110 Reserved
	 111 ExtINT Deliver the signal to the INTR signal of all processor cores listed in the
	 destination as an interrupt that originated in an externally connected
	 (8259A-compatible) interrupt controller. The INTA cycle that corresponds
	 to this ExtINT delivery is routed to the external controller that is expected
	 to supply the vector. A Delivery Mode of "ExtINT"  requires an edge
	 trigger mode. */
#define IO_REDIRECTION_TABLE_REG_DELIVERY_MODE_FIXED (0x0 << 8)
#define IO_REDIRECTION_TABLE_REG_DELIVERY_MODE_LOWEST_PRIORITY (0x1 << 8)
#define IO_REDIRECTION_TABLE_REG_DELIVERY_MODE_SYSTEM_MANAGEMENT_INTERRUPT (0x2 << 8)
/* 3: reserved */
#define IO_REDIRECTION_TABLE_REG_DELIVERY_MODE_NON_MASKED_INTERRUPT (0x4 << 8)
#define IO_REDIRECTION_TABLE_REG_DELIVERY_MODE_INIT (0x5 << 8)
#define IO_REDIRECTION_TABLE_REG_DELIVERY_MODE_EXTINT (0x7 << 8)
/* 7-0: Interrupt Vector (INTVEC) - R/W:  The vector field is an 8 bit field containing the interrupt
	 vector for this interrupt. Vector values range from 10h to FEh. */
#define IO_REDIRECTION_TABLE_REG_INTERUPT_VECTOR_MASK 0x000000ff


#define NR_REDIRECTION_TABLE_REGS 24


#ifndef __ASSEMBLY__


#endif /* !__ASSEMBLY__ */

#endif	/* __MCUBE_X86_IOAPIC_H__ */
