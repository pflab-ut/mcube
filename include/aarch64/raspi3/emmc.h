/**
 * @file include/aarch64/raspi3/emmc.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AARCH64_RASPI3_EMMC_H__
#define __MCUBE_AARCH64_RASPI3_EMMC_H__

/**
 * @def EMMC_ARG2
 * @brief EMMC AMCD23 argument.
 */
#define EMMC_ARG2 (EMMC_BASE + 0x00)

/**
 * @def EMMC_BLKSIZECNT
 * @brief EMMC block size and count.
 */
#define EMMC_BLKSIZECNT (EMMC_BASE + 0x04)

/**
 * @def EMMC_ARG1
 * @brief EMMC argument.
 */
#define EMMC_ARG1 (EMMC_BASE + 0x08)

/**
 * @def EMMC_CMDTM
 * @brief EMMC command and transfer mode.
 */
#define EMMC_CMDTM (EMMC_BASE + 0x0c)

/**
 * @def EMMC_RESP0
 * @brief EMMC response bits [31-0].
 */
#define EMMC_RESP0 (EMMC_BASE + 0x10)

/**
 * @def EMMC_RESP1
 * @brief EMMC response bits [63-32].
 */
#define EMMC_RESP1 (EMMC_BASE + 0x14)

/**
 * @def EMMC_RESP2
 * @brief EMMC response bits [95-64].
 */
#define EMMC_RESP2 (EMMC_BASE + 0x18)

/**
 * @def EMMC_RESP3
 * @brief EMMC response bits [127-96].
 */
#define EMMC_RESP3 (EMMC_BASE + 0x1c)

/**
 * @def EMMC_DATA
 * @brief EMMC data.
 */
#define EMMC_DATA (EMMC_BASE + 0x20)

/**
 * @def EMMC_STATUS
 * @brief EMMC status.
 */
#define EMMC_STATUS (EMMC_BASE + 0x24)

/**
 * @def EMMC_CONTROL0
 * @brief EMMC host configuration bits.
 */
#define EMMC_CONTROL0 (EMMC_BASE + 0x28)

/**
 * @def EMMC_CONTROL1
 * @brief EMMC host configuration bits.
 */
#define EMMC_CONTROL1 (EMMC_BASE + 0x2c)

/**
 * @def EMMC_INTERRUPT
 * @brief EMMC interrupt flags.
 */
#define EMMC_INTERRUPT (EMMC_BASE + 0x30)

/**
 * @def EMMC_INT_MASK
 * @brief EMMC interrupt flag enable.
 */
#define EMMC_INT_MASK (EMMC_BASE + 0x34)

/**
 * @def EMMC_INT_EN
 * @brief EMMC interrupt generation enable.
 */
#define EMMC_INT_EN (EMMC_BASE + 0x38)

/**
 * @def EMMC_CONTROL2
 * @brief EMMC host configuration bits.
 */
#define EMMC_CONTROL2 (EMMC_BASE + 0x3c)

/**
 * @def EMMC_FORCE_INT
 * @brief EMMC force interrupt event.
 */
#define EMMC_FORCE_INT (EMMC_BASE + 0x50)

/**
 * @def EMMC_BOOT_TIMEOUT
 * @brief EMMC timeout in boot mode.
 */
#define EMMC_BOOT_TIMEOUT (EMMC_BASE + 0x70)

/**
 * @def EMMC_DBG_SEL
 * @brief EMMC debug bus configuration.
 */
#define EMMC_DBG_SEL (EMMC_BASE + 0x74)

/**
 * @def EMMC_EXFIFO_CONFIG
 * @brief EMMC extension FIFO configuration.
 */
#define EMMC_EXFIFO_CONFIG (EMMC_BASE + 0x80)

/**
 * @def EMMC_EXFIFO_ENABLE
 * @brief EMMC extension FIFO enable.
 */
