/**
 * @file arch/x86/acpi.c
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

#include <mcube/mcube.h>


/*
 * Validate ACPI checksum: Since the ACPI checksum is a one-byte modular sum,
 * this function calculates the sum of len bytes from the memory space pointed
 * by ptr.  If the checksum is valid, this function returns zero.  Otherwise,
 * this returns a non-zero value.
 */
static int validate_checksum(const uint8_t *ptr, int len)
{
  uint8_t sum = 0;
  int i;

  for (i = 0; i < len; i++) {
    sum += ptr[i];
  }

  return sum;
}

/*
 * APIC
 *   0: Processor Local APIC
 *   1: I/O APIC
 *   2: Interrupt Source Override
 */
static int parse_apic(acpi_t *acpi, struct acpi_sdt_hdr *sdt)
{
  uint64_t addr;
  struct acpi_sdt_apic *apic;
  struct acpi_sdt_apic_hdr *hdr;
  struct acpi_sdt_apic_lapic *lapic;
  struct acpi_sdt_apic_ioapic *ioapic;
  uint32_t len;

  len = 0;
  addr = (uint64_t) sdt;
  len += sizeof(struct acpi_sdt_hdr);

  apic = (struct acpi_sdt_apic *)(addr + len);
  len += sizeof(struct acpi_sdt_apic);

  /* Local APIC */
  acpi->apic_address = apic->local_controller_addr;

  while (len < sdt->length) {
    hdr = (struct acpi_sdt_apic_hdr *)(addr + len);
    if (len + hdr->length > sdt->length) {
      /* Invalid */
      return -1;
    }
    switch (hdr->type) {
    case 0:
      /* Local APIC */
      lapic = (struct acpi_sdt_apic_lapic *)hdr;
      break;
    case 1:
      /* I/O APIC */
      ioapic = (struct acpi_sdt_apic_ioapic *)hdr;
      if (!acpi->ioapic_base) {
        /* First I/O APIC is used */
        acpi->ioapic_base = ioapic->addr;
      }
      break;
    case 2:
      /* Interrupt Source Override */
      break;
    default:
      /* Other */
      ;
    }
    len += hdr->length;
  }

  return 0;
}

/*
 * Parse Fixed ACPI Description Table (FADT)
 */
static int parse_fadt(acpi_t *acpi, struct acpi_sdt_hdr *sdt)
{
  uint64_t addr;
  struct acpi_sdt_fadt *fadt;
  uint32_t len;
  uint64_t dsdt;

  len = 0;
  addr = (uint64_t) sdt;
  len += sizeof(struct acpi_sdt_hdr);
  fadt = (struct acpi_sdt_fadt *)(addr + len);

  if (sdt->revision >= 3) {
    /* FADT revision 2.0 or higher */
    if (fadt->x_pm_timer_block.addr_space == 1) {
      /* Must be 1 (System I/O) */
      acpi->pm_tmr_port = fadt->x_pm_timer_block.addr;
      if (!acpi->pm_tmr_port) {
        acpi->pm_tmr_port = fadt->pm_timer_block;
      }
    }

    /* PM1a control block */
    if (fadt->x_pm1a_ctrl_block.addr_space == 1) {
      /* Must be 1 (System I/O) */
      acpi->pm1a_ctrl_block = fadt->x_pm1a_ctrl_block.addr;
      if (!acpi->pm1a_ctrl_block) {
        acpi->pm1a_ctrl_block = fadt->pm1a_ctrl_block;
      }
    }

    /* PM1b control block */
    if (fadt->x_pm1b_ctrl_block.addr_space == 1) {
      /* Must be 1 (System I/O) */
      acpi->pm1b_ctrl_block = fadt->x_pm1b_ctrl_block.addr;
      if (!acpi->pm1b_ctrl_block) {
        acpi->pm1b_ctrl_block = fadt->pm1b_ctrl_block;
      }
    }

    /* DSDT */
    dsdt = fadt->x_dsdt;
    if (!dsdt) {
      dsdt = fadt->dsdt;
    }
  } else {
    /* Revision < 3 */
    acpi->pm_tmr_port = fadt->pm_timer_block;

    /* PM1a control block  */
    acpi->pm1a_ctrl_block = fadt->pm1a_ctrl_block;

    /* PM1b control block  */
    acpi->pm1b_ctrl_block = fadt->pm1b_ctrl_block;

    /* DSDT */
    dsdt = fadt->dsdt;
  }

  /* Check flags: The eighth bit of fadt->flags presents the TMR_VAL_EXT flag.
     If this flag is clear, the counter of the timer is implemented as a
     24-bit value.  Otherwise, it is implemented as a 32-bit value. */
  acpi->pm_tmr_ext = (fadt->flags >> 8) & 0x1;

  /* SMI command */
  acpi->smi_cmd_port = fadt->smi_cmd_port;

  /* ACPI enable */
  acpi->acpi_enable = fadt->acpi_enable;

  /* Century */
  acpi->cmos_century = fadt->century;

  return 0;
}

