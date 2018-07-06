/**
 * @file arch/sim/stdio.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>



ssize_t console_write(const void *input_data, size_t length, void *devdata)
{
	int i;
	for (i = 0; i < length; i++) {
		putchar(((char *) input_data)[i]);
	}
	return i;
}