#define EMMC_EXFIFO_ENABLE (EMMC_BASE + 0x84)

/**
 * @def EMMC_TUNE_STEP
 * @brief Delay per card clock tuning step.
 */
#define EMMC_TUNE_STEP (EMMC_BASE + 0x88)

/**
 * @def EMMC_TUNE_STEPS_STD
 * @brief EMMC card clock tuning steps for STD.
 */
#define EMMC_TUNE_STEPS_STD (EMMC_BASE + 0x8c)

/**
 * @def EMMC_TUNE_STEPS_DDR
 * @brief EMMC card clock tuning steps for DDR.
 */
#define EMMC_TUNE_STEPS_DDR (EMMC_BASE + 0x90)

/**
 * @def EMMC_SPI_INT_SPT
 * @brief EMMC SPI interrupt support.
 */
#define EMMC_SPI_INT_SPT (EMMC_BASE + 0xf0)

/**
 * @def EMMC_SLOTISR_VER
 * @brief EMMC slot interrupt status and version.
 */
#define EMMC_SLOTISR_VER (EMMC_BASE + 0xfc)

/* command flags */

/**
 * @def CMD_NEED_APP
 * @brief Command need applications.
 */
#define CMD_NEED_APP 0x80000000

/**
 * @def CMD_RSPNS_48
 * @brief Command RSPNS 48.
 */
#define CMD_RSPNS_48 0x00020000

/**
 * @def CMD_ERRORS_MASK
 * @brief Command errors mask.
 */
#define CMD_ERRORS_MASK 0xfff9c004

/**
 * @def CMD_RCA_MASK
 * @brief Command RCA mask.
 */
#define CMD_RCA_MASK 0xffff0000


/* COMMANDs */
/**
 * @def CMD_GO_IDLE
 * @brief Command go idle.
 */
#define CMD_GO_IDLE 0x00000000

/**
 * @def CMD_ALL_SEND_CID
 * @brief Command all send CID.
 */
#define CMD_ALL_SEND_CID 0x02010000

/**
 * @def CMD_SEND_REL_ADDR
 * @brief Command send rel address.
 */
#define CMD_SEND_REL_ADDR 0x03020000

/**
 * @def CMD_CARD_SELECT
 * @brief Command card select.
 */
#define CMD_CARD_SELECT 0x07030000

/**
 * @def CMD_SEND_IF_COND
 * @brief Command send if condition holds.
 */
#define CMD_SEND_IF_COND 0x08020000

/**
 * @def CMD_STOP_TRANS
 * @brief Command stop transmissions.
 */
#define CMD_STOP_TRANS 0x0c030000

/**
 * @def CMD_READ_SINGLE
 * @brief Command read single.
 */
#define CMD_READ_SINGLE 0x11220010

/**
 * @def CMD_READ_MULTI
 * @brief Command read multi.
 */
#define CMD_READ_MULTI 0x12220032

/**
 * @def CMD_SET_BLOCKCNT
 * @brief Command set blockcount.
 */
#define CMD_SET_BLOCKCNT 0x17020000

/**
 * @def CMD_WRITE_SINGLE
 * @brief Command write single.
 */
#define CMD_WRITE_SINGLE 0x18220010

/**
 * @def CMD_WRITE_MULTI
 * @brief Command write multi.
 */
#define CMD_WRITE_MULTI 0x19220032

/**
 * @def CMD_APP_CMD
 * @brief Command application command.
 */
#define CMD_APP_CMD 0x37000000

/**
 * @def CMD_SET_BUS_WIDTH
 * @brief Command set bus width.
 */
#define CMD_SET_BUS_WIDTH (0x06020000 | CMD_NEED_APP)

/**
 * @def CMD_SEND_OP_COND
 * @brief Command send operation command.
 */
#define CMD_SEND_OP_COND    (0x29020000 | CMD_NEED_APP)

/**
 * @def CMD_SEND_SCR
 * @brief Command send scr.
 */
