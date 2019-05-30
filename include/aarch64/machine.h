/**
 * @file include/aarch64/machine.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AARCH64_MACHINE_H__
#define __MCUBE_AARCH64_MACHINE_H__


#if CONFIG_ARCH_AARCH64_RASPI3
#include <aarch64/raspi3/raspi3_header.h>
#elif CONFIG_ARCH_AARCH64_SYNQUACER
#include <aarch64/synquacer/synquacer_header.h>
#else
#error "Unknown Machine"
#endif /* CONFIG_ARCH_AARCH64_RASPI3 */


#endif /* __MCUBE_AARCH64_MACHINE_H__ */
