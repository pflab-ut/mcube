/**
 * @file include/mcube/shell.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_MCUBE_SHELL_H__
#define	__MCUBE_MCUBE_SHELL_H__


#ifndef __ASSEMBLY__

#define CMDBUF 0x100
#define NR_ARGS 0x20
#define ARGBUF 0x40

#define MAX_CMDS 0x100

/**
 * @brief Argument parameter
 *
 * The arg_parameter structure has argument parameter information.
 */
struct arg_parameter {
	/** Debug mode. */
	uint32_t debug;
	/** Minimum utilization of optional parts. */
	uint32_t oumin;
	/** Maximum utilization of optional parts. */
	uint32_t oumax;
	/** Minimum utilization of mandatory parts. */
	uint32_t umin;
	/** Maximum utilization of mandatory parts. */
	uint32_t umax;
	/** Start index of task set. */
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


/**
 * @brief Arguments
 *
 * The args structure has arguments from command line.
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

#define NR_HISTORIES 0x10

#define backspace() do {   \
    putchar('\b');        \
    putchar(' ');         \
    putchar('\b');        \
  } while (0)


/**
 * @brief History
 *
 * The hstry structure has histroy of command line.
 */
struct hstry {
	/** Command buffer. */
	char cmd[NR_HISTORIES][CMDBUF];
	/** Current history. */
	int current;
	/** Number of histories. */
	int num;
};

typedef struct hstry hstry;

extern hstry hst;

/**
 * @brief Escape
 *
 * The esc structure has escape key.
 */
struct esc {
	/** Name. */
  char name[CMDBUF];
	/** Pointer to function. */
  int (*func)(char cmdbuf[CMDBUF], char cmdbak[CMDBUF], int *curcmdp, int *cursorp, int *cmdlenp, ascii code);
};

typedef struct esc esc;

extern void mv2right(char cmdbuf[CMDBUF], int *cursorp, int *cmdlenp, ascii code);
extern void mv2left(int *cursorp);


extern int uparrow(char cmdbuf[CMDBUF], char cmdbak[CMDBUF], int *curcmdp, int *cursorp, int *cmdlenp, ascii code);
extern int downarrow(char cmdbuf[CMDBUF], char cmdbak[CMDBUF], int *curcmdp, int *cursorp, int *cmdlenp, ascii code);
extern int rightarrow(char cmdbuf[CMDBUF], char cmdbak[CMDBUF], int *curcmdp, int *cursorp, int *cmdlenp, ascii code);
extern int leftarrow(char cmdbuf[CMDBUF], char cmdbak[CMDBUF], int *curcmdp, int *cursorp, int *cmdlenp, ascii code);


extern void do_shell(void);


#define USERNAME "chishiro"
#define HOSTNAME "est"

#define PRINT_SHELL  USERNAME "@" HOSTNAME "[~]"

#define print_shell() printk("%s", PRINT_SHELL);


extern int slen;
extern int curcmd;

extern int kargc;
extern char *kargv[NR_ARGS];
extern char kargv_buf[NR_ARGS][ARGBUF];

extern char cmdbak[CMDBUF];

extern void do_esc(char cmdbuf[CMDBUF], char cmdbak[CMDBUF], int *curcmdp, int *cursorp, int *cmdlenp, ascii code);

extern void do_tab_completion(void);


extern void save_cmd(int argc, char *argv[NR_ARGS]);
extern void do_cmd(int argc, char *argv[NR_ARGS]);
extern void clear_cmd(char cmdbuf[CMDBUF], int *cursorp, int *cmdlenp);
extern void print_cmd(char cmdbuf[CMDBUF], int *cursorp, int *cmdlenp);
extern void print_all_cmd(void);
extern void print_prev_cmd(char cmdbuf[CMDBUF], char cmdbak[CMDBUF],
                   int *curcmdp, int *cursorp, int *cmdlenp);
extern void print_next_cmd(char cmdbuf[CMDBUF], char cmdbak[CMDBUF],
                    int *curcmdp, int *cursorp, int *cmdlenp);
extern void split_arg(char cmdbuf[CMDBUF]);
extern void init_cmd(void);
extern void init_shell(void);


/* command list */

extern int cexit(int argc, char *argv[NR_ARGS]);
extern int history(int argc, char *argv[NR_ARGS]);
extern int led(int argc, char *argv[NR_ARGS]);

#endif /* !__ASSEMBLY__ */

#endif	/* __MCUBE_MCUBE_SHELL_H__ */
