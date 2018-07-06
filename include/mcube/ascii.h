/**
 * @file include/mcube/ascii.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_MCUBE_ASCII_H__
#define	__MCUBE_MCUBE_ASCII_H__

#ifndef __ASSEMBLY__


/** 
 * @brief ASCII for controll key.
 */
enum ascii {
	NUL,
	CTRL_A,
	CTRL_B,
	CTRL_C,
	CTRL_D,
	CTRL_E,
	CTRL_F,
	CTRL_G,
	CTRL_H,
	CTRL_I,
	CTRL_J,
	CTRL_K,
	CTRL_L,
	CTRL_M,
	CTRL_N,
	CTRL_O,
	CTRL_P,
	CTRL_Q,
	CTRL_R,
	CTRL_S,
	CTRL_T,
	CTRL_U,
	CTRL_V,
	CTRL_W,
	CTRL_X,
	CTRL_Y,
	CTRL_Z,
	ESC,
	DEL = 127,
};

typedef enum ascii ascii;

static inline char num2ascii(char num)
{
	char ret;
	if (num >= 0 && num <= 9) {
		ret = num + '0';
	} else if (num >= 10 && num <= 15) {
		ret = num - 10 + 'a';
	} else {
		printk("Invalid Number %d\n", num);
		ret = -1;
	}
	return ret;
}

#endif /* !__ASSEMBLY__ */

#endif	/* __MCUBE_MCUBE_ASCII_H__ */
