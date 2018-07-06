//============================================================================
/// @file       main.c
/// @brief      The kernel's main entry point.
/// @details    This file contains the function kmain(), which is the first
///             function called by the kernel's start code in start.asm.
//
// Copyright 2016 Brett Vickers.
// Use of this source code is governed by a BSD-style license that can
// be found in the MonkOS LICENSE file.
//============================================================================

#include <mcube/mcube.h>

void kmain(void)
{
  main(0, NULL);
  for (;;)
    ;
#if 0
    // Memory initialization
    acpi_init();
    pmap_init();
    page_init();

    // Interrupt initialization
    interrupts_init();
    exceptions_init();

    // Device initialization
    tty_init();
    kb_init();
    timer_init(20);     // 20Hz

    // System call initialization
    syscall_init();

    // Let the games begin
    enable_interrupt();

    // Display a welcome message on the virtual console.
    tty_set_textcolor(TTY_CONSOLE, TEXTCOLOR_LTGRAY, TEXTCOLOR_BLACK);
    tty_clear(TTY_CONSOLE);
    tty_print(TTY_CONSOLE, "Welcome to \033[e]MonkOS\033[-] (v0.1).\n");

    // Launch the interactive test shell.
    kshell();
#endif
}
