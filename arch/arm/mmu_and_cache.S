/**
 * @file arch/arm/mmu_and_cache.S
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

  // Put a 64-bit value with little endianness.
  .macro PUT_64B high, low
  .word \low
  .word \high
  .endm
  // Create an entry pointing to a next-level table.
  .macro TABLE_ENTRY PA, ATTR
  PUT_64B \ATTR, (\PA) + 0x3
  .endm
  // Create an entry for a 1GB block.
  .macro BLOCK_1GB PA, ATTR_HI, ATTR_LO
  PUT_64B \ATTR_HI, ((\PA) & 0xC0000000) | \ATTR_LO | 0x1
  .endm
  // Create an entry for a 2MB block.
  .macro BLOCK_2MB PA, ATTR_HI, ATTR_LO
  PUT_64B \ATTR_HI, ((\PA) & 0xFFE00000) | \ATTR_LO | 0x1
  .endm
  .align 12 // 12 for 4KB granule.
ttb0_base:
  TABLE_ENTRY level2_pagetable, 0
  BLOCK_1GB 0x40000000, 0, 0x740
  BLOCK_1GB 0x80000000, 0, 0x740
  BLOCK_1GB 0xC0000000, 0, 0x740
  .align 12 // 12 for 4KB granule.
level2_pagetable:
  .set ADDR, 0x000 // The current page address.
  .rept 0x200
  BLOCK_2MB (ADDR << 20), 0, 0x74C
  .set ADDR, ADDR+2
  .endr

  
ENTRY(configure_mmu_and_cache)  
clean_and_invalidate_cache:
disable_l1_cache: 
  mrs x0, sctlr_el3 // read sctlr_el3
  bic x0, x0, #(0x1 << 2) // disable D-cache.
  msr sctlr_el3, x0 // write sctlr_el3
  /*
   * invalidate Data cache to make the code general purpose.
   * Calculate the cache size first and loop through each set + way.
   */
  mov x0, #0x0 // x0 = cache level
  msr csselr_el1, x0 // 0x0 for L1 D-cache, 0x2 for L2 D-cache.
  
  mrs x4, ccsidr_el1 // read cache size ID.
  and x1, x4, #0x7
  and x1, x1, #0x4 // x1 = cache line size
  ldr x3, =0x7fff
  and x2, x3, x4, lsr #13 // x2 = cache set number - 1
  ldr x3, =0x3ff
  and x3, x3, x4, lsr #3 // x3 = cache associativity number - 1
  clz w4, w3 // x4 = way position in the CISW instruction.

  mov x5, #0 // x5 = way counter way_loop
way_loop:
  mov x6, #0 // x6 = set counter set_loop
set_loop:
  lsl x7, x5, x4
  orr x7, x0, x7 // set way.
  lsl x8, x6, x1
  orr x7, x7, x8
  dc cisw, x7 // clean and invalidate cache line.
  add x6, x6, #1 // increment set counter.
  cmp x6, x2 // last set reached yet?
  ble set_loop // If not, iterate set_loop
  add x5, x5, #1 // else, next way.
  cmp x5, x3 // last way reached yet?
  ble way_loop

setup_mmu:
  /* initialize translation table control registers. */
  ldr x1, =0x3520 // 4GB space 4KB granularity.
                  // Inner-shareable
  msr tcr_el3, x1 // normal inner and outer cacheable.
  ldr x1, =0xff440400 // ATTR0 Device-nGnRnE ATTR1 Device.
  msr mair_el3, x1 // ATTR2 Normal Non-Cacheable.
                   // ATTR3 Normal Cacheable.
  adr x0, ttb0_base // ttb0_Base must be a 4KB-aligned address.
  msr ttbr0_el3, x0
  
  /*
   * set up translation table entries in memory with looped store instructions.
   * Set the level 1 translation table.
   * The first entry points to level2_pagetable.
   */
  ldr x1, =level2_pagetable // Must be a 4KB align address.
  ldr x2, =0xfffff000
  and x2, x1, x2 // NSTable=0 APTable=0 XNTable=0 PXNTable=0.
  orr x2, x2, 0x3
  str x2, [x0], #8
  // The second entry is 1GB block from 0x40000000 to 0x7FFFFFFF.
  ldr x2, =0x40000741 // Executable Inner and Outer Shareable.
  str x2, [x0], #8 // R/W at all ELs secure memory
  // AttrIdx=000 Device-nGnRnE.
  // The third entry is 1GB block from 0x80000000 to 0xBFFFFFFF.
  ldr x2, =0x80000741
  str x2, [x0], #8
  // The fourth entry is 1GB block from 0xC0000000 to 0xFFFFFFFF.
  ldr x2, =0xc0000741
  str x2, [x0], #8
  // Set level 2 translation table.
  ldr x0, =level2_pagetable // Base address of level2_pagetable.
  ldr x2, =0x0000074d // Executable Inner and Outer Shareable.
  // R/W at all ELs secure memory.
  // AttrIdx=011 Normal Cacheable.
  mov x4, #512 // Set 512 level2 block entries.
  ldr x5, =0x00200000 // Increase 2MB address each time.
loop:
  str x2, [x0], #8 // Each entry occupies 2 words.
  add x2, x2, x5
  subs x4, x4, #1
  bne loop
  
enable_mmu_and_cache:
  // It is implemented in the CPUECTLR register.
  mrs x0, s3_1_c15_c2_1
  orr x0, x0, #(0x1 << 6) // The SMP bit.
  msr s3_1_c15_c2_1, x0
  // Enable caches and the MMU.
  mrs x0, sctlr_el3
  orr x0, x0, #(0x1 << 2) // The C bit (data cache).
  orr x0, x0, #(0x1 << 12) // The I bit (instruction cache).
  orr x0, x0, #0x1 // The M bit (MMU).
  msr sctlr_el3, x0
  dsb sy
  isb
  
  ret
  
