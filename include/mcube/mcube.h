/**
 * @file include/mcube/mcube.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_MCUBE_H__
#define __MCUBE_MCUBE_MCUBE_H__

#include <mcube/config.h>
#include <mcube/c_header.h>
#include <mcube/mcube_pre_header.h>
#include <net/net_header.h>
#include <fs/fs_header.h>

#if CONFIG_ARCH_SIM
#include <sim/sim_header.h>
#elif CONFIG_ARCH_X86_64
#include <x86_64/x86_64_header.h>
#elif CONFIG_ARCH_AARCH64_RASPI3 || CONFIG_ARCH_AARCH64_SYNQUACER
#include <aarch64/aarch64_header.h>
#elif CONFIG_ARCH_AXIS
#include <axis/axis_header.h>
#else
#error "Unknown Architecture"
#endif


#include <mcube/mcube_post_header.h>

#include <debug/debug_header.h>


#endif /* __MCUBE_MCUBE_MCUBE_H__ */
