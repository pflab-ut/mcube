OUTPUT_FORMAT("binary","binary","binary");
OUTPUT_ARCH(i386:x86-64);
ENTRY(bootmon)

SECTIONS
{
  . = 0x9000;
  .text : { *(.text) }
  .data : { *(.data) }
}
