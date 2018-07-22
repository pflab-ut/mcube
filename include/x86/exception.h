/**
 * @file include/x86/exception.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_EXCEPTION_H__
#define __MCUBE_X86_EXCEPTION_H__
//============================================================================
/// @file       exception.h
/// @brief      CPU exceptions.
//
//  Copyright 2016 Brett Vickers.
//  Use of this source code is governed by a BSD-style license
//  that can be found in the MonkOS LICENSE file.
//============================================================================

#ifndef __ASSEMBLY__

// CPU exception constants
#define EXCEPTION_DIVBYZERO            0x00
#define EXCEPTION_DEBUG                0x01
#define EXCEPTION_NMI                  0x02
#define EXCEPTION_BREAKPOINT           0x03
#define EXCEPTION_OVERFLOW             0x04
#define EXCEPTION_BOUNDS               0x05
#define EXCEPTION_INVALID_OPCODE       0x06
#define EXCEPTION_NO_DEVICE            0x07
#define EXCEPTION_DOUBLE_FAULT         0x08
#define EXCEPTION_COPROCESSOR          0x09
#define EXCEPTION_INVALID_TSS          0x0a
#define EXCEPTION_SEGMENT_NOT_PRESENT  0x0b
#define EXCEPTION_STACK_FAULT          0x0c
#define EXCEPTION_GENERAL_PROTECTION   0x0d
/* 0x0f: reserved */
#define EXCEPTION_PAGE_FAULT           0x0e
#define EXCEPTION_FPU                  0x10
#define EXCEPTION_ALIGNMENT            0x11
#define EXCEPTION_MACHINE_CHECK        0x12
#define EXCEPTION_SIMD                 0x13
#define EXCEPTION_VIRTUALIZATION       0x14
/* 0x15-1d: reserved */
#define EXCEPTION_SECURITY             0x1e
/* 0x1f: reserved */

/* 0x20-0xff: maskable interrupts */
#define SCHED_IRQ 0x20
#define INTERRUPT_FATAL 0xff



//----------------------------------------------------------------------------
//  @function   exceptions_init
/// @brief      Initialize all exception handling routines.
//----------------------------------------------------------------------------
void init_exception(void);
#endif	/* !__ASSEMBLY__ */

#endif	/* __MCUBE_X86_DUMP_H__ */
