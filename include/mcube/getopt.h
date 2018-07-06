/**
 * @file include/mcube/getopt.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_MCUBE_GETOPT_H__
#define	__MCUBE_MCUBE_GETOPT_H__

#ifndef __ASSEMBLY__

/**
 * @brief Option for kgetopt
 *
 * The option structure is used for kgetopt to maange options.
 */
struct option {
	/** Name. */
	const char *name;
	/** Does the option have argument? */
	int has_arg;
	/** Flag. */
	int *flag;
	/** Value. */
	int val;
};

typedef struct option option;

#define NR_ARG_OPTIONS 16

/**
 * @brief Argument options.
 */
enum arg_options {
	ARG_OPTIONS_DEBUG = 0,
	ARG_OPTIONS_EMIN,
	ARG_OPTIONS_HELP,
	ARG_OPTIONS_HNUM,
	ARG_OPTIONS_OFFSET,
	ARG_OPTIONS_OUMAX,
	ARG_OPTIONS_OUMIN,
	ARG_OPTIONS_NR_INTRA_KERNEL_CPUS,
	ARG_OPTIONS_TEND,
	ARG_OPTIONS_TSTART,
	ARG_OPTIONS_UMAX,
	ARG_OPTIONS_UMIN,
	ARG_OPTIONS_WSS,
};

extern struct option arg_option[NR_ARG_OPTIONS];

extern char *optarg;

/* Names for the values of the `has_arg' field of `struct option'.  */
#define no_argument 0
#define required_argument 1
#define optional_argument 2

int kgetopt(int argc, const char *argv[], const char *optstring);


extern int kgetopt_long(int argc, const char *argv[],
											 const char *optstring,
											 const struct option *longopts, int *longindex);

extern int kgetopt_long_only(int argc, const char *argv[],
														const char *optstring,
														const struct option *longopts, int *longindex);

#endif /* !__ASSEMBLY__ */

#endif	/* __MCUBE_MCUBE_GETOPT_H__ */