/*
 * Parse ACPI Static Resource Affinity Table (SRAT)
 */
static int parse_srat(acpi_t *acpi, struct acpi_sdt_hdr *sdt)
{
  uint64_t addr;
  struct acpi_sdt_srat_common *srat;
  struct acpi_sdt_srat_lapic *srat_lapic;
  struct acpi_sdt_srat_memory *srat_memory;
  uint32_t len;
  uint64_t mbase;
  uint64_t mlen;

  len = 0;
  addr = (uint64_t) sdt;
  len += sizeof(struct acpi_sdt_hdr) + sizeof(struct acpi_sdt_srat_hdr);

  while (len < sdt->length) {
    srat = (struct acpi_sdt_srat_common *)(addr + len);
    if (len + srat->length > sdt->length) {
      /* Oversized */
      break;
    }
    switch (srat->type) {
    case 0:
      /* Local APIC */
      srat_lapic = (struct acpi_sdt_srat_lapic *) srat;
      acpi->lapic_domain[srat_lapic->apic_id].valid = 1;
      acpi->lapic_domain[srat_lapic->apic_id].domain = srat_lapic->proximity_domain
        | ((uint32_t)srat_lapic->proximity_domain2[0] << 8)
        | ((uint32_t)srat_lapic->proximity_domain2[1] << 16)
        | ((uint32_t)srat_lapic->proximity_domain2[2] << 24);
      break;
    case 1:
      /* Memory */
      srat_memory = (struct acpi_sdt_srat_memory *) srat;
      if ((srat_memory->flags & 1)
          && acpi->num_memory_region < MAX_MEMORY_REGIONS) {
        mbase = (uint64_t)srat_memory->base_addr_low
          |((uint64_t)srat_memory->base_addr_high << 32);
        mlen = (uint64_t)srat_memory->length_low
          | ((uint64_t)srat_memory->length_high << 32);
        acpi->memory_domain[acpi->num_memory_region].base = mbase;
        acpi->memory_domain[acpi->num_memory_region].length = mlen;
        acpi->memory_domain[acpi->num_memory_region].domain = srat_memory->proximity_domain;
        acpi->num_memory_region++;
      }
      break;
    default:
      /* Unknown */
      ;
    }

    /* Next entry */
    len += srat->length;
  }

  return 0;
}

/*
 * Parse Root System Description Table (RSDT/XSDT) in RSDP
 */
static int parse_rsdt(acpi_t *acpi, struct acpi_rsdp *rsdp)
{
  struct acpi_sdt_hdr *rsdt;
  int i;
  int nr;
  int sz;
  uint64_t addr;
  struct acpi_sdt_hdr *tmp;

  /* Check the ACPI version */
  if (rsdp->revision >= 1) {
    /* ACPI 2.0 or later */
    sz = 8;
    rsdt = (struct acpi_sdt_hdr *) rsdp->xsdt_addr;
    if (kmemcmp((uint8_t *) rsdt->signature, "XSDT", 4) != 0) {
      return -1;
    }
  } else {
    /* Parse RSDT (ACPI 1.x) */
    sz = 4;
    rsdt = (struct acpi_sdt_hdr *)(uintptr_t) rsdp->rsdt_addr;
    if (kmemcmp((uint8_t *)rsdt->signature, "RSDT", 4) != 0) {
      return -1;
    }
  }

  /* Compute the number of SDTs */
  nr = (rsdt->length - sizeof(struct acpi_sdt_hdr)) / sz;

  /* Check all SDTs */
  for (i = 0; i < nr; i++) {
    if (sz == 4) {
      addr = *(uint32_t *)((uintptr_t)(rsdt)
                           + sizeof(struct acpi_sdt_hdr) + i * sz);
    } else {
      addr = *(uint64_t *)((uintptr_t)(rsdt)
                           + sizeof(struct acpi_sdt_hdr) + i * sz);
    }
    tmp = (struct acpi_sdt_hdr *) addr;
    if (kmemcmp((uint8_t *) tmp->signature, "APIC", 4) == 0) {
      /* APIC */
      if (parse_apic(acpi, tmp) < 0) {
        return -1;
      }
    } else if (kmemcmp((uint8_t *)tmp->signature, "FACP", 4) == 0) {
      /* FADT */
      if (parse_fadt(acpi, tmp) < 0) {
        return -1;
      }
    } else if (kmemcmp((uint8_t *)tmp->signature, "SRAT ", 4) == 0) {
      /* SRAT */
      if (parse_srat(acpi, tmp) < 0) {
        return -1;
      }
    }
  }

  return 0;
}

