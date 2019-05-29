/**
 * @file arch/x86_64/init.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

/*
 * TSS (104 bytes)
 */
struct tss {
  uint32_t reserved1;
  uint32_t rsp0l;
  uint32_t rsp0h;
  uint32_t rsp1l;
  uint32_t rsp1h;
  uint32_t rsp2l;
  uint32_t rsp2h;
  uint32_t reserved2;
  uint32_t reserved3;
  uint32_t ist1l;
  uint32_t ist1h;
  uint32_t ist2l;
  uint32_t ist2h;
  uint32_t ist3l;
  uint32_t ist3h;
  uint32_t ist4l;
  uint32_t ist4h;
  uint32_t ist5l;
  uint32_t ist5h;
  uint32_t ist6l;
  uint32_t ist6h;
  uint32_t ist7l;
  uint32_t ist7h;
  uint32_t reserved4;
  uint32_t reserved5;
  uint16_t reserved6;
  uint16_t iomap;
} __packed;

/*
 * Stackframe for 64-bit mode
 */
struct stackframe64 {
  /* Segment registers */
  uint16_t gs;
  uint16_t fs;

  /* Base pointer */
  uint64_t bp;

  /* Index registers */
  uint64_t di;
  uint64_t si;

  /* Generic registers */
  uint64_t r15;
  uint64_t r14;
  uint64_t r13;
  uint64_t r12;
  uint64_t r11;
  uint64_t r10;
  uint64_t r9;
  uint64_t r8;
  uint64_t dx;
  uint64_t cx;
  uint64_t bx;
  uint64_t ax;

  /* Restored by `iretq' instruction */
  uint64_t ip;            /* Instruction pointer */
  uint64_t cs;            /* Code segment */
  uint64_t flags;         /* Flags */
  uint64_t sp;            /* Stack pointer */
  uint64_t ss;            /* Stack segment */
} __attribute__((packed));

#if 0
/*
 * Task
 */
struct arch_task {
  /* Restart point (stackframe) */
  struct stackframe64 *rp;
  /* SP0 for TSS */
  uint64_t sp0;
  /* User stack */
  void *ustack;
  /* Kernel stack */
  void *kstack;
  /* Kernel task (architecture-independent data structure) */
  task_t *task;
} __attribute__((packed));

/*
 * Processor's task information (24 bytes)
 */
struct arch_cpu_data {
  struct arch_task *cur_task;
  struct arch_task *next_task;
  struct arch_task *idle_task;
} __attribute__((packed));
#endif

/* Maximum number of supported processors */
#define MAX_PROCESSORS          256

/* GDT and IDT */
//#define GDT_ADDR                0xc0074000ULL
//#define GDT_ADDR                0xffffffff80074000ULL
#define GDT_MAX_SIZE            0x2000
//#define IDT_ADDR                0xc0076000ULL
#define IDT_MAX_SIZE            0x2000

uint8_t GDT_ADDR[GDT_MAX_SIZE] PAGE_ALIGNMENT;
uint8_t IDT_ADDR[IDT_MAX_SIZE] PAGE_ALIGNMENT;


/* Per-core data (128-byte per core) */
#define CPU_DATA_SIZE_SHIFT     7
#define CPU_DATA_SIZE           (1 << CPU_DATA_SIZE_SHIFT)

#if 0
#define CPU_DATA_BASE           0xc0060000
#define CPU_DATA(i)             (CPU_DATA_BASE + ((i) << 7))
#define CPU_TSS_BASE            (CPU_DATA_BASE + 0)
#define CPU_TSS(i)              (CPU_TSS_BASE + ((i) << 7))
#define CPU_TASK_BASE           (CPU_DATA_BASE + 104)
#define CPU_TASK(i)             (CPU_TASK_BASE + ((i) << 7))
#else
uint8_t CPU_DATA_BASE[CPU_DATA_SIZE] PAGE_ALIGNMENT;
#define CPU_DATA(i)             (&CPU_DATA_BASE[(i) << 7])
#define CPU_TSS_BASE            (&CPU_DATA_BASE[0])
//#define CPU_TSS(i)              (CPU_TSS_BASE + ((i) << 7))
#define CPU_TSS(i)              (&CPU_DATA_BASE[(i) << 7])
#define CPU_TASK_BASE           (&CPU_DATA_BASE[104])
#define CPU_TASK(i)             (&CPU_TASK_BASE[104 + ((i) << 7)])
#endif


