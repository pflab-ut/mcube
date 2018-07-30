/**
 * @file include/arm/machine.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_ARM_MACHINE_H__
#define __MCUBE_ARM_MACHINE_H__


#if CONFIG_ARCH_ARM_RASPI3
#include <arm/raspi3/raspi3_header.h>
#elif CONFIG_ARCH_ARM_SYNQUACER
#include <arm/synquacer/synquacer_header.h>
#else
#error "Unknown Machine"
#endif /* CONFIG_ARCH_ARM_RASPI3 */


#endif /* __MCUBE_ARM_MACHINE_H__ */
