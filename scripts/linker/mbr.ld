OUTPUT_FORMAT("binary","binary","binary");
OUTPUT_ARCH(i386:x86-64);
ENTRY(start)

SECTIONS
{
  . = 0x7c00;
  .text : { *(.text) }
  .data : { *(.data) }
}
