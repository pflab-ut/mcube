/**
 * @file include/mcube/mcube_pre_header.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_MCUBE_PRE_HEADER_H__
#define __MCUBE_MCUBE_MCUBE_PRE_HEADER_H__


/* early include due to dependency */
#include <mcube/types.h>
#include <mcube/compiler.h>
#include <mcube/math.h>
#include <mcube/kernel.h>
#include <mcube/stdio.h>
#include <mcube/linkage.h>
#include <mcube/irq.h>
#include <mcube/cpu.h>
#include <mcube/limits.h>
#include <mcube/list.h>
#include <mcube/sched.h>
#include <mcube/spinlock.h>
#include <mcube/rwlock.h>
#include <mcube/seqlock.h>
#include <mcube/stat.h>

/* alphabetical order */
#include <mcube/bitops.h>
#include <mcube/buffer.h>
#include <mcube/buffer_dumper.h>
#include <mcube/byteops.h>
#include <mcube/core_state.h>
#include <mcube/ctype.h>
#include <mcube/dmac.h>
#include <mcube/errno_base.h>
#include <mcube/errno.h>
#include <mcube/fs.h>
#include <mcube/hash.h>
#include <mcube/heap.h>
#include <mcube/io.h>
#include <mcube/llist.h>
#include <mcube/kmalloc.h>
#include <mcube/mm.h>
#include <mcube/mt.h>
#include <mcube/semaphore.h>
#include <mcube/stdalgo.h>
#include <mcube/stdlib.h>
#include <mcube/string.h>
#include <mcube/syscall.h>
#include <mcube/timer.h>
#include <mcube/uart.h>
#include <mcube/unrolled_list.h>
#include <mcube/user.h>


#endif /* __MCUBE_MCUBE_MCUBE_PRE_HEADER_H__ */