#define CMD_SEND_SCR (0x33220010 | CMD_NEED_APP)

/* STATUS register settings. */

/**
 * @def SR_READ_AVAILABLE
 * @brief SR read available.
 */
#define SR_READ_AVAILABLE 0x00000800

/**
 * @def SR_WRITE_AVAILABLE
 * @brief SR write available.
 */
#define SR_WRITE_AVAILABLE 0x00000400

/**
 * @def SR_DAT_INHIBIT
 * @brief SR data inhibit.
 */
#define SR_DAT_INHIBIT 0x00000002

/**
 * @def SR_CMD_INHIBIT
 * @brief SR command inhibit.
 */
#define SR_CMD_INHIBIT 0x00000001

/**
 * @def SR_APP_CMD
 * @brief SR application command.
 */
#define SR_APP_CMD 0x00000020

/* INTERRUPT register settings */

/**
 * @def INT_DATA_TIMEOUT
 * @brief Interrupt data timeout.
 */
#define INT_DATA_TIMEOUT 0x00100000

/**
 * @def INT_CMD_TIMEOUT
 * @brief Interrupt command timeout.
 */
#define INT_CMD_TIMEOUT 0x00010000

/**
 * @def INT_READ_RDY
 * @brief Interrupt read ready.
 */
#define INT_READ_RDY 0x00000020

/**
 * @def INT_WRITE_RDY
 * @brief Interrupt write ready.
 */
#define INT_WRITE_RDY 0x00000010

/**
 * @def INT_DATA_DONE
 * @brief Interrupt data done.
 */
#define INT_DATA_DONE 0x00000002

/**
 * @def INT_CMD_DONE
 * @brief Interrupt command done.
 */
#define INT_CMD_DONE 0x00000001

/**
 * @def INT_ERROR_MASK
 * @brief Interrupt error mask.
 */
#define INT_ERROR_MASK 0x017e8000

/* CONTROL register settings */

/* 31-23: reserved */

/**
 * @def C0_ALT_BOOT_EN
 * @brief Enable alternate boot mode access:
 * 0 = disabled.
 * 1 = enabled.
 */
#define C0_ALT_BOOT_EN (0x1 << 21)

/**
 * @def C0_SPI_MODE_EN
 * @brief 20: SPI mode enable:
 * 0 = normal mode.
 * 1 = SPI mode.
 */
#define C0_SPI_MODE_EN (0x1 << 20)

/**
 * @def C0_GAP_IEN
 * @brief 19: Enable SDIO interrupt at block gap (only valid if the HCTL_DWIDTH bit is set):
 * 0 = disabled.
 * 1 = enabled.
 */
#define C0_GAP_IEN (0x1 << 19)

/**
 * @def C0_READWAIT_EN
 * @brief 18: Use DAT2 read-wait protocol for SDIO cards supporting this:
 * 0 = disabled.
 * 1 = enabled.
 */
#define C0_READWAIT_EN (0x1 << 18)

/**
 * @def C0_GAP_RESTART
 * @brief 17: Rest.art a transaction which was stopped using the GAP_STOP bit:
 * 0 = ignore.
 * 1 = restart.
 */
#define C0_GAP_RESTART (0x1 << 17)

/**
 * @def C0_GAP_STOP
 * @brief 16: Stop the current transaction at the next block gap:
 * 0 = ignore.
 * 1 = stop.
 */
#define C0_GAP_STOP (0x1 << 16)

/* 15-6: reserved */

/**
 * @def C0_HCTL_8BIT
 * @brief 5: Use 8 data lines:
 * 0 = disabled.
 * 1 = enabled.
 */
#define C0_HCTL_8BIT (0x1 << 5)

/* 4-3: reserved */

/**
 * @def C0_HCTL_HS_EN
 * @brief 2: select high speed mode (i.e. DAT and CMD lines change on the rising CLK edge):
 * 0 = disabled.
 * 1 = enabled.
 */
