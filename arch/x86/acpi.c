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

/* BIOS data area (BDA): 0x0400--0x04ff; if extended BDA (EDBA) presents, its
   address >> 4 (16 bit) is stored in 0x040e. */
#define BDA_EDBA        0x040e

#define ACPI_TMR_HZ 3579545
#define ACPI_SCI_EN 0x1
#define ACPI_SLP_EN (1<<13)

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


/* Prototype declarations */
static int _validate_checksum(const uint8_t *, int);
static int _parse_apic(acpi_t *, struct acpi_sdt_hdr *);
static int _parse_fadt(acpi_t *, struct acpi_sdt_hdr *);
static int _parse_rsdt(acpi_t *, struct acpi_rsdp *);
static int _rsdp_search_range(acpi_t *, uintptr_t, uintptr_t);

/*
 * Validate ACPI checksum: Since the ACPI checksum is a one-byte modular sum,
 * this function calculates the sum of len bytes from the memory space pointed
 * by ptr.  If the checksum is valid, this function returns zero.  Otherwise,
 * this returns a non-zero value.
 */
static int
_validate_checksum(const uint8_t *ptr, int len)
{
    uint8_t sum = 0;
    int i;

    for ( i = 0; i < len; i++ ) {
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
static int
_parse_apic(acpi_t *acpi, struct acpi_sdt_hdr *sdt)
{
    uint64_t addr;
    struct acpi_sdt_apic *apic;
    struct acpi_sdt_apic_hdr *hdr;
    struct acpi_sdt_apic_lapic *lapic;
    struct acpi_sdt_apic_ioapic *ioapic;
    uint32_t len;

    len = 0;
    addr = (uint64_t)sdt;
    len += sizeof(struct acpi_sdt_hdr);

    apic = (struct acpi_sdt_apic *)(addr + len);
    len += sizeof(struct acpi_sdt_apic);

    /* Local APIC */
    acpi->apic_address = apic->local_controller_addr;

    while ( len < sdt->length ) {
        hdr = (struct acpi_sdt_apic_hdr *)(addr + len);
        if ( len + hdr->length > sdt->length ) {
            /* Invalid */
            return -1;
        }
        switch  ( hdr->type ) {
        case 0:
            /* Local APIC */
            lapic = (struct acpi_sdt_apic_lapic *)hdr;
            break;
        case 1:
            /* I/O APIC */
            ioapic = (struct acpi_sdt_apic_ioapic *)hdr;
            if ( !acpi->ioapic_base ) {
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
static int
_parse_fadt(acpi_t *acpi, struct acpi_sdt_hdr *sdt)
{
    uint64_t addr;
    struct acpi_sdt_fadt *fadt;
    uint32_t len;
    uint64_t dsdt;

    len = 0;
    addr = (uint64_t)sdt;
    len += sizeof(struct acpi_sdt_hdr);
    fadt = (struct acpi_sdt_fadt *)(addr + len);

    if ( sdt->revision >= 3 ) {
        /* FADT revision 2.0 or higher */
        if ( fadt->x_pm_timer_block.addr_space == 1 ) {
            /* Must be 1 (System I/O) */
            acpi->pm_tmr_port = fadt->x_pm_timer_block.addr;
            if ( !acpi->pm_tmr_port ) {
                acpi->pm_tmr_port = fadt->pm_timer_block;
            }
        }

        /* PM1a control block */
        if ( fadt->x_pm1a_ctrl_block.addr_space == 1 ) {
            /* Must be 1 (System I/O) */
            acpi->pm1a_ctrl_block = fadt->x_pm1a_ctrl_block.addr;
            if ( !acpi->pm1a_ctrl_block ) {
                acpi->pm1a_ctrl_block = fadt->pm1a_ctrl_block;
            }
        }

        /* PM1b control block */
        if ( fadt->x_pm1b_ctrl_block.addr_space == 1 ) {
            /* Must be 1 (System I/O) */
            acpi->pm1b_ctrl_block = fadt->x_pm1b_ctrl_block.addr;
            if ( !acpi->pm1b_ctrl_block ) {
                acpi->pm1b_ctrl_block = fadt->pm1b_ctrl_block;
            }
        }

        /* DSDT */
        dsdt = fadt->x_dsdt;
        if ( !dsdt ) {
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
static int
_parse_srat(acpi_t *acpi, struct acpi_sdt_hdr *sdt)
{
    uint64_t addr;
    struct acpi_sdt_srat_common *srat;
    struct acpi_sdt_srat_lapic *srat_lapic;
    struct acpi_sdt_srat_memory *srat_memory;
    uint32_t len;
    uint64_t mbase;
    uint64_t mlen;

    len = 0;
    addr = (uint64_t)sdt;
    len += sizeof(struct acpi_sdt_hdr) + sizeof(struct acpi_sdt_srat_hdr);

    while ( len < sdt->length ) {
        srat = (struct acpi_sdt_srat_common *)(addr + len);
        if ( len + srat->length > sdt->length ) {
            /* Oversized */
            break;
        }
        switch ( srat->type ) {
        case 0:
            /* Local APIC */
            srat_lapic = (struct acpi_sdt_srat_lapic *)srat;
            acpi->lapic_domain[srat_lapic->apic_id].valid = 1;
            acpi->lapic_domain[srat_lapic->apic_id].domain
                = srat_lapic->proximity_domain
                | ((uint32_t)srat_lapic->proximity_domain2[0] << 8)
                | ((uint32_t)srat_lapic->proximity_domain2[1] << 16)
                | ((uint32_t)srat_lapic->proximity_domain2[2] << 24);
            break;
        case 1:
            /* Memory */
            srat_memory = (struct acpi_sdt_srat_memory *)srat;
            if ( (srat_memory->flags & 1)
                 && acpi->num_memory_region < MAX_MEMORY_REGIONS ) {
                mbase = (uint64_t)srat_memory->base_addr_low
                    |((uint64_t)srat_memory->base_addr_high << 32);
                mlen = (uint64_t)srat_memory->length_low
                    | ((uint64_t)srat_memory->length_high << 32);
                acpi->memory_domain[acpi->num_memory_region].base = mbase;
                acpi->memory_domain[acpi->num_memory_region].length = mlen;
                acpi->memory_domain[acpi->num_memory_region].domain
                    = srat_memory->proximity_domain;
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
static int
_parse_rsdt(acpi_t *acpi, struct acpi_rsdp *rsdp)
{
    struct acpi_sdt_hdr *rsdt;
    int i;
    int nr;
    int sz;
    uint64_t addr;
    struct acpi_sdt_hdr *tmp;

    /* Check the ACPI version */
    if ( rsdp->revision >= 1 ) {
        /* ACPI 2.0 or later */
        sz = 8;
        rsdt = (struct acpi_sdt_hdr *)rsdp->xsdt_addr;
        if ( 0 != kmemcmp((uint8_t *)rsdt->signature, "XSDT", 4) ) {
            return -1;
        }
    } else {
        /* Parse RSDT (ACPI 1.x) */
        sz = 4;
        rsdt = (struct acpi_sdt_hdr *)(uintptr_t)rsdp->rsdt_addr;
        if ( 0 != kmemcmp((uint8_t *)rsdt->signature, "RSDT", 4) ) {
            return -1;
        }
    }

    /* Compute the number of SDTs */
    nr = (rsdt->length - sizeof(struct acpi_sdt_hdr)) / sz;

    /* Check all SDTs */
    for ( i = 0; i < nr; i++ ) {
        if ( 4 == sz ) {
            addr = *(uint32_t *)((uintptr_t)(rsdt)
                                 + sizeof(struct acpi_sdt_hdr) + i * sz);
        } else {
            addr = *(uint64_t *)((uintptr_t)(rsdt)
                                 + sizeof(struct acpi_sdt_hdr) + i * sz);
        }
        tmp = (struct acpi_sdt_hdr *)addr;
        if ( 0 == kmemcmp((uint8_t *)tmp->signature, "APIC", 4) ) {
            /* APIC */
            if ( _parse_apic(acpi, tmp) < 0 ) {
                return -1;
            }
        } else if ( 0 == kmemcmp((uint8_t *)tmp->signature, "FACP", 4) ) {
            /* FADT */
            if ( _parse_fadt(acpi, tmp) < 0 ) {
                return -1;
            }
        } else if ( 0 == kmemcmp((uint8_t *)tmp->signature, "SRAT ", 4) ) {
            /* SRAT */
            if ( _parse_srat(acpi, tmp) < 0 ) {
                return -1;
            }
        }
    }

    return 0;
}

/*
 * Search Root System Description Pointer (RSDP) in ACPI data structure
 */
static int
_rsdp_search_range(acpi_t *acpi, uintptr_t start, uintptr_t end)
{
    uintptr_t addr;
    struct acpi_rsdp *rsdp;

    for ( addr = start; addr < end; addr += 0x10 ) {
        /* Check the checksum of the RSDP */
        if ( 0 == _validate_checksum((uint8_t *)addr, 20) ) {
            /* Checksum is correct, then check the signature. */
            rsdp = (struct acpi_rsdp *)addr;
            if ( 0 == kmemcmp((uint8_t *)rsdp->signature, "RSD PTR ", 8) ) {
                /* This seems to be a valid RSDP, then parse RSDT. */
                return _parse_rsdt(acpi, rsdp);
            }
        }
    }

    return -1;
}

/*
 * acpi_load -- load and parse ACPI from EBDA or main BIOS area
 */
int
acpi_load(acpi_t *acpi)
{
    uint16_t ebda;
    uintptr_t ebda_addr;

    /* Reset the data structure */
    kmemset(acpi, 0, sizeof(acpi_t));

    /* Check 1KB of EBDA, first */
    ebda = *(uint16_t *)BDA_EDBA;
    if ( ebda ) {
        ebda_addr = (uintptr_t)ebda << 4;
        if ( _rsdp_search_range(acpi, ebda_addr, ebda_addr + 0x0400) >= 0 ) {
            return 0;
        }
    }

    /* If not found in the EDBA, check main BIOS area */
    return _rsdp_search_range(acpi, 0xe0000, 0x100000);
}

/*
 * acpi_timer_available -- check the availability of the ACPI PM timer
 */
int
acpi_timer_available(acpi_t *acpi)
{
    if ( 0 == acpi->pm_tmr_port ) {
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
uint32_t
acpi_get_timer(acpi_t *acpi)
{
    return inl(acpi->pm_tmr_port);
}

/*
 * Get the timer period (wrapping)
 */
uint64_t
acpi_get_timer_period(acpi_t *acpi)
{
    if ( acpi->pm_tmr_ext ) {
        /* 32-bit counter */
        return ((uint64_t)1ULL << 32);
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
void
acpi_busy_usleep(acpi_t *acpi, uint64_t usec)
{
    uint64_t clk;
    volatile uint64_t acc;
    volatile uint64_t cur;
    volatile uint64_t prev;

    /* usec to count */
    clk = (ACPI_TMR_HZ * usec) / 1000000;

    prev = acpi_get_timer(acpi);
    acc = 0;
    while ( acc < clk ) {
        cur = acpi_get_timer(acpi);
        if ( cur < prev ) {
            /* Overflow */
            acc += acpi_get_timer_period(acpi) + cur - prev;
        } else {
            acc += cur - prev;
        }
        prev = cur;
        pause();
    }
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
