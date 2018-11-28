/**
 * @file kernel/sched.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

struct thread_struct *current_th[NR_INTRA_KERNEL_CPUS];
struct thread_struct *prev_th[NR_INTRA_KERNEL_CPUS];

volatile atomic_int sched_lock = SPIN_UNLOCKED;

struct thread_struct kernel_th[NR_INTRA_KERNEL_CPUS];


unsigned long sys_jiffies = 0;

#if CONFIG_ARCH_SIM
unsigned long exec_jiffies = 0;
#endif /* CONFIG_ARCH_SIM */

unsigned long nr_tasks = 0;

int bindex[NR_INTRA_KERNEL_CPUS];
int bidleindex;

unsigned long sched_time;
volatile uint32_t sched_end = FALSE;

#if SCHED_FP
void set_priority(struct thread_struct *th)
{
  /* the priority of idle thread is ULONG_MAX */
  /* smaller value, higher priority */
  /* priority 0 is the highest priority for US or ZL. */
  th->priority = th->id;
  /* NOTE: assume that period is an integer multiple of 100. */
  /* period range should be within [100,3200] */
  //              th->priority = th->sched.period / 100;
  //              th->priority = th->sched.period / 100 - 1;
}

#elif SCHED_DP

void set_priority(struct thread_struct *th)
{
  th->priority = th->sched.deadline;
}
#else
#error "Error: unknown priority"
#endif

void do_sync(void)
{
#if CONFIG_SYNC_PCP || CONFIG_SYNC_SRP
	int i;
	int cpu = get_cpu_id();
	for (i = 0; i < nr_resources; i++) {
		print("current_th: remaining = %llu wcmt[0] = %llu enter_sem = %llu sem = %u\n",
					 current_th[cpu]->sched.remaining, current_th[cpu]->sched.wcmt[0], current_th[cpu]->sched.enter_sem[i], current_th[cpu]->sched.sem[i]);
		/* check if current_th enters critical section of sem[j] */
		if (current_th[cpu]->sched.sem[i] > 0) {
			if (current_th[cpu]->sched.remaining == current_th[cpu]->sched.enter_sem[i]) {
				/* Now enter critical section of sem[i] */
				print("th->id = %llu enter critical section %d\n", current_th[cpu]->id, i);
				sync_sem_down(&sync_sem[i]);
			}
			if (current_th[cpu]->sched.remaining == current_th[cpu]->sched.enter_sem[i] + current_th[cpu]->sched.sem[i]) {
				/* Now exit critical section of sem[j] */
				print("th->id = %llu exit critical section %d\n", current_th[cpu]->id, i);
				sync_sem_up(&sync_sem[i]);
			}
		}
	}
#endif
}


void do_release(void)
{
  //	unsigned long release_latency = current_jiffies_time();
	struct thread_struct *th;
  unsigned long cpu = get_cpu_id();
  pdebug_array(run_tq[cpu].array);
	PDEBUG("run_tq: bheap\n");
  pdebug_bheap(&run_tq[cpu], run_tq[cpu].head);


	//pdebug_jiffies();
	//	pdebug_thread(current_th);


	//	print("sleep_tq[cpu]->sched.release = %d\n", sleep_tq[cpu]->sched.release);

	pdebug_bitmap(run_tq[cpu].bitmap);
	//	pdebug_bheap(&run_tq, run_tq[cpu].head);
  pdebug_sleep_tq();

	while (sleep_tq[cpu]->sched.release <= get_current_jiffies()) {
		th = sleep_tq[cpu];
    th->job_id++;
		sleep_tq[cpu] = dequeue_thread(sleep_tq[cpu], th);
		enqueue_rq(&run_tq[cpu], th);

    th->sched.remaining = th->sched.wcet;
		//		pdebug_jiffies();
	}
  pdebug_array(run_tq[cpu].array);
}