#define C0_HCTL_HS_EN (0x1 << 2)

/**
 * @def C0_HCTL_DWIDTH
 * @brief 1: Use 4 data lines:
 * 0 = disabled.
 * 1 = enabled.
 */
#define C0_HCTL_DWIDTH (0x1 << 1)

/* 0: reserved */

/* 31-27: reserved */

/**
 * @def C1_SRST_DATA
 * @brief 26: Reset the data handling circuit:
 * 0 = disabled.
 * 1 = enabled.
 */
#define C1_SRST_DATA (0x1 << 26)

/**
 * @def C1_SRST_CMD
 * @brief 25: Reset the command handling circuit:
 * 0 = disabled.
 * 1 = enabled.
 */
#define C1_SRST_CMD (0x1 << 25)

/**
 * @def C1_SRST_HC
 * @brief 24: Reset the complete host circuit:
 * 0 = disabled.
 * 1 = enabled.
 */
#define C1_SRST_HC (0x1 << 24)

/* 23-20: reserved */

/**
 * @def C1_DATA_TOUNIT
 * @brief Data timeout unit exponent:
 * 0b1111 = disabled.
 * x = TMCLK * 2^(x+13).
 */
#define C1_DATA_TOUNIT (0xf << 16)

/**
 * @def C1_DATA_TOUNIT_MAX
 * @brief Data timeout unit maximum.
 */
#define C1_DATA_TOUNIT_MAX (0xe << 16)

/**
 * @def C1_CLK_FREQ8
 * @brief SD clock base divider LSBs.
 */
#define C1_CLK_FREQ8 (0xff << 8)

/**
 * @def C1_CLK_FREQ_MS2
 * @brief SD clock base divider MSBs.
 */
#define C1_CLK_FREQ_MS2 (0x3 << 6)


/**
 * @def C1_CLK_GENSEL
 * @brief Mode of clock generation:
 * 0 = divided.
 * 1 = programmable.
 */
#define C1_CLK_GENSEL (0x1 << 5)

/* 4-3: reserved */

/**
 * @def C1_CLK_EN
 * @brief SD clock enable:
 * 0 = disabled.
 * 1 = enabled.
 */
#define C1_CLK_EN (0x1 << 2)

/**
 * @def C1_CLK_STABLE
 * @brief 1: SD clock stable:
 * 0 = no.
 * 1 = yes.
 */
#define C1_CLK_STABLE (0x1 << 1)

/**
 * @def C1_CLK_INTLEN
 * @brief 0: Clock enable for internal EMMC clocks for power saving:
 * 0 = disabled.
 * 1 = enabled.
 */
#define C1_CLK_INTLEN (0x1 << 0)


/* SLOTISR_VER values */

/**
 * @def SLOTISR_VER_VENDOR
 * @brief 31-24: Vendor version number.
 */
#define SLOTISR_VER_VENDOR (0xff << 24)

/**
 * @def SLOTISR_VER_SDVERSION_SHIFT
 * @brief 23-16: Host specification number shift.
 */
#define SLOTISR_VER_SDVERSION_SHIFT 16

/**
 * @def SLOTISR_VER_SDVERSION
 * @brief Host controller specification number.
 */
#define SLOTISR_VER_SDVERSION (0xff << SLOTISR_VER_SDVERSION_SHIFT)


/**
 * @def SLOTISR_VER_SDVERSION_HOST_SPEC_V3
 * @brief Host specfication version 3.
 */
#define SLOTISR_VER_SDVERSION_HOST_SPEC_V3 2

/**
 * @def SLOTISR_VER_SDVERSION_HOST_SPEC_V2
 * @brief Host specification version 2.
 */
#define SLOTISR_VER_SDVERSION_HOST_SPEC_V2 1

/**
 * @def SLOTISR_VER_SDVERSION_HOST_SPEC_V1
 * @brief Host specification version 1.
 */
