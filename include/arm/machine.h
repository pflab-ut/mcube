/**
 * @file include/arm/machine.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_ARM_MACHINE_H__
#define __MCUBE_ARM_MACHINE_H__

#define CONFIG_ARM_MACHINE_RASPI3 1
#define CONFIG_ARM_MACHINE_SYNQUACER 0


#if CONFIG_ARM_MACHINE_RASPI3
#include <arm/machine_raspi3/machine_raspi3_header.h>
#elif CONFIG_ARM_MACHINE_SYNQUACER
#include <arm/machine_synquacer/machine_synquacer_header.h>
#else
#error "Unknown Machine"
#endif /* CONFIG_ARCH_MACHINE_RASPI3 */



#define virt_to_phys(addr) (addr)
#define phys_to_virt(addr) (addr)

#endif /* __MCUBE_ARM_MACHINE_H__ */
