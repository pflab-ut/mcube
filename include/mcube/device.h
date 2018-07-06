/**
 * @file include/mcube/device.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_MCUBE_DEVICE_H__
#define	__MCUBE_MCUBE_DEVICE_H__

#ifndef __ASSEMBLY__

/** 
 * @brief Device operations
 *
 * The dev_ops structure is used to manage functions for device operations.
 */
struct dev_ops {
	/** Pointer to open function. */
	int (*open)(const char *, int, int);
	/** Pointer to read function. */
	ssize_t (*read)(void *, size_t, void *);
	/** Pointer to write function. */
	ssize_t (*write)(const void *, size_t, void *);
	/** Pointer to close function. */
	int (*close)(int);
	/** Pointer to ioctl function. */
	int (*ioctl)(int, unsigned long, void *);
};

typedef struct dev_ops dev_ops;

/**
 * @brief Device information
 *
 * The dev_struct structure has device information.
 */
struct dev_struct {
	/** Pointer to device operations. */
	struct dev_ops *ops;
	/** Pointer to device data. */
	void *devdata;
};

typedef struct dev_struct dev_struct;

#define	DEV_HASH_SZ	13

#if CONFIG_DEV_UART
#define	DEV_HASH_INIT	{													\
		{																						\
			.ops	= &uart_ops,												\
			.devdata	= &uart_data[STDIO_PORT],			\
		},																		\
	}
#else
#define DEV_HASH_INIT {{.ops = NULL, .devdata = NULL}}
#endif


/** 
 * @brief Device ID.
 */
enum dev_id {
	DEV_ID_PCI = 3,
	DEV_ID_IEEE1394,
	DEV_ID_USB,
	DEV_ID_ADDA,
	DEV_ID_PIO,
	DEV_ID_PLSCNT,
	DEV_ID_PWM,
	DEV_ID_UDC
};

typedef enum dev_id dev_id;


extern void setup_devhash(void);
extern int register_dev(int, struct dev_ops *, void *);
extern int unregister_dev(int);
extern ssize_t sys_read(int, void *, size_t);
extern ssize_t sys_write(int, void *, size_t);
extern int sys_ioctl(int, int, unsigned long);

#endif /* !__ASSEMBLY__ */

#endif	/* __MCUBE_MCUBE_DEVICE_H__ */
