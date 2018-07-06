/**
 * @file include/mcube/fs/ram.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_MCUBE_FS_RAM_H__
#define __MCUBE_MCUBE_FS_RAM_H__


/* 512 byte/sector */
#define SECTOR_BITS 9
#define SECTOR_SIZE (0x1 << SECTOR_BITS)


/**
 * @brief RAM information
 *
 * The RAM_INFO structure has RAM information.
 */
struct RAM_INFO {
	/** Sectors. */
	uint32_t sectors;
	/** File system type. */
	FS_TYPE fs;
	/** Multi Media Card? */
	int is_mmc;
};

typedef struct RAM_INFO RAM_INFO;


/**
 * @brief RAM status information
 *
 * The RAM_STATUS structure has RAM status information.
 */
union RAM_STATUS {
	/** 16-bit access. */
	uint16_t ui16;
	/**
	 * @brief Two 8-bit unsigned int variables
	 *
	 * The ui8 structure has two 8-bit unsigned int variables.
	 */
	struct ui8 {
		/** Higher 8-bit access. */
		uint8_t ui8h:8;
		/** Lower 8-bit access. */
		uint8_t ui8l:8;
	};
	/**
	 * @brief Bit information
	 *
	 * The bits structure has bit information.
	 */
	struct bits {
		/** Start bit = always 0. */
		uint8_t start:1;
    /** Parameter error. */
		uint8_t pram_err:1;
    /* Address error. */
		uint8_t addr_err:1;
		/** Erase sequence error. */
		uint8_t erase_err:1;
		/** command CRC error. */
		uint8_t crc_err:1;
		/** Illegal command. */
		uint8_t bad_cmd:1;
		/** Erase reset. */
		uint8_t erace_rst:1;
		/** Idle state. */
		uint8_t idle:1;
		/** Out of range. */
		uint8_t out_range:1;
		/** Erace parameter. */
		uint8_t erase_parm:1;
		/** Write protection. */
		uint8_t wp_err:1;
		/** Card ECC error. */
		uint8_t ecc_err:1;
		/** Card controller error. */
		uint8_t ctr_err:1;
		/** Generic/Unknown error. */
		uint8_t unk_err:1;
    /** Write protection erase. */
		uint8_t wp_erase:1;
		/** Card is locked. */
		uint8_t locked:1;
	};
};

typedef union RAM_STATUS RAM_STATUS;

extern IO_ERROR ram_init(void);
extern IO_ERROR ram_write_sector(uint32_t addr, const uint8_t *buf);
extern IO_ERROR ram_read_sector(uint32_t addr, uint8_t *buf);
extern const RAM_INFO *ram_get_info(void);
extern uint16_t  ram_get_status(void);

#endif /* __MCUBE_MCUBE_FS_RAM_H__ */
