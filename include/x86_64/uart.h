/**
 * @file include/x86_64/uart.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_64_UART_H__
#define __MCUBE_X86_64_UART_H__


#ifndef __ASSEMBLY__


/*
 * A 'baud rate' is the number of bits transferred over the serial link
 * per second. To convert this to number of bytes sent per second, the
 * baud rate is divided by the number of bits in the 'serial frame'.
 *
 * One of the most common serial frames used include a start bit, 8-bits
 * of data, no parity bit, and one stop bit, totalling 10-bits. So, an
 * 8/N/1 9600 baud rate = a transfer of 960 bytes per second.
 *
 * The UART has a built-in baud rate generator, which is set to match an
 * attached device. As in all hardware timers, that crystal generator
 * has a programmed divisor, ensuring the same baud rate over any system.
 *
 * Use max baud: this is only used in a virtual machine anyway.
 */
#define MAX_BAUD  115200
#define DESIRED_BAUD  MAX_BAUD

/*
 * UART register numbers
 *
 * All PCs are capable of operating up to four serial ports using
 * standard serial RS-232 I/O adapters. Their respective UART registers
 * are accessed through 8 _sequential_ I/O ports. Each port represents
 * an UART reg accessed through the three address lines A0, A1, and A2.
 *
 * The 'base' I/O address refers to the first I/O port in a sequential
 * group (representng a single UART). To access a UART register, the
 * base port is added to the UART 3-bit register number.
 */

/*
 * When the Divisor Latch Access Bit (DLAB) is set to zero, an
 * output to this reg stores a byte into the UART's TX buffer.
 */
#define UART_TRANSMIT_BUF  0

/*
 * When DLAB=1, an output to these registers holds the least
 * and highest order bytes of the baud rate divisor latch.
 */
#define UART_DIVISOR_LATCH_LOW  0
#define UART_DIVISOR_LATCH_HIGH  1

/*
 * When DLAB=0, a serial port can be configured to operate on
 * an interrupt basis using this reg. Nonetheless, we want to
 * use serial output in regions where interrupts are disabled,
 * thus the use of polling instead.
 */
#define UART_INTERRUPT_ENABLE  1

/*
 * When DLAB=0, this is used to provide a FIFO queueing
 * discipline, buffering up to 16-bytes of received data.
 */
#define UART_FIFO_CTRL    2

/*
 * Line control register, controls DLAB and send mode
 */
#define UART_LINE_CTRL    3

/**
 * @union line_control_reg
 * @brief Line control register.
 */
union line_control_reg {
  uint8_t raw;
  struct {
    uint8_t data_len: 2, /* # data bits in frame */
            stop_bit: 1, /* # stop bits in frame */
            parity_on: 1, /* Enable parity bit? */
            even_parity: 1, /* Even parity (if parity_on = 1) */
            sticky_parity: 1, /* Unused */
            break_ctrl: 1, /* Unused */
            DLAB: 1;   /* the DLAB control bit (see above) */
  } __packed;
};

enum {
  DATA_LEN_8 =  0x3,    /* 8 bytes in serial frame */
  DATA_LEN_7 =  0x2,    /* 7 bytes in serial frame */
  DATA_LEN_6 =  0x1,    /* 6 bytes in serial frame */
  DATA_LEN_5 =  0x0,    /* 5 bytes in serial frame */
};

enum {
  STOP_BIT_1 =  0x0,    /* 1 stop  bit in serial frame */
  STOP_BIT_0 =  0x1,    /* 2 stop bits in serial frame */
};

/*
 * Modem control register: 'Data Terminal Ready' informs the
 * attached device that _we_ are active and ready for commun-
 * ication. 'Request to Send' tells attached device that _we_
 * want to send data.
 */
#define UART_MODEM_CTRL    4

/**
 * @union modem_control_reg
 * @brief Modem control register.
 */
union modem_control_reg {
  uint8_t raw;
  struct {
    uint8_t dt_ready: 1, /* RS-232 Data Termainal Ready */
            req_send: 1, /* RS-232 Request To Send */
            unused0: 2, /* Unused */
            loopback: 1, /* Loopback between tx and rx */
            unused1: 3; /* Unused */
  } __packed;
};

/*
 * Status and error info relating to rx and tx.
 */
#define UART_LINE_STATUS  5

/**
 * @union line_status_reg
 * @brief Line status register.
 */
union line_status_reg {
  uint8_t raw;
  struct {
    uint8_t rx_avail: 1, /* Received data available */
            err_overrun: 1, /* A byte was lost (busy CPU) */
            err_parity: 1, /* Parity mismatch */
            err_frame: 1, /* frame corruption; noisy signal */
            rx_break: 1, /* If 1, a break signal received */
            tx_empty: 1, /* ALL tx buffers are empty */
            tx_has_byte: 1, /* holding reg or shift reg has a byte */
            unused: 1; /* Unused */
  } __packed;
};

/*
 * Modem Status: 'Data Set Ready' indicates that the remote
 * device is powered up and ready. 'Clear to send' informs
 * us that the attached device is ready for receiving data.
 */
#define UART_MODEM_STATUS  6

/**
 * @union modem_status_reg
 * @brief Modem status register.
 */
union modem_status_reg {
  uint8_t raw;
  struct {
    uint8_t unused0: 4, /* Unused */
            clr_to_send: 1, /* RS-232 Clear to Send */
            remote_ready: 1, /* RS-232 Data Set Ready */
            unused1: 2; /* Unused */
  } __packed;
};


/*
 * Port #1 base I/O address, from BIOS Data Area.
 * If the BDA returned a zero value, there's no active
 * serial port attached, and thus no data can be sent.
 */
#define BDA_COM1_ENTRY  0x400



#endif /* !__ASSEMBLY__ */



#endif /* __MCUBE_X86_64_UART_H__ */
