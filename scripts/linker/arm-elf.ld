ENTRY(_start)

SECTIONS
{
  /* Starts at LOADER_ADDR. 0x0 for bare metal and 0x80000 for qemu.  */
  . = 0x80000;
  /* . = 0x0; */
  __begin = .;
  __text_begin = .;
  .text :
  {
    KEEP(*(.text.boot))
    *(.text)
  }
  . = ALIGN(4096); /* align to page size */
  __text_end = .;

  __rodata_begin = .;
  .rodata :
  {
    *(.rodata)
  }
  . = ALIGN(4096); /* align to page size */
  __rodata_end = .;

  __data_begin = .;
  .data :
  {
    *(.data)
  }
  . = ALIGN(4096); /* align to page size */
  __data_end = .;

  __bss_begin = .;
  .bss :
  {
    bss = .;
    *(.bss)
  }

  . = ALIGN(4096); /* align to page size */
  __bss_end = .;
  __end = .;
}
