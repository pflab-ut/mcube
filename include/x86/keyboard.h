/**
 * @file include/x86/keyboard.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_KEYBOARD_H__
#define __MCUBE_X86_KEYBOARD_H__

#ifndef __ASSEMBLY__

void __kb_handler(void);
void keyboard_init(void);

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_KEYBOARD_H__ */
