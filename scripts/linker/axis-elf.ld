/**
 * @file scripts/linker/axis-elf.ld
 *
 * @author Hiroyuki Chishiro
 */

ENTRY(__begin)
OUTPUT_FORMAT("elf32-bigmaxis", "elf32-bigmaxis", "elf32-littlemaxis")
SEARCH_DIR(.)

SECTIONS {
	.text 0x00000000 : {
  __text_begin = .;
		*(.init)
		*(.init.*)
		*(.text)
		*(.text.*)
  __text_end = .;
	}
 
	.data ALIGN(4) : {
  __data_begin = .;
		*(.data)
		*(.rdata)
		*(.rdata.*)
		*(.rodata)
		*(.rodata.*)
  __data_end = .;
	}
	_gp = .;
	.lit8 : {
		*(.lit8)
	}
	.lit4 : {
		*(.lit4)
	}

	.sdata : {
		*(.sdata)
	}
 
	.bss ALIGN(4) : {
		__bss_begin = .;
		*(.sbss)
		*(.scommon)
		*(.bss)
		*(COMMON)
		__bss_end = .;
	}

}
