/**
 * @file include/x86/acpi.h
 *
 * @author Hiroyuki Chishiro
 */
/*_
 * Copyright (c) 2018 Hirochika Asai <asai@jar.jp>
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef	__MCUBE_X86_ACPI_H__
#define	__MCUBE_X86_ACPI_H__

/* BIOS data area (BDA): 0x0400--0x04ff; if extended BDA (EDBA) presents, its
   address >> 4 (16 bit) is stored in 0x040e. */
#define BDA_EDBA        0x040e

#define ACPI_TMR_HZ 3579545
#define ACPI_SCI_EN 0x1
#define ACPI_SLP_EN (1<<13)

#ifndef __ASSEMBLY__

/*
 * ACPI configuration
 */
typedef struct {
  /* Local APIC address */
  uint64_t apic_address;
  /* I/O APIC base */
  uint64_t ioapic_base;
  /* ACPI PM timer */
  uint64_t pm_tmr_port;
  uint8_t pm_tmr_ext;
  /* Power control */
  uint32_t pm1a_ctrl_block;
  uint32_t pm1b_ctrl_block;
  /* ACPI SMI command port */
  uint32_t smi_cmd_port;
  /* ACPI enable */
  uint8_t acpi_enable;
  /* CMOS century */
  uint8_t cmos_century;

  /* CPU domain */
  struct {
    int valid;
    uint32_t domain;
  } lapic_domain[MAX_PROCESSORS];
  int num_memory_region;
  struct {
    uint64_t base;
    uint64_t length;
    uint32_t domain;
  } memory_domain[MAX_MEMORY_REGIONS];
} acpi_t;



/*
 * Root System Descriptor Pointer (RSDP)
 */
struct acpi_rsdp {
  char signature[8];
  uint8_t checksum;
  char oemid[6];
  uint8_t revision;
  uint32_t rsdt_addr;
  /* the following values are introduced since 2.0 */
  uint32_t length;
  uint64_t xsdt_addr;
  uint8_t extended_checksum;
  char reserved[3];
} __attribute__ ((packed));

/*
 * System Description Table (SDT) header
 */
struct acpi_sdt_hdr {
  char signature[4];
  uint32_t length;
  uint8_t revision;
  uint8_t checksum;
  char oemid[6];
  char oemtableid[8];
  uint32_t oemrevision;
  uint32_t creatorid;
  uint32_t creatorrevision;
} __attribute__ ((packed));

/*
 * APIC
 */
struct acpi_sdt_apic {
  uint32_t local_controller_addr;
  uint32_t flags;
} __attribute__ ((packed));

/*
 * APIC header
 */
struct acpi_sdt_apic_hdr {
  uint8_t type; /* 0 = local APIC, 1 = I/O APIC */
  uint8_t length;
} __attribute__ ((packed));

/*
 * Local APIC
 */
struct acpi_sdt_apic_lapic {
  struct acpi_sdt_apic_hdr hdr;
  uint8_t cpu_id;
  uint8_t apic_id;
  uint32_t flags;
} __attribute__ ((packed));

/*
 * I/O APIC
 */
struct acpi_sdt_apic_ioapic {
  struct acpi_sdt_apic_hdr hdr;
  uint8_t id;
  uint8_t reserved;
  uint32_t addr;
  uint32_t global_int_base;
} __attribute__ ((packed));

/*
 * Interrupt Source Override
 */
struct acpi_sdt_apic_int_src {
  struct acpi_sdt_apic_hdr hdr;
  uint8_t bus;
  uint8_t bus_int;
  uint32_t global_int;
  uint16_t mps_flags;
} __attribute__ ((packed));

/*
 * Generic address structure of ACPI
 */
struct acpi_generic_addr_struct {
  uint8_t addr_space;
  uint8_t bit_width;
  uint8_t bit_offset;
  uint8_t access_size;
  uint64_t addr;
} __attribute__ ((packed));

/*
 * FADT
 */
struct acpi_sdt_fadt {
  /* acpi_sdt_hdr */
  uint32_t firmware_ctrl;
  uint32_t dsdt;

  uint8_t reserved;

