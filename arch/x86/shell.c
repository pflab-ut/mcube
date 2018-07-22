/**
 * @file arch/x86/shell.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>
//============================================================================
/// @file       shell.c
/// @brief      Simple kernel shell for testing purposes.
//
// Copyright 2016 Brett Vickers.
// Use of this source code is governed by a BSD-style license that can
// be found in the MonkOS LICENSE file.
//============================================================================



// Forward declarations
static void command_prompt(void);
static void command_run(void);
static void keycode_run(void);
static bool cmd_display_help(void);
static bool cmd_display_apic(void);
static bool cmd_display_pci(void);
static bool cmd_display_pcie(void);
static bool cmd_switch_to_keycodes(void);
static bool cmd_test_heap(void);


// Standard shell command mode
static shell_mode_t mode_command = {
  command_prompt,
  command_run,
  NULL
};

// Keycode display mode
static shell_mode_t mode_keycode = {
  NULL,
  keycode_run,
  NULL
};


static shell_mode_t *active_mode;

static void switch_mode(shell_mode_t *mode)
{
  if (active_mode->stop) {
    active_mode->stop();
  }
  active_mode = mode;

  if (active_mode->start) {
    active_mode->start();
  }
}


static struct cmd commands[] = {
  { "?", NULL, cmd_display_help },
  { "help", "Show this help text", cmd_display_help },
  { "apic", "Show APIC configuration", cmd_display_apic },
  { "pci", "Show PCI devices", cmd_display_pci },
  { "pcie", "Show PCIexpress configuration", cmd_display_pcie },
  { "kc", "Switch to keycode display mode", cmd_switch_to_keycodes },
  { "heap", "Test heap allocation", cmd_test_heap },
};

static int cmp_cmds(const void *c1, const void *c2)
{
  return strcmp(((const struct cmd *)c1)->str,
                ((const struct cmd *)c2)->str);
}

static bool cmd_display_help(void)
{
  printk("Available commands:\n");
  for (int i = 0; i < arrsize(commands); i++) {
    if (!commands[i].help) {
      continue;
    }
    printk("  %s %s\n",
           commands[i].str, commands[i].help);
  }
  return TRUE;
}

static bool cmd_display_apic(void)
{
  const struct acpi_madt *madt = acpi_madt();
  if (!madt) {
    printk("No ACPI MADT detected.\n");
    return TRUE;
  }

  printk("Local APIC addr: %#x\n", madt->ptr_local_apic);

  const struct acpi_madt_local_apic *local = NULL;
  while ((local = acpi_next_local_apic(local))) {
    printk("Local APIC id %u: %s\n",
               local->apicid,
               (local->flags & 1) ? "Usable" : "Unusable");
  }

  const struct acpi_madt_io_apic *io = NULL;
  while ((io = acpi_next_io_apic(io))) {
    printk("I/O APIC id %u: Addr=%#x Base=%u\n",
               io->apicid,
               io->ptr_io_apic,
               io->interrupt_base);
  }

  const struct acpi_madt_iso *iso = NULL;
  while ((iso = acpi_next_iso(iso))) {
    printk("ISO irq=%-2u int=%-2u flags=0x%04x\n",
               iso->source,
               iso->interrupt,
               iso->flags);
  }

  return TRUE;
}

static bool cmd_display_pci(void)
{
  //  pci_init();         // Temporary
  return TRUE;
}

static bool cmd_display_pcie(void)
{
  const struct acpi_mcfg_addr *addr = acpi_next_mcfg_addr(NULL);
  if (!addr) {
    printk("No PCIe configuration.\n");
    return TRUE;
  }

  while (addr) {
    printk("PCIe addr=0x%lx  grp=%u bus=0x%x..0x%x\n",
               addr->base, addr->seg_group, addr->bus_start,
               addr->bus_end);
    addr = acpi_next_mcfg_addr(addr);
  }

  return TRUE;
}

static bool cmd_switch_to_keycodes(void)
{
  printk("Entering keycode mode. Hit Alt-Tab to exit.\n");
  switch_mode(&mode_keycode);
  return FALSE;
}

static bool cmd_test_heap(void)
{
  pagetable_t pt;
  pagetable_create(&pt, (void *)0x8000000000, PAGE_SIZE * 1024);
  pagetable_activate(&pt);

  struct heap *heap = heap_create(&pt, (void *)0x9000000000, 1024);
  void        *ptr1 = heap_alloc(heap, 128);
  void        *ptr2 = heap_alloc(heap, 0xff00);
  void        *ptr3 = heap_alloc(heap, 8);
  heap_free(heap, ptr1);
  heap_free(heap, ptr2);
  heap_free(heap, ptr3);

  heap_destroy(heap);
  pagetable_activate(NULL);
  pagetable_destroy(&pt);
  return TRUE;
}

static bool command_exec(const char *cmd)
{
  if (cmd[0] == 0) {
    return TRUE;
  }
  
  for (int i = 0; i < arrsize(commands); i++) {
    if (!strcmp(commands[i].str, cmd)) {
      return commands[i].run();
    }
  }

  printk("Unknown command: %s\n", cmd);
  return TRUE;
}

static void command_prompt(void)
{
  printk("> ");
}

static void command_run(void)
{
  char cmd[256];
  int  cmdlen = 0;

  for (;;) {
    halt();

    keyboard_t key;
    bool  avail;
    while ((avail = kb_getkey(&key)) != FALSE) {

      // If a printable character was typed, append it to the command.
      if (key.ch >= 32 && key.ch < 127) {
        if (cmdlen < arrsize(cmd) - 1) {
          cmd[cmdlen] = key.ch;
          putchar(cmd[cmdlen]);
          cmdlen++;
        }
      } else if (key.brk == KEYBRK_DOWN) {
      // Handle special keys (like enter, backspace).

        if (key.code == KEY_ENTER) {
          putchar('\n');

          // Strip trailing whitespace.
          while (cmdlen > 0 && cmd[cmdlen - 1] == ' ') {
            cmdlen--;
          }
          cmd[cmdlen] = 0;

          // Execute the command.
          bool cont = command_exec(cmd);
          cmdlen = 0;
          if (cont) {
            command_prompt();
          } else { 
            return;
          }
        } else if (key.code == KEY_BACKSPACE && cmdlen > 0) {
          putchar('\b');
          cmdlen--;
        }

      }
    }
  }
}

static void keycode_run(void)
{
  for (;;) {
    halt();

    keyboard_t key;
    bool  avail;
    while ((avail = kb_getkey(&key)) != FALSE) {
      if (key.ch) {
        printk("Keycode: \033[%c]%02x\033[-] meta=%x '%c'\n",
               key.brk == KEYBRK_UP ? 'e' : '2',
               key.code,
               key.meta,
               key.ch);
      } else {
        printk("Keycode: \033[%c]%02x\033[-] meta=%02x\n",
               key.brk == KEYBRK_UP ? 'e' : '2',
               key.code,
               key.meta);
      }
      if ((key.brk == KEYBRK_UP) && (key.meta & META_ALT) &&
          (key.code == KEY_TAB)) {
        switch_mode(&mode_command);
        return;
      }
    }
  }
}

void kshell(void)
{
  qsort(commands, arrsize(commands), sizeof(struct cmd), cmp_cmds);

  active_mode = &mode_command;
  active_mode->start();
  for (;;) {
    active_mode->run();
  }
}