/* GDT type */
#define GDT_TYPE_EX     8
#define GDT_TYPE_DC     4
#define GDT_TYPE_RW     2
#define GDT_TYPE_AC     1
/* IDT flags */
#define IDT_PRESENT     0x80
#define IDT_INTGATE     0x0e
#define IDT_TRAPGATE    0x0f

#define TSS_INACTIVE    0x9
#define TSS_BUSY        0xb

/*
 * Global Descriptor
 *  base: 32-bit base address of the memory space
 *  limit: 20-bit size minus 1
 *  type: (Executable, Direction/Conforming, Readable/Writable, Accessed)
 *  DPL: Privilege (0: highest, 3: lowest)
 *  P: Present bit
 *  G: Granularity; 0 for 1 byte block, 1 for 4 KiB block granularity
 *  DB: Size; 0 for 16 bit mode, 1 for 32 bit mode
 *  L: 1 for 64 bit mode
 *  A: 0
 *  S: 1
 */
struct gdt_desc {
  uint16_t w0;        /* limit[0:15] */
  uint16_t w1;        /* base[0:15] */
  uint16_t w2;        /* base[16:23] type[0:3] S DPL[0:1] P  */
  uint16_t w3;        /* limit[16:19] A L DB G base[24:31] */
} __attribute__((packed));

/*
 * TSS
 */
struct gdt_desc_tss {
  uint16_t w0;
  uint16_t w1;
  uint16_t w2;
  uint16_t w3;
  uint16_t w4;
  uint16_t w5;
  uint16_t w6;
  uint16_t w7;
} __attribute__((packed));

/*
 * Global Descriptor Table Register
 */
struct gdtr {
  uint16_t size;
  uint64_t base;      /* (struct gdt_desc *) */
} __attribute__((packed));

void lgdt(void *gdtr, uint64_t selector);


/*
 * Setup a null descriptor
 */
static void gdt_setup_desc_null(struct gdt_desc *e)
{
  e->w0 = 0;
  e->w1 = 0;
  e->w2 = 0;
  e->w3 = 0;
}

/*
 * Setup global descriptor table entry
 */
static void gdt_setup_desc(struct gdt_desc *e, uint32_t base, uint32_t limit,
                           uint8_t type,
                           uint8_t dpl, uint8_t l, uint8_t db, uint8_t g)
{
  limit &= 0xfffff;
  type &= 0xf;

  /* Setup the descriptor: see desc.h for detailed information */
  /* P=1, A=0, S=1 */
  /* 16bit: l=0, db=0
     32bit: l=0, db=1
     64bit: l=1, db=0 */
  e->w0 = limit & 0xffff;
  e->w1 = base & 0xffff;
  e->w2 = ((base >> 16) & 0xff) | ((uint64_t)type << 8) | ((uint64_t)1 << 12)
          | ((uint64_t)dpl << 13) | ((uint64_t)1 << 15);
  e->w3 = ((limit >> 16) & 0xf) | ((uint64_t)l << 5) | ((uint64_t)db << 6)
          | ((uint64_t)g << 7) | (((base >> 24) & 0xff) << 8);
}

/*
 * Setup global descriptor table for TSS
 */
static void gdt_setup_desc_tss(struct gdt_desc_tss *e, uint64_t base,
                               uint32_t limit,
                               uint8_t type, uint8_t dpl, uint8_t g)
{
  limit &= 0xfffff;
  type &= 0xf;

  /* g => *4KiB */
  e->w0 = limit & 0xffff;
  e->w1 = base & 0xffff;
  e->w2 = ((base >> 16) & 0xff) | ((uint64_t)type << 8)
          | ((uint64_t)dpl << 13) | ((uint64_t)1 << 15);
  e->w3 = ((limit >> 16) & 0xf) | ((uint64_t)g << 7)
          | (((base >> 24) & 0xff) << 8);
  e->w4 = base >> 32;
  e->w5 = base >> 40;
  e->w6 = 0;
  e->w7 = 0;
}



/*
 * Initialize global descriptor table
 */
