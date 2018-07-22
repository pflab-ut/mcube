/**
 * @file include/x86/segment.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_SEGMENT_H__
#define __MCUBE_X86_SEGMENT_H__


// Segment selector values for segment registers.
#define SEGMENT_SELECTOR_KERNEL_DATA  0x08
#define SEGMENT_SELECTOR_KERNEL_CODE  0x10
#define SEGMENT_SELECTOR_USER_DATA    0x18
#define SEGMENT_SELECTOR_USER_CODE    0x20
#define SEGMENT_SELECTOR_TSS          0x28


// Model-specific registers used to set up system calls.
#define MSR_IA32_STAR   0xc0000081
#define MSR_IA32_LSTAR  0xc0000082
#define MSR_IA32_FMASK  0xc0000084

#endif /*  __MCUBE_X86_SEGMENT_H__ */

