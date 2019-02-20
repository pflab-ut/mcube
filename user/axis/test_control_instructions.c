/**
 * @file user/test_control_instructions.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>
#include "user_axis.h"

void test_jal_func(void);

void test_jal_func(void)
{
  print("test_jal_func()\n");
}

static void test_jal(void)
{
  /* different from MIPS I's JAL. */
  //  unsigned int target = ((unsigned int) test_jal_func) >> 2;
  print("test_jal()\n");
  asm volatile("jal test_jal_func");
  //  asm volatile("jal target");
}

static void test_jalr_func(void)
{
  print("test_jalr_func()\n");
}

static void test_jalr(void)
{
  unsigned int rx = (unsigned int) test_jalr_func;
  print("test_jalr()\n");
  asm volatile("jalr $31, %0" :: "r"(rx));
}


static void test_beq(void)
{
  unsigned int rx = 12;
  unsigned int ry = 12;
  print("test_beq()\n");
  asm volatile("beq %0, %1, label_beq" :: "r"(rx), "r"(ry));
  asm volatile("sync");
  asm volatile("label_beq:");
  asm volatile("b label_b");
  asm volatile("sync");
  asm volatile("label_b:");
  asm volatile("beqz %0, label_beqz" :: "r"(rx));
  asm volatile("sync");
  asm volatile("label_beqz:");
}

static void test_bne(void)
{
  unsigned int rx = 12;
  unsigned int ry = 34;
  print("test_bne()\n");
  asm volatile("bne %0, %1, label_bne" :: "r"(rx), "r"(ry));
  asm volatile("sync");
  asm volatile("label_bne:");
  asm volatile("bnez %0, label_bnez" :: "r"(rx));
  asm volatile("sync");
  asm volatile("label_bnez:");
}


static void test_blt(void)
{
  //  int rx = 12;
  //  int ry = 12;
  print("test_blt()\n");
  asm volatile("blt $5, $6, label_blt");
  asm volatile("sync");
  asm volatile("label_blt:");
  asm volatile("bltz $5, label_bltz");
  asm volatile("sync");
  asm volatile("label_bltz:");
  asm volatile("bgt $5, $6, label_bgt");
  asm volatile("sync");
  asm volatile("label_bgt:");
  asm volatile("bgtz $5, label_bgtz");
  asm volatile("sync");
  asm volatile("label_bgtz:");
}

static void test_bltu(void)
{
  //  unsigned int rx = 12;
  //  unsigned int ry = 12;
  print("test_bltu()\n");
  //  asm volatile("bltu %0, %1, %2" :: "r"(rx), "r"(ry), "i"(4));
  asm volatile("bltu $15, $16, label_bltu");
  asm volatile("label_bltu:");
  asm volatile("bgtu $15, $16, label_bgtu");
  asm volatile("label_bgtu:");
  
}

static void test_bge(void)
{
  int rx = 12;
  int ry = 12;
  print("test_bge()\n");
  asm volatile("bge %0, %1, label_bge" :: "r"(rx), "r"(ry));
  asm volatile("label_bge:");
  asm volatile("sync");
  asm volatile("bgez %0, label_bgez" :: "r"(rx));
  asm volatile("label_bgez:");
  asm volatile("sync");
  asm volatile("ble %0, %1, label_ble" :: "r"(ry), "r"(rx));
  asm volatile("label_ble:");
  asm volatile("sync");
  asm volatile("blez %0, label_bgez" :: "r"(ry));
  asm volatile("label_blez:");
  asm volatile("sync");
}

static void test_bgeu(void)
{
  //  unsigned int rx = 12;
  //  unsigned int ry = 12;
  print("test_bgeu()\n");
  //  asm volatile("bgeu %0, %1, %2" :: "r"(rx), "r"(ry), "i"(4));
  asm volatile("bgeu $17, $18, label_bgeu");
  asm volatile("label_bgeu:");
  asm volatile("sync");
  asm volatile("bleu $18, $17, label_bleu");
  asm volatile("sync");
  asm volatile("label_bleu:");
}


void test_control_instructions(void)
{
  test_jal();
  test_jalr();
  test_beq();
  test_bne();
  test_blt();
  test_bltu();
  test_bge();
  test_bgeu();
}