struct gdtr *gdt_init(void)
{
  uint64_t sz;
  struct gdt_desc *gdt;
  struct gdt_desc_tss *tss;
  struct gdtr *gdtr;
  uint8_t code;
  uint8_t data;
  int i;

  sz = GDT_NR * sizeof(struct gdt_desc)
       + MAX_PROCESSORS * sizeof(struct gdt_desc_tss);
  /* GDT register */
  gdtr = (struct gdtr *)(GDT_ADDR + sz);
  /* Global descriptors */
  gdt = (struct gdt_desc *)GDT_ADDR;
  /* Type */
  code = GDT_TYPE_EX | GDT_TYPE_RW;
  data = GDT_TYPE_RW;

  /* Null descriptor */
  gdt_setup_desc_null(&gdt[0]);
  /* Code and data descriptor for each ring */
  gdt_setup_desc(&gdt[1], 0, 0xfffff, code, 0, 1, 0, 1); /* Ring 0 code */
  gdt_setup_desc(&gdt[2], 0, 0xfffff, data, 0, 1, 0, 1); /* Ring 0 data */
  gdt_setup_desc(&gdt[3], 0, 0xfffff, code, 3, 0, 1, 1); /* Ring 3 code */
  gdt_setup_desc(&gdt[4], 0, 0xfffff, data, 3, 0, 1, 1); /* Ring 3 data */
  gdt_setup_desc(&gdt[5], 0, 0xfffff, code, 3, 1, 0, 1); /* Ring 3 code */
  gdt_setup_desc(&gdt[6], 0, 0xfffff, data, 3, 1, 0, 1); /* Ring 3 data */

  /* TSS */
  tss = (struct gdt_desc_tss *)(GDT_ADDR + GDT_TSS_SEL_BASE);

  for (i = 0; i < MAX_PROCESSORS; i++) {
    gdt_setup_desc_tss(&tss[i], (uint64_t) CPU_TSS(i), sizeof(struct tss) - 1,
                       TSS_INACTIVE, 0, 0);
    //    printk("CPU_TSS(%d) = 0x%lx\n", i, CPU_TSS(i));
  }

  /* Set the GDT base address and the table size */
  gdtr->base = (uint64_t)gdt;
  gdtr->size = sz - 1;

  return gdtr;
}

/*
 * Load global descriptor table
 */
void gdt_load(void)
{
  uint64_t sz;
  struct gdtr *gdtr;

  sz = GDT_NR * sizeof(struct gdt_desc)
       + MAX_PROCESSORS * sizeof(struct gdt_desc_tss);
  /* GDT register */
  gdtr = (struct gdtr *)(GDT_ADDR + sz);

  lgdt(gdtr, GDT_RING0_CODE_SEL);
}



static void setup_idt(void)
{
  for (int i = 0; i < EXCEPTION_GATES; i ++) {
    set_intr_gate(i, (void *) &idt_exception_stubs[i]);
  }

  set_intr_gate(HALT_CPU_IPI_VECTOR, halt_cpu_ipi_handler);

  load_idt(&idtdesc);
}

void clear_bss(void)
{
  memset(__bss_start, 0, __bss_end - __bss_start);
}


static void print_memory_info(void)
{
  printk("Mcube Kernel\n\n");

  printk("Text start = 0x%lx\n", __text_start);
  printk("Text end   = 0x%lx\n", __text_end);
  printk("Text size  = %d bytes\n\n", __text_end - __text_start);

  printk("Data start = 0x%lx\n", __data_start);
  printk("Data end   = 0x%lx\n", __data_end);
  printk("Data size  = %d bytes\n\n", __data_end - __data_start);

  printk("BSS start  = 0x%lx\n", __bss_start);
  printk("BSS end    = 0x%lx\n", __bss_end);
  printk("BSS size   = %d bytes\n\n", __bss_end - __bss_start);
}

static void print_vendor_id(void)
{
  char vendor_id[VENDOR_ID_LENGTH + 1];
  cpuid_info_t cinfo;
  cpuid(0x0, &cinfo);
  printk("Largest Standard Function Number Supported: %d\n", cinfo.rax);
  memcpy(&vendor_id[0], &cinfo.rbx, 4);
  memcpy(&vendor_id[4], &cinfo.rdx, 4);
  memcpy(&vendor_id[8], &cinfo.rcx, 4);
  vendor_id[VENDOR_ID_LENGTH] = '\0';
  printk("Vendor ID: %s\n", vendor_id);
}