  uint8_t preferred_pm_profile;
  uint16_t sci_interrupt;
  uint32_t smi_cmd_port;
  uint8_t acpi_enable;
  uint8_t acpi_disable;
  uint8_t s4bios_req;
  uint8_t pstate_ctrl;
  uint32_t pm1a_event_block;
  uint32_t pm1b_event_block;
  uint32_t pm1a_ctrl_block;
  uint32_t pm1b_ctrl_block;
  uint32_t pm2_ctrl_block;
  uint32_t pm_timer_block;
  uint32_t gpe0_block;
  uint32_t gpe1_block;
  uint8_t pm1_event_length;
  uint8_t pm1_ctrl_length;
  uint8_t pm2_ctrl_length;
  uint8_t pm_timer_length;
  uint8_t gpe0_length;
  uint8_t gpe1_length;
  uint8_t gpe1_base;
  uint8_t cstate_ctrl;
  uint16_t worst_c2_latency;
  uint16_t worst_c3_latency;
  uint16_t flush_size;
  uint16_t flush_stride;
  uint8_t duty_offset;
  uint8_t duty_width;
  uint8_t day_alarm;
  uint8_t month_alarm;
  uint8_t century;

  uint16_t boot_arch_flags;

  uint8_t reserved2;
  uint32_t flags;

  struct acpi_generic_addr_struct reset_reg;
  uint8_t reset_value;

  uint8_t reserved3[3];

  uint64_t x_firmware_ctrl;
  uint64_t x_dsdt;

  struct acpi_generic_addr_struct x_pm1a_event_block;
  struct acpi_generic_addr_struct x_pm1b_event_block;
  struct acpi_generic_addr_struct x_pm1a_ctrl_block;
  struct acpi_generic_addr_struct x_pm1b_ctrl_block;
  struct acpi_generic_addr_struct x_pm2_ctrl_block;
  struct acpi_generic_addr_struct x_pm_timer_block;
  struct acpi_generic_addr_struct x_gpe0_block;
  struct acpi_generic_addr_struct x_gpe1_block;

} __attribute__ ((packed));

/*
 * SRAT
 * - acpi_sdt_hdr
 * - reserved[4+8]
 * - Static Resource Allocation Structure[n]
 */
struct acpi_sdt_srat_common {
  uint8_t type;
  uint8_t length;
} __attribute__ ((packed));
struct acpi_sdt_srat_lapic {
  uint8_t type;                    /* 0: Local APIC */
  uint8_t length;                  /* 16 */
  uint8_t proximity_domain;        /* Bit 7-0 */
  uint8_t apic_id;
  uint32_t flags;
  uint8_t local_sapic_eid;
  uint8_t proximity_domain2[3];    /* Bit 31-8 */
  uint32_t clock_domain;
} __attribute__ ((packed));
struct acpi_sdt_srat_memory {
  uint8_t type;                    /* 1: Memory */
  uint8_t length;                  /* 40 */
  uint32_t proximity_domain;
  uint8_t reserved1[2];
  uint32_t base_addr_low;
  uint32_t base_addr_high;
  uint32_t length_low;
  uint32_t length_high;
  uint32_t reserved2;
  uint32_t flags;
  uint8_t reserved3[8];
} __attribute__ ((packed));
struct acpi_sdt_srat_lapicx2 {
  uint8_t type;                    /* 2: Local x2APIC */
  uint8_t length;                  /* 24 */
  uint16_t reserved1;
  uint32_t proximity_domain;
  uint32_t x2apic_id;
  uint32_t flags;
  uint32_t clock_domain;
  uint32_t reserved2;
} __attribute__ ((packed));
struct acpi_sdt_srat_hdr {
  /* acpi_sdt_hdr */
  uint8_t reserved1[4];
  uint8_t reserved2[8];
  /* acpi_sdt_srat_*[n] */
} __attribute__ ((packed));


int acpi_load(acpi_t *);
int acpi_timer_available(acpi_t *);
uint32_t acpi_get_timer(acpi_t *);
uint64_t acpi_get_timer_period(acpi_t *);
void acpi_busy_usleep(acpi_t *, uint64_t);

#endif /* !__ASSEMBLY__ */

#endif /*	__MCUBE_X86_ACPI_H__ */

