/**
 * @file scripts/linker/dom-elf.ld
 *
 * @author Hiroyuki Chishiro
 */

ENTRY(__begin)
OUTPUT_FORMAT("elf32-bigdom", "elf32-bigdom", "elf32-littledom")
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

 /* bank size is 0x1000 and text and data regions do not share the same bank. */
	.rodata ALIGN(0x1000) : {
  __rodata_begin = .;
		*(.rdata)
		*(.rdata.*)
		*(.rodata)
		*(.rodata.*)
  __rodata_end = .;
	}

	.data : {
  __data_begin = .;
		*(.data)
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