static void print_simd_info(void)
{
  cpuid_info_t cinfo;
  cpuid(0x1, &cinfo);
  printk("MMX: %s, ", cinfo.rdx & 1 << 23 ? "OK" : "NG");
  printk("SSE: %s, ", cinfo.rdx & 1 << 25 ? "OK" : "NG");
  printk("AVX: %s, ", cinfo.rcx & 1 << 28 ? "OK" : "NG");
  printk("FMA: %s, ", cinfo.rcx & 1 << 12 ? "OK" : "NG");
  cpuid(0x7, &cinfo);
  printk("AVX2: %s\n", cinfo.rbx & 1 <<  5 ? "OK" : "NG");
}


static void print_cpu_brand_info(void)
{
  cpuid_info_t cinfo;
  char cpu_brand[CPU_BRAND_LENGTH + 1];
  cpuid(0x80000002, &cinfo);
  memcpy(&cpu_brand[0], &cinfo, sizeof(cinfo));
  cpuid(0x80000003, &cinfo);
  memcpy(&cpu_brand[16], &cinfo, sizeof(cinfo));
  cpuid(0x80000004, &cinfo);
  memcpy(&cpu_brand[32], &cinfo, sizeof(cinfo));
  cpu_brand[CPU_BRAND_LENGTH] = '\0';
  printk("CPU Brand: %s\n", cpu_brand);
}


static void print_info(void)
{
  print_memory_info();
  print_vendor_id();
  print_simd_info();
  print_cpu_brand_info();
}


void init_arch(void)
{
  /* Before anything else, zero the bss section. As said by C99:
   * ¡ÈAll objects with static storage duration shall be inited
   * before program startup¡É, and that the implicit init is done
   * with zero. Kernel assembly code also assumes a zeroed BSS
   * space */
  clear_bss();
  init_uart();

  /*
   * Very-early setup: Do not call any code that will use
   * printk(), `current', per-CPU vars, or a spin lock.
   */

  setup_idt();
  //  gdt_init();
  //  gdt_load();

  schedulify_this_code_path(BOOTSTRAP);

  /* print information */
  print_info();

  init_socket();
  /*
   * Memory Management init
   */
  /* First, don't override the ramdisk area (if any) */
  ramdisk_init();

  /* Then discover our physical memory map .. */
  e820_init();

  /* and tokenize the available memory into allocatable pages */
  pagealloc_init();

  /* With the page allocator in place, git rid of our temporary
   * early-boot page tables and setup dynamic permanent ones */
  vm_init();

  /* MM basics done, enable dynamic heap memory to kernel code
   * early on .. */
  init_kmalloc();

  //  init_syscall();

  /*
   * Secondary-CPUs startup
   */

  /* Discover our secondary-CPUs and system IRQs layout before
   * initializing the local APICs */
  mptables_init();

  /* Remap and mask the PIC; it's just a disturbance */
  pic_init();

  /* Initialize the APICs (and map their MMIO regs) before enabling
   * IRQs, and before firing other cores using Inter-CPU Interrupts */
  apic_init();
  ioapic_init();


  /* SMP infrastructure ready, fire the CPUs! */
  smpboot_init();

  keyboard_init();

  /* Startup finished, roll-in the scheduler! */
  sched_init();
  enable_local_irq();


  /*
   * Second part of kernel initialization (Scheduler is now on!)
   */

  init_ext2();


  // Signal the secondary cores to run their own test-cases code.
  // They've been waiting for us (thread 0) till all of kernel
  // subsystems has been properly initialized.  Wait No More!
  //  smpboot_trigger_secondary_cores_testcases();

  //  run_test_cases();
  //  user_main(0, NULL);

}

void exit_arch(void)
{
}


/*
 * Bootstrap-CPU start; we came from head.S
 */
__noreturn void kernel_start(void)
{
  int argc = 1;
  char exec_file[] = {"build/mcube"};
  char *argv[NR_ARGS] = {exec_file};
  main(argc, argv);
  halt();
}

void init_arch_ap(void)
{
}


void exit_arch_ap(void)
{
}
