/**
 * @file include/mcube/mcube_pre_header.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_MCUBE_MCUBE_PRE_HEADER_H__
#define	__MCUBE_MCUBE_MCUBE_PRE_HEADER_H__


/* early include due to dependency */
#include <mcube/config.h>
#include <mcube/types.h>
#include <mcube/atomic.h>
#include <mcube/stdio.h>
#include <mcube/stddef.h>
#include <mcube/linkage.h>
#include <mcube/irq.h>
#include <mcube/spinlock.h>
#include <mcube/math.h>
#include <mcube/cpu.h>
#include <mcube/limits.h>
#include <mcube/sched.h>
#include <mcube/main.h>

/* alphabetical order */
#include <mcube/attribute.h>
#include <mcube/barrier.h>
#include <mcube/bitops.h>
#include <mcube/buffer.h>
#include <mcube/byteops.h>
#include <mcube/cache.h>
#include <mcube/compiler.h>
#include <mcube/ctype.h>
#include <mcube/dmac.h>
#include <mcube/errno-base.h>
#include <mcube/errno.h>
#include <mcube/fs.h>
#include <mcube/io.h>
#include <mcube/metric.h>
#include <mcube/mm.h>
#include <mcube/mt.h>
#include <mcube/rand.h>
#include <mcube/rwlock.h>
#include <mcube/semaphore.h>
#include <mcube/seqlock.h>
#include <mcube/stdalgo.h>
#include <mcube/stdarg.h>
#include <mcube/stdlib.h>
#include <mcube/string.h>
#include <mcube/syscall.h>
#include <mcube/timer.h>
#include <mcube/uart.h>
#include <mcube/user.h>


#endif /* __MCUBE_MCUBE_MCUBE_PRE_HEADER_H__ */
