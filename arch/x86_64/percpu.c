/**
 * @file arch/x86_64/percpu.c
 *
 * @author Hiroyuki Chishiro
 */
/*
 * Per-CPU Data Area Init
 *
 * Copyright (C) 2011 Ahmed S. Darwish <darwish.07@gmail.com>
 *
 * The real deal is in the headers: this is only init & test-cases.
 */

#include <mcube/mcube.h>

/*
 * Initialize the calling CPU's per-CPU area.
 */
void percpu_area_init(enum cpu_type t)
{
  if (t == BOOTSTRAP) {
    set_gs(BOOTSTRAP_PERCPU_AREA);
  }

  /* else, we're on a secondary core where %gs
   * is already set-up by the trampoline. */

  percpu_set(self, get_gs());
  sched_percpu_area_init();
}

