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

#ifndef _ADVOS_KERNEL_ACPI_H
#define _ADVOS_KERNEL_ACPI_H

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

int acpi_load(acpi_t *);
int acpi_timer_available(acpi_t *);
uint32_t acpi_get_timer(acpi_t *);
uint64_t acpi_get_timer_period(acpi_t *);
void acpi_busy_usleep(acpi_t *, uint64_t);

#endif /* !__ASSEMBLY__ */

#endif
