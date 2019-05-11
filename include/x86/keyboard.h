/**
 * @file include/x86/keyboard.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_KEYBOARD_H__
#define __MCUBE_X86_KEYBOARD_H__

#ifndef __ASSEMBLY__

void keyboard_init(void);

void kb_handler(void);

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_KEYBOARD_H__ */
