/**
 * @file include/sim/addr.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_SIM_ADDR_H__
#define __MCUBE_SIM_ADDR_H__
/* NOTE: These are dummies. */

#define virt_to_phys(addr) (addr)
#define phys_to_virt(addr) (addr)

#define THREAD_STACK_ADDR(cpu, id) ((unsigned long) &stack[cpu][STACK_SIZE])


#endif /* __MCUBE_SIM_ADDR_H__ */
