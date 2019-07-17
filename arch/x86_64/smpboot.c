/**
 * @file arch/x86_64/smpboot.c
 *
 * @author Hiroyuki Chishiro
 */
/*
 * Multiple-Processor (MP) Initialization
 *
 * Copyright (C) 2009-2013 Ahmed S. Darwish <darwish.07@gmail.com>
 */

#include <mcube/mcube.h>


/**
 * @var nr_alive_cpus
 * @brief Number of active CPUs so far: BSC + SIPI-started AP
 * cores that are now verifiably executing kernel code.
 */
static int nr_alive_cpus = 1;

/*
 * Common Inter-Processor Interrupts
 */

/**
 * @fn static inline void send_init_ipi(int apic_id)
 * @brief Zero INIT vector field for "future compatibility".
 *
 * @param apic_id APIC ID.
 */
static inline void send_init_ipi(int apic_id)
{
  apic_send_ipi(apic_id, APIC_DELMOD_INIT, 0);
}

/**
 * @fn static inline void send_startup_ipi(int apic_id, uint32_t start_vector)
 * @brief ICR's vector field is 8-bits; For the value 0xVV,
 * SIPI target core will start from 0xVV000.
 *
 * @param apic_id APIC ID.
 * @param start_vector Start vector.
 */
static inline void send_startup_ipi(int apic_id, uint32_t start_vector)
{
  assert(page_aligned(start_vector));
  assert(start_vector >= 0x10000 && start_vector <= 0x90000);

  apic_send_ipi(apic_id, APIC_DELMOD_START, start_vector >> 12);
}

/**
 * @def MAX_SIPI_RETRY
 * @brief "It is up to the software to determine if the SIPI was
 * not successfully delivered and to reissue the SIPI if
 * necessary." --Intel
 */
#define MAX_SIPI_RETRY 3

/**
 * @fn static int start_secondary_cpu(struct percpu *cpu,
 *                                    struct smpboot_params *params)
 * @brief Do not broadcast Intel's INIT-SIPI-SIPI sequence as this
 * may wake-up CPUs marked by the BIOS as faulty, or defeat
 * the user choice of disabing a certain core in BIOS setup.
 *
 * The trampoline code cannot also be executed in parallel.
 *
 * FIXME: 200 micro-second delay between the SIPIs
 * FIXME: fine-grained timeouts using micro-seconds
 *
 * @param cpu CPU.
 * @param params SMP boot parameters.
 * @return Zero if success, and nonzero if failure.
 */
static int start_secondary_cpu(struct percpu *cpu,
                               struct smpboot_params *params)
{
  int count, acked, timeout, apic_id;

  barrier();
  count = nr_alive_cpus;
  apic_id = cpu->apic_id;

  /*
   * Personally allocate a 'current' thread descriptor and a stack
   * for the given CPU. It can't do this by itself since kmalloc()
   * uses lots of spinlocks, which need an already allocated
   * 'current' thread descriptor (cyclic dependency.)
   *
   * We've statically allocated such structures for the boot core.
   */

  /* All cores initialize their own 'current'; dont do it for 'em. */
  if (!(cpu->__current = kmalloc(sizeof(struct process)))) {
    panic("Error: cannot allocate memory %lu\n", sizeof(struct process));
  }

  if (!(params->stack_ptr = kmalloc(STACK_SIZE))) {
    panic("Error: cannot allocate memory %lu\n", STACK_SIZE);
  }

  params->stack_ptr += STACK_SIZE;
  params->percpu_area_ptr = cpu;

  memcpy(TRAMPOLINE_PARAMS, params, sizeof(*params));

  /* INIT: wakeup the core from its deep (IF=0)
   * halted state and let it wait for the SIPIs */
  send_init_ipi(apic_id);
  acked = apic_ipi_acked();

  if (!acked) {
    printk("SMP: Failed to deliver INIT to CPU#%d\n", apic_id);
    goto fail;
  }

  pit_udelay(10 * 1000);

  for (int j = 1; j <= MAX_SIPI_RETRY; j++) {
    send_startup_ipi(apic_id, SMPBOOT_START);
    acked = apic_ipi_acked();

    if (acked) {
      break;
    }

    printk("SMP: Failed to deliver SIPI#%d to CPU#%d\n",
           j, apic_id);

    if (j == MAX_SIPI_RETRY) {
      printk("SMP: Giving-up SIPI delivery\n");
      goto fail;
    }

    printk("SMP: Retrying SIPI delivery\n");
  }

  /* The just-started AP core should now signal us
   * by incrementing the active-CPUs counter by one */
  timeout = 1000;

  while (timeout-- && count == nr_alive_cpus) {
    barrier();
    pit_udelay(1 * 1000);
  }

  if (timeout == -1) {
    printk("SMP: Timeout waiting for CPU#%d to start\n",
           apic_id);
    goto fail;
  }

  return 0;

fail:
  kfree(cpu->__current);
  kfree(params->stack_ptr - STACK_SIZE);
  return 1;
}

/**
 * @def for_all_cpus(cpu)
 * @param cpu iterator of type "struct percpu *".
 */
#define for_all_cpus(cpu)      \
  for (cpu = &cpus[0]; cpu != &cpus[mptables_get_nr_cpus()]; cpu++)

/**
 * @def for_all_cpus_except_bootstrap(cpu)
 * @param cpu iterator of type "struct percpu *".
 */
#define for_all_cpus_except_bootstrap(cpu)                          \
  for (cpu = &cpus[1]; cpu != &cpus[mptables_get_nr_cpus()]; cpu++)


/*
 * AP cores C code entry. We come here from the trampoline,
 * which has assigned us a unique stack, the per-CPU area
 * addr in %gs, and bootstrap's gdt, idt, and page tables.
 */
__noreturn void secondary_start(void)
{
  union apic_id id;
  int argc = 1;
  char exec_file[] = {"build/mcube"};
  char *argv[NR_ARGS] = {exec_file};

  /* Quickly tell the parent we're alive */
  ++nr_alive_cpus;

  schedulify_this_code_path(SECONDARY);
  apic_local_regs_init();

  /* Assert validity of our per-CPU area */
  id.raw = apic_read(APIC_ID);
  assert(id.id == percpu_get(apic_id));

  printk("SMP: CPU apic_id=%d started\n", id.id);

  enable_local_irq();

  user_ap_main(argc, argv);
  printk("user_ap_main() end\n");
  halt();
}


int smpboot_get_nr_alive_cpus(void)
{
  barrier();
  return nr_alive_cpus;
}

void smpboot_init(void)
{
  int nr_cpus;
  struct smpboot_params *params;
  struct percpu *cpu;

  smpboot_params_validate_offsets();

  if (!(params = kmalloc(sizeof(struct smpboot_params)))) {
    panic("Error: cannot allocate memory %lu\n", sizeof(struct smpboot_params));
  }


  params->cr3 = get_cr3();
  params->idtr = get_idt();
  params->gdtr = get_gdt();

  nr_cpus = mptables_get_nr_cpus();
  printk("SMP: %d usable CPU(s) found\n", nr_cpus);

  memcpy(TRAMPOLINE_START, trampoline, trampoline_end - trampoline);

  for_all_cpus_except_bootstrap(cpu) {
    if (start_secondary_cpu(cpu, params)) {
      panic("SMP: Could not start-up all AP cores\n");
    }
  }

  kfree(params);

  barrier();
  assert(nr_alive_cpus == nr_cpus);
}

