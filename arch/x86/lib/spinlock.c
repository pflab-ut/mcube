/*
 * SMP spinlocks
 *
 * Copyright (C) 2009-2011 Ahmed S. Darwish <darwish.07@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 2.
 *
 * Textbook locks: Allow only one code path in the critical region.
 *
 * The LOCK prefix insures that a read-modify-write operation on memory
 * is carried out atomically. While the LOCK# signal is asserted,
 * requests from other processors or bus agents for control of the bus
 * are blocked.
 *
 * If the area of memory being locked is cached in the processor and is
 * completely contained in a cache line, the CPU may not assert LOCK# on
 * the bus. Instead, it will modify the location internally and depend
 * on the “cache locking” feature of the x86 cache coherency mechanism.
 *
 * During cache locking, cache coherency prevents two or more cores that
 * have cached the same area of memory from simultaneously moifying data
 * in that area (cache line).
 *
 * NOTE1: Always use x86 'pause' for busy-wait loops, check cpu_pause()
 * [kernel.h] for rationale; it's is an agnostic REP NOP for old cores.
 *
 * Please check the “Intel 64 and IA-32 Optimization Reference Manual”,
 * Chapter 8: Multicore and Hyper-Threading Tech, for further details.
 *
 * NOTE2: Without caution, it's possible to interrupt the very core
 * holding a lock: the irq handler may busy-loop waiting for the core
 * to release its lock, but the core is already interrupted and can't
 * release such lock till the irq handler finishes [deadlock]. Thus,
 * _local_ interrupts are disabled before holding ANY locks.
 *
 * Not to halt the system in the unfortunate state of lock contention,
 * if IRQs were originally on, we re-enable them while spinning.
 *
 * NOTE3: An intentional byproduct of disabling IRQs at lock entrance
 * is disabling kernel preemption in all critical regions. This is a
 * necessity:
 * a) If a thread holding a spin lock got preempted, it will let a
 *    possibly huge number of other threads consume their entire time
 *    slice (which is in the order of milliseconds) just spinning!
 * b) If that preempted thread (which is now in the scheduler runqueue)
 *    got killed, we can deadlock the entire system!
 * c) In the Uni-processor case (where no locking is actually done),
 *    classical race conditions may arise if the newly scheduled thread
 *    accessed the same critical-region the older thread was accessing
 *    while being preempted. Hence these locks act as critical region
 *    markers in the preemptive uni-processor case.
 *
 * This spin design is closer, in spirit, to FreeBSD rather than Linux.
 * Upon lock entrance, the latter only disables preemption by default.
 */

#include <mcube/mcube.h>


/*
 * SPIN_UNLOCKED = 1, SPIN_LOCKED = 0
 */

void spin_init(struct lock_spin *lock)
{
  lock->val = SPIN_UNLOCKED;
}

