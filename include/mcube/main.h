/**
 * @file include/mcube/main.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_MCUBE_MAIN_H__
#define	__MCUBE_MCUBE_MAIN_H__


#define NR_THREADS 16

/* 4K */
#define KERNEL_STACK_SIZE 0x1000
#define USER_STACK_SIZE 0x1000


#define NR_PRIORITIES NR_THREADS

/* each bitmap length is 32 bit */
#define NR_PRIORITY_BITMAPS CEIL(NR_PRIORITIES, 32)

#ifndef	__ASSEMBLY__

#define ARRSIZE(x)           ((int)(sizeof(x) / sizeof(x[0])))

// Division macros, rounding (a/b) up or down
#define DIV_DN(a, b)         ((a) / (b))
#define DIV_UP(a, b)         (((a) + (b) - 1) / (b))

// Alignment macros, align a to the nearest multiple of b.
#define ALIGN_DN(a, b)       (div_dn(a, b) * (b))
#define ALIGN_UP(a, b)       (div_up(a, b) * (b))

#define PTR_ADD(t, p, o)     ((t *)((uint8_t *)(p) + (o)))
#define PTR_SUB(t, p, o)     ((t *)((uint8_t *)(p) - (o)))

#undef NULL
#define NULL ((void *) 0)

#undef EOF
#define EOF -1

#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif

#define FALSE 0
#define TRUE 1




#define SUCCESS 1
#define FAILURE 0

/// Forced structure packing (use only when absolutely necessary)
#define PACKSTRUCT           __attribute__((packed, aligned(1)))

/// Compile-time static assertion
#define STATIC_ASSERT(a, b)  _Static_assert(a, b)


#define __stringify_1(x...)   #x
#define __stringify(x...)    __stringify_1(x)

typedef int bool;


/**
 * @struct arg_parameter
 * @brief Argument parameter
 */
struct arg_parameter {
	/** Debug mode. */
	uint32_t debug;
	/** Minimum utilization. */
	uint32_t umin;
	/** Maximum utilization. */
	uint32_t umax;
	/** Beginning index of task set. */
	uint32_t tstart;
	/** End index of task set. */
	uint32_t tend;
	/** Minimum ratio of actual case execution time */
	uint32_t emin;
	/** Number of hyperperiods for simulation length. */
	uint32_t hnum;
	/** Number of processors. */
	uint32_t nr_cpus;
	/** Offset of system utilization. */
	uint32_t offset;
	/** Working set size. */
	uint32_t wss;
};

typedef struct arg_parameter arg_parameter;

extern struct arg_parameter arg_param;

#define NR_ARGS 0x20
#define ARGBUF 0x40

/**
 * @struct args
 * @brief Arguments
 */
struct args {
	/** Argv. */
	char argv[ARGBUF];
	/** Pointer to function adddress. */
	int (*func)(int argc, char *argv[NR_ARGS]);
	/** Argc. */
	uint32_t *argc;
	/** Error. */
	int err;
	/** Error message. */
	char *errstr;
};

typedef struct args args;


/**
 * @struct conv_flag
 * @brief Convert flag
 */
struct conv_flag {
	/** Pad. */
	short pad;
	/** Digital number. */
	short digit;
};

typedef struct conv_flag conv_flag;

int usermain(void);
int usertask(void *arg);

#define inf_loop() do {																									\
		print("%s:%s():%d %s\n", __FILE__, __func__, __LINE__, "inf_loop()"); \
		if (call_sys_get_mode_level() != USER_LEVEL) {                      \
      disable_local_irq();                                              \
    }                                                                   \
		for (;;)																														\
			;																																	\
  } while (0)


extern unsigned int Debug;

int main(int argc, char *argv[]);

void init_arch(void);
void init_arch_ap(void);
void exit_arch(void);
void exit_arch_ap(void);


#endif /* !__ASSEMBLY__ */

#endif	/* __MCUBE_MCUBE_MAIN_H__ */