void do_sched(void)
{
  unsigned long cpu = get_cpu_id();
  printk("do_sched()\n");
	struct thread_struct *th;
  pdebug_deadline_tq();


	/* jump to algorithm-specific function */
  //  do_sched_algo();

	/* assign the highest priority task to cpu */
	th = pick_next_task();
	if (th) {
		current_th[cpu] = th;
	} else {
		current_th[cpu] = &kernel_th[cpu];
	}
  printk("current_th[%lu]->id = %lu\n", cpu, current_th[cpu]->id);

	if (prev_th[cpu] != &kernel_th[cpu] && is_preempted(prev_th[cpu])) {
		/* preemption occurs */
		prev_th[cpu]->state = READY;
    end_budget(prev_th[cpu]);
	}

  //	switch_to(tasks[next]);
	current_th[cpu]->state = RUNNING;
  if (prev_th[cpu] != current_th[cpu]) {
    begin_budget(current_th[cpu]);
  }
  //  switch_to(current_th[cpu]);
	//	pdebug_jiffies();
	printk("do_sched(): end\n");
}


int check_deadline_miss(void)
{
  unsigned long cpu = get_cpu_id();
	if (deadline_tq[cpu]->sched.deadline <= get_current_jiffies()) {
		PDEBUG("cpu %lu get_current_jiffies() %lu\n", cpu, get_current_jiffies());
		PDEBUG("task %lu missed deadline\n", deadline_tq[cpu]->id);
		PDEBUG("if the following inequality holds,\n");
		PDEBUG("then you must adjust parameters:\n");
		PDEBUG("current_th->sched.deadline = %lu\n", deadline_tq[cpu]->sched.deadline);
		return FALSE;
	}
	return TRUE;
}


int run(unsigned long nr_threads)
{
	unsigned int i;
	int ret;
  print("run()\n");
  //  asm volatile("move %0, $fp" : "=r"(current_fp));
  //  print("current_fp = 0x%x\n", current_fp);
  
  //  pdebug_array(run_tq[cpu].array);
#if 1
  for (i = 0; i < nr_threads; i++) {
		//		current_cpu = i;
    /* check feasibility and activate */
    if ((ret = do_activate(&ths[i])) != 0) {
      //			print("Error: do_activate(): %d\n", ret);
      return 1;
    }
		//		do_sched_trace_thread_name(&ths[i]);
  }
#endif

	sys_jiffies = 0;
	sched_end = FALSE;

  print("run()2\n");
  for (i = 0; i < NR_INTRA_KERNEL_CPUS; i++) {
    current_th[i] = prev_th[i] = &kernel_th[i];
    current_th[i]->id = 0;
    current_th[i]->stack_top = KERNEL_THREAD_STACK_ADDR(i);
    print("current_th[%u]->stack.top = 0x%lx\n", i, current_th[i]->stack_top);
  }
  //		print("current_th[%d] = %x\n", i, current_th[i]);

	//	inf_loop();
	//syscall0(SYS_sched);

  do_release();
  start_timer(0);

  
  //  generate_software_interrupt(0);
  
	/* idle thread start */
	while (sched_end == FALSE) {
    //print("");
    // print("get_timer_count() = %lu\n", get_timer_count());
    // printk("CNTV_TVAL: %lu\n", get_cntvct_el0()); 
    //    print("0");
    //    halt();
    //    print("sched_end = %d\n", sched_end);
		//		print("idle!");
    //    asm volatile("move %0, $sp" : "=r"(current_fp));
    //    print("current_fp = 0x%x\n", current_fp);
    //    nop();
    //    delay(1000);
    //    wait_until_next_interrupt();
	}
	stop_timer(0);

  print("run() end\n");
  //  asm volatile("move %0, $fp" : "=r"(current_fp));
  //  print("current_fp = 0x%x\n", current_fp);
  //  asm volatile("move %0, $sp" : "=r"(current_sp));
  //  print("current_sp = 0x%x\n", current_sp);
  
	return 0;
}

void schedule_tail(void)
{
  preempt_enable();
}



void init_sched(void)
{
  unsigned long cpu = get_cpu_id();
  unsigned int i, j;
  sys_jiffies = 0;

  for (i = 0; i < NR_INTRA_KERNEL_CPUS; i++) {
    kernel_th[i] = (struct thread_struct) INIT_KERNEL_THREAD(cpu);
    kernel_th[i].thflags = THFLAGS_START_TH;
    run_tq[i].util = 0;
    run_tq[i].nr_threads = 0;
    for (j = 0; j < NR_PRIORITY_BITMAPS; j++) {
      run_tq[i].bitmap[j] = 0;
    }
  }
  current_th[cpu] = prev_th[cpu] = &kernel_th[cpu];
  deadline_tq[cpu] = &kernel_th[cpu];
  sleep_tq[cpu] = &kernel_th[cpu];

}
