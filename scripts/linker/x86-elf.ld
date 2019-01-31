/* 
 * Kernel linker script
 *
 * Copyright (C) 2009-2010 Ahmed S. Darwish <darwish.07@gmail.com>
 *
 * Refer to official LD documentation "Using LD - the GNU linker"
 * for more information.
 *
 * Symbols prefixed by `__' are used by our C and assembly files;
 * modify them cautiously
 */

OUTPUT_FORMAT("elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)

ENTRY(startup_32)

SECTIONS
{
  /* Where the bootsector loads us */
  . = 0x100000;
 
  /* The global GDT is loaded in all CPU modes, including 16-
   * bit code where the GDT base addr must not exceed 3 bytes. */

  ASSERT(global_gdt >> 24 == 0,
         "Error: The 'global_gdt' must reside in a 3-byte address")


  /*
   * Bootstrap sections can't be merged with the rest
   * of the kernel as they're physical-addresses based
   */

  .text.head : {
        ./build/./arch/x86/boot/head.o(.text)
        ./build/./arch/x86/boot/rmcall.o(.text)
        ./build/./arch/x86/boot/e820.o(.text)
        ./build/./arch/x86/boot/load_ramdisk.o(.text)
  }

  .data.head : {
        ./build/./arch/x86/boot/head.o(.data)
        ./build/./arch/x86/boot/rmcall.o(.data)
        ./build/./arch/x86/boot/e820.o(.data)
        ./build/./arch/x86/boot/load_ramdisk.o(.data)
  }

  /* Putting the bootstrap BSS in the middle will force
   * the linker to reserve section size in the executable.
   * If you are interested in absolutely saving executable
   * size, you may like the trickery used in commit
   * cd618e752a which led to 0 executable bytes wasted,
   * with the tradeoff price of much extra complexity */
  .bss.head (NOLOAD) : {
        ./build/./arch/x86/boot/head.o(.bss)
        ./build/./arch/x86/boot/rmcall.o(.bss)
        ./build/./arch/x86/boot/e820.o(.bss)
        ./build/./arch/x86/boot/load_ramdisk.o(.bss)
  }

  /*
   * Use virtual addresses for the rest of the kernel
   */

  . += 0xffffffff80000000;

  .text : AT(ADDR(.text) - 0xffffffff80000000) {
         __text_start = .;
         *(EXCLUDE_FILE (*./build/./arch/x86/boot/head.o *./build/./arch/x86/boot/rmcall.o \
             *./build/./arch/x86/boot/e820.o *./build/./arch/x86/boot/load_ramdisk.o)
             .text)
         __text_end = .;
  }

  .data : {
         __data_start = .;
         *(EXCLUDE_FILE (*./build/./arch/x86/boot/head.o *./build/./arch/x86/boot/rmcall.o \
             *./build/./arch/x86/boot/e820.o *./build/./arch/x86/boot/load_ramdisk.o)
             .data)
         *(.rodata)
         __data_end = .;
        }

  .bss : {
    __bss_start = .;
         *(EXCLUDE_FILE (*./build/./arch/x86/boot/head.o *./build/./arch/x86/boot/rmcall.o \
             *./build/./arch/x86/boot/e820.o *./build/./arch/x86/boot/load_ramdisk.o)
             .bss)
         *(EXCLUDE_FILE (*./build/./arch/x86/boot/head.o *./build/./arch/x86/boot/rmcall.o \
             *./build/./arch/x86/boot/e820.o *./build/./arch/x86/boot/load_ramdisk.o)
             COMMON)
    __bss_end = .;
  }

  __kernel_end = .;

  /*
   * The ramdisk header is loaded directly after the kernel.
   */

  /DISCARD/ : {
    *(.eh_frame)
    *(.comment)
    *(.note)
  }
}
