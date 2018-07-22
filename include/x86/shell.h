/**
 * @file include/x86/shell.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_SHELL_H__
#define __MCUBE_X86_SHELL_H__

#ifndef __ASSEMBLY__
//============================================================================
/// @file       shell.h
/// @brief      Simple kernel shell for testing purposes.
//
// Copyright 2016 Brett Vickers.
// Use of this source code is governed by a BSD-style license that can
// be found in the MonkOS LICENSE file.
//============================================================================


/// Shell mode descriptor.
typedef struct shell_mode {
  void (*start)(void);
  void (*run)(void);
  void (*stop)(void);
} shell_mode_t;

/// A command descriptor, describing each command accepted in command mode.
struct cmd {
  const char *str;
  const char *help;
  bool        (*run)();
};

void kshell(void);

#endif	/* !__ASSEMBLY__ */

#endif	/* __MCUBE_X86_SHELL_H__ */