#define SLOTISR_VER_SDVERSION_HOST_SPEC_V1 0

/* 15-8: reserved */

/**
 * @def SLOTISR_VER_SLOT_STATUS
 * @brief Logical OR of interrupt and wakeup signal for each slot.
 */
#define SLOTISR_VER_SLOT_STATUS 0xff


/* SCR flags */

/**
 * @def SCR_SD_BUS_WIDTH_4
 * @brief SCR SD bus width = 4.
 */
#define SCR_SD_BUS_WIDTH_4 0x00000400

/**
 * @def SCR_SUPP_SET_BLKCNT
 * @brief SCR supp set block count.
 */
#define SCR_SUPP_SET_BLKCNT 0x02000000

/* added by my driver */
/**
 * @def SCR_SUPP_CCS
 * @brief SCR SUPP card capacity status.
 */
#define SCR_SUPP_CCS 0x00000001

/**
 * @def ACMD41_VOLTAGE
 * @brief ACMD41 voltage.
 */
#define ACMD41_VOLTAGE 0x00ff8000

/**
 * @def ACMD41_CMD_COMPLETE
 * @brief AMCD41 command complete.
 */
#define ACMD41_CMD_COMPLETE 0x80000000

/**
 * @def ACMD41_CMD_CCS
 * @brief ACMD41 command card capacity status.
 */
#define ACMD41_CMD_CCS 0x40000000

/**
 * @def ACMD41_ARG_HC
 * @brief AMCD41 argument HC.
 */
#define ACMD41_ARG_HC 0x51ff8000


/**
 * @def SD_OK
 * @brief SD OK.
 */
#define SD_OK 0

/**
 * @def SD_TIMEOUT
 * @brief SD timeout.
 */
#define SD_TIMEOUT 1

/**
 * @def SD_ERROR
 * @brief SD error.
 */
#define SD_ERROR 2


#ifndef __ASSEMBLY__

/**
 * @fn int sd_status(unsigned int mask)
 * @brief wait for data or command ready.
 *
 * @param mask Mask.
 * @return Zero if success, and nonzero if failure.
 */
int sd_status(unsigned int mask);

/**
 * @fn int sd_int(unsigned int mask)
 * @brief wait for interrupt.
 *
 * @param mask Mask.
 * @return Zero if success, and nonzero if failure.
 */
int sd_int(unsigned int mask);

/**
 * @fn int sd_cmd(unsigned int code, unsigned int arg)
 * @brief send a command.
 *
 * @param code Code.
 * @param arg Argument.
 * @return Nonzero if success, and zero if failure.
 */
int sd_cmd(unsigned int code, unsigned int arg);

/**
 * @fn int sd_readblock(unsigned int lba, unsigned char *buffer, unsigned int num)
 * @brief read a block from sd card.
 *
 * @param lba Logical block address.
 * @param buffer Buffer.
 * @param num Number.
 * @return Number of bytes read.
 */
int sd_readblock(unsigned int lba, unsigned char *buffer, unsigned int num);

/**
 * @fn int sd_writeblock(unsigned char *buffer, unsigned int lba, unsigned int num)
 * @brief write a block to the sd card.
 *
 * @param buffer Buffer.
 * @param lba Logical block address.
 * @param num Number.
 * @return Number of bytes written.
 */
int sd_writeblock(unsigned char *buffer, unsigned int lba, unsigned int num);

/**
 * @fn int sd_clk(unsigned int f)
 * @brief set SD clock to frequency in Hz.
 *
 * @param f Frequency.
 * @return Zero if success, and nonzero if failure.
 */
int sd_clk(unsigned int f);

/**
 * @fn int init_sd(void)
 * @brief initialize EMMC to read SDHC card.
 *
 * @return Zero if success, and nonzero if failure.
 */
int init_sd(void);


#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_AARCH64_RASPI3_EMMC_H__ */

