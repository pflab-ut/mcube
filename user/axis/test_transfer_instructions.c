/**
 * @file user/test_transfer_instructions.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>
#include "user_axis.h"

#define BASE_ADDR (MEMORY_SIZE / 2)
#define OFFSET_ADDR 0x12

static void test_mts(void)
{
  /*
  int rx = 12;
  int rt;
  asm volatile("mts %0, %1" : "=r"(rt) : "r"(rx));
  print("test_mts(): %d = %d\n", rx, rt);
  */
}

static void test_mfs(void)
{
  /*
  int rt = 12;
  int rd;
  asm volatile("mfs %0, %1" : "=r"(rd) : "r"(rt));
  print("test_mfs(): %d = %d\n", rt, rd);
  */
}

static void test_lb(void)
{
  unsigned int rx = BASE_ADDR;
  int rc;
  asm volatile("lb %0, %1(%2)" : "=r"(rc) : "i"(OFFSET_ADDR), "r"(rx));
  print("test_lb(): 0x%x(0x%x) = 0x%x\n", OFFSET_ADDR, rx, rc);
}

static void test_lbu(void)
{
  unsigned int rx = BASE_ADDR;
  unsigned int rc;
  asm volatile("lbu %0, %1(%2)" : "=r"(rc) : "i"(OFFSET_ADDR), "r"(rx));
  print("test_lbu(): 0x%x(0x%x) = 0x%x\n", OFFSET_ADDR, rx, rc);
}

static void test_lh(void)
{
  unsigned int rx = BASE_ADDR;
  int rc;
  asm volatile("lh %0, %1(%2)" : "=r"(rc) : "i"(OFFSET_ADDR), "r"(rx));
  print("test_lh(): 0x%x(0x%x) = 0x%x\n", OFFSET_ADDR, rx, rc);
}

static void test_lhu(void)
{
  unsigned int rx = BASE_ADDR;
  unsigned int rc;
  asm volatile("lhu %0, %1(%2)" : "=r"(rc) : "i"(OFFSET_ADDR), "r"(rx));
  print("test_lhu(): 0x%x(0x%x) = 0x%x\n", OFFSET_ADDR, rx, rc);
}

static void test_lw(void)
{
  unsigned int rx = BASE_ADDR;
  int rc;
  asm volatile("lw %0, %1(%2)" : "=r"(rc) : "i"(OFFSET_ADDR), "r"(rx));
  print("test_lw(): 0x%x(0x%x) = 0x%x\n", OFFSET_ADDR, rx, rc);
}

static void test_sb(void)
{
  unsigned int rx = BASE_ADDR;
  int rc = 56;
  asm volatile("sb %0, %1(%2)" :: "r"(rc), "i"(OFFSET_ADDR), "r"(rx));
  print("test_sb(): 0x%x(0x%x) = 0x%x\n", OFFSET_ADDR, rx, rc);
}

static void test_sh(void)
{
  unsigned int rx = BASE_ADDR;
  int rc = 56;
  asm volatile("sh %0, %1(%2)" :: "r"(rc), "i"(OFFSET_ADDR), "r"(rx));
  print("test_sh(): 0x%x(0x%x) = 0x%x\n", OFFSET_ADDR, rx, rc);
}

static void test_sw(void)
{
  volatile unsigned int rx = BASE_ADDR;
  volatile int rc = 56;
  asm volatile("sw %0, %1(%2)" :: "r"(rc), "i"(OFFSET_ADDR), "r"(rx));
  print("test_sw(): 0x%x(0x%x) = 0x%x\n", OFFSET_ADDR, rx, rc);
}


void test_transfer_instructions(void)
{
  *((uint32_t *) (BASE_ADDR + OFFSET_ADDR)) = 0x12345678;
  test_mts();
  test_mfs();
  test_lb();
  test_lbu();
  test_lh();
  test_lhu();
  test_lw();
  test_sb();
  test_sh();
  test_sw();
}

