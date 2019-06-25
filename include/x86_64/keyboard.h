/**
 * @file include/x86_64/keyboard.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_64_KEYBOARD_H__
#define __MCUBE_X86_64_KEYBOARD_H__

#ifndef __ASSEMBLY__

/**
 * @fn void keyboard_init(void)
 * @brief initialize keyboard.
 */
void keyboard_init(void);

/**
 * @fn void __kb_handler(void)
 * @brief Keyboard handler in C.
 */
void __kb_handler(void);

/**
 * @fn void kb_handler(void)
 * @brief Keyboard handler in assembler.
 */
void kb_handler(void);

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_64_KEYBOARD_H__ */