/*
 * Search Root System Description Pointer (RSDP) in ACPI data structure
 */
static int rsdp_search_range(acpi_t *acpi, uintptr_t start, uintptr_t end)
{
  uintptr_t addr;
  struct acpi_rsdp *rsdp;

  for (addr = start; addr < end; addr += 0x10) {
    /* Check the checksum of the RSDP */
    if (validate_checksum((uint8_t *)addr, 20) == 0) {
      /* Checksum is correct, then check the signature. */
      rsdp = (struct acpi_rsdp *) addr;
      if (kmemcmp((uint8_t *) rsdp->signature, "RSD PTR ", 8) == 0) {
        /* This seems to be a valid RSDP, then parse RSDT. */
        return parse_rsdt(acpi, rsdp);
      }
    }
  }

  return -1;
}

/*
 * acpi_load -- load and parse ACPI from EBDA or main BIOS area
 */
int acpi_load(acpi_t *acpi)
{
  uint16_t ebda;
  uintptr_t ebda_addr;

  /* Reset the data structure */
  kmemset(acpi, 0, sizeof(acpi_t));

  /* Check 1KB of EBDA, first */
  ebda = *(uint16_t *) BDA_EDBA;
  if (ebda) {
    ebda_addr = (uintptr_t) ebda << 4;
    if (rsdp_search_range(acpi, ebda_addr, ebda_addr + 0x0400) >= 0) {
      return 0;
    }
  }

  /* If not found in the EDBA, check main BIOS area */
  return rsdp_search_range(acpi, 0xe0000, 0x100000);
}

/*
 * acpi_timer_available -- check the availability of the ACPI PM timer
 */
int acpi_timer_available(acpi_t *acpi)
{
  if (acpi->pm_tmr_port == 0) {
    return -1;              /* Not available */
  } else {
    return 0;               /* Available */
  }
}

/*
 * Get the current ACPI timer.  Note that the caller must check the
 * availability of the ACPI timer through the acpi_timer_available() function
 * before calling this function.
 */
uint32_t acpi_get_timer(acpi_t *acpi)
{
  return inl(acpi->pm_tmr_port);
}

/*
 * Get the timer period (wrapping)
 */
uint64_t acpi_get_timer_period(acpi_t *acpi)
{
  if (acpi->pm_tmr_ext) {
    /* 32-bit counter */
    return ((uint64_t) 1ULL << 32);
  } else {
    /* 24-bit counter */
    return (1 << 24);
  }
}

/*
 * Wait usec microseconds using ACPI timer.  Note that the caller must check
 * the availability of the ACPI timer through the acpi_timer_available()
 * function before calling this function.
 */
void acpi_busy_usleep(acpi_t *acpi, uint64_t usec)
{
  uint64_t clk;
  volatile uint64_t acc;
  volatile uint64_t cur;
  volatile uint64_t prev;

  /* usec to count */
  clk = (ACPI_TMR_HZ * usec) / 1000000;

  prev = acpi_get_timer(acpi);
  acc = 0;
  while (acc < clk) {
    cur = acpi_get_timer(acpi);
    if (cur < prev) {
      /* Overflow */
      acc += acpi_get_timer_period(acpi) + cur - prev;
    } else {
      acc += cur - prev;
    }
    prev = cur;
    pause();
  }
}
