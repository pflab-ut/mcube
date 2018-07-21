/*
 *            apic.h
 * written by Shinichi Awamoto, 2017
 * 
 * APIC関連の関数の宣言、構造体の定義
 */

#pragma once

#include "common.h"

#define MAX_CPU (16)

struct apic_descriptor;
void apic_init(struct apic_descriptor *madt);
void apic_enable_lapic();
void apic_send_eoi();
void apic_send_ipi(uint8_t destid, int vector);
void apic_start_other_processors();
void apic_initialize_ap();
