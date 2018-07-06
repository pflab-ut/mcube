/**
 * @file include/mcube/console.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_MCUBE_CONSOLE_H__
#define	__MCUBE_MCUBE_CONSOLE_H__

#ifndef __ASSEMBLY__

/**
 * @brief Console information
 *
 * The console_struct structure has console information to show CUI monitor.
 */
struct console_struct {
	/** column index. */
	int column;
	/** row index. */
	int row;
	/** x-coordinate of cursor. */
	int cursor_x;
	/** y-coordinate of cursor. */
	int cursor_y;
	/** command buffer. */
	char cmd[CMDBUF];
};

typedef struct console_struct console_struct;

extern console_struct console;


extern ssize_t console_write(const void *input_data, size_t length, void *devdata);
extern ssize_t console_read(void *input_data, size_t length, void *devdata);

extern void init_console(void);


/**
 * @brief Cursor information
 *
 * The cursor_struct structure has cursor information.
 */
struct cursor_struct {
	/** X-axis. */
	int x;
	/** Y-axis. */
	int y;
};

typedef struct coursor_struct cursor_struct;

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_CONSOLE_H__ */
