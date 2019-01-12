/**
 * @file include/mcube/mcube.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_MCUBE_H__
#define __MCUBE_MCUBE_MCUBE_H__


#include <mcube/mcube_pre_header.h>

#if CONFIG_ARCH_SIM
#include <sim/sim_header.h>
#elif CONFIG_ARCH_X86
#include <x86/x86_header.h>
#elif CONFIG_ARCH_ARM
#include <arm/arm_header.h>
#elif CONFIG_ARCH_AXIS
#include <axis/axis_header.h>
#else
#error "Unknown Architecture"
#endif

#include <mcube/mcube_post_header.h>

#include <debug/debug_header.h>


#endif /* __MCUBE_MCUBE_MCUBE_H__ */
