/**
 * @file user/axis/test_integer_instructions.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>
#include "user_axis.h"

static void test_add(void)
{
  int rx = 12;
  int ry = 34;
  int rd;
  
  asm volatile("add %0, %1, %2" : "=r"(rd) : "r"(rx), "r"(ry));
  print("test_add(): %d + %d = %d\n", rx, ry, rd);
}

static void test_add_c(void)
{
  int rx = 12;
  int ry = 34;
  int rd;

  rd = rx + ry;
  print("test_add_c(): %d + %d = %d\n", rx, ry, rd);
}

static void test_addi(void)
{
  int rx = 12;
  int rc;
  
  asm volatile("addi %0, %1, %2" : "=r"(rc) : "r"(rx), "i"(34));
  print("test_addi(): %d + %d = %d\n", rx, 34, rc);
}

static void test_addi_c(void)
{
  int rx = 12;
  int rc;

  rc = rx + 34;
  print("test_addi_c(): %d + %d = %d\n", rx, 34, rc);
}

static void test_addi_li(void)
{
  int rc;
  asm volatile("li %0, %1" : "=r"(rc) : "i"(34));
  print("test_addi_li(): 0 + %d = %d\n", 34, rc);
}


static void test_sub(void)
{
  int rx = 56;
  int ry = 34;
  int rd;
  asm volatile("sub %0, %1, %2" : "=r"(rd) : "r"(rx), "r"(ry));
  print("test_sub(): %d - %d = %d\n", rx, ry, rd);
}


static void test_sub_c(void)
{
  int rx = 56;
  int ry = 34;
  int rd;
  rd = rx - ry;
  print("test_sub_c(): %d - %d = %d\n", rx, ry, rd);
}

static void test_sub_neg(void)
{
  int ry = 56;
  int rd;
  asm volatile("neg %0, %1" : "=r"(rd) : "r"(ry));
  print("test_sub_neg(): 0 - %d = %d\n", ry, rd);
}

static void test_mul(void)
{
  int rx = 12;
  int ry = 34;
  int rd;
  
  asm volatile("mul %0, %1, %2" : "=r"(rd) : "r"(rx), "r"(ry));
  print("test_mul(): %d * %d = %d\n", rx, ry, rd);
}

static void test_mul_c(void)
{
  int rx = 12;
  int ry = 34;
  int rd;
  
  rd = rx * ry;
  print("test_mul_c(): %d * %d = %d\n", rx, ry, rd);  
}

static void test_muli(void)
{
  int rx = 12;
  int rc;
  asm volatile("muli %0, %1, %2" : "=r"(rc) : "r"(rx), "i"(34));
  print("test_muli(): %d * %d = %d\n", rx, 34, rc);
}

static void test_muli_c(void)
{
  int rx = 12;
  int rc;
  rc = rx * 34;
  print("test_muli_c(): %d * %d = %d\n", rx, 34, rc);
}

static void test_div(void)
{
  int rx = 1234;
  int ry = 56;
  int rd;
  
  asm volatile("div %0, %1, %2" : "=r"(rd) : "r"(rx), "r"(ry));
  print("test_div(): %d / %d = %d\n", rx, ry, rd);
}

static void test_div_c(void)
{
  int rx = 1234;
  int ry = 56;
  int rd;

  rd = rx / ry;
  print("test_div_c(): %d / %d = %d\n", rx, ry, rd);
}

static void test_divi(void)
{
  int rx = 1234;
  int rc;
  asm volatile("divi %0, %1, %2" : "=r"(rc) : "r"(rx), "i"(56));
  print("test_divi(): %d / %d = %d\n", rx, 56, rc);
}

static void test_divi_c(void)
{
  int rx = 1234;
  int rc;
  rc = rx / 56;
  print("test_divi_c(): %d / %d = %d\n", rx, 56, rc);
}

static void test_divu(void)
{
  unsigned int rx = 1234;
  unsigned int ry = 56;
  unsigned int rd;
  
  asm volatile("divu %0, %1, %2" : "=r"(rd) : "r"(rx), "r"(ry));
  print("test_divu(): %u / %u = %u\n", rx, ry, rd);
}

static void test_divu_c(void)
{
  unsigned int rx = 1234;
  unsigned int ry = 56;
  unsigned int rd;

  rd = rx / ry;
  print("test_divu_c(): %u / %u = %u\n", rx, ry, rd);
}

static void test_rem(void)
{
  int rx = 1234;
  int ry = 56;
  int rd;
  
  asm volatile("rem %0, %1, %2" : "=r"(rd) : "r"(rx), "r"(ry));
  print("test_rem(): %d %% %d = %d\n", rx, ry, rd);
}

static void test_rem_c(void)
{
  int rx = 1234;
  int ry = 56;
  int rd;

  rd = rx % ry;
  print("test_rem_c(): %d %% %d = %d\n", rx, ry, rd);
}


static void test_remu(void)
{
  unsigned int rx = 1234;
  unsigned int ry = 56;
  unsigned int rd;
  
  asm volatile("remu %0, %1, %2" : "=r"(rd) : "r"(rx), "r"(ry));
  print("test_remu(): %u %% %u = %u\n", rx, ry, rd);
}

static void test_remu_c(void)
{
  unsigned int rx = 1234;
  unsigned int ry = 56;
  unsigned int rd;
  
  rd = rx % ry;
  print("test_remu_c(): %u %% %u = %u\n", rx, ry, rd);
}

static void test_and(void)
{
  unsigned int rx = 0x5a5a;
  unsigned int ry = 0x5a00;
  unsigned int rd;
  
  asm volatile("and %0, %1, %2" : "=r"(rd) : "r"(rx), "r"(ry));
  print("test_and(): 0x%x & 0x%x = 0x%x\n", rx, ry, rd);
}

static void test_and_c(void)
{
  unsigned int rx = 0x5a5a;
  unsigned int ry = 0x5a00;
  unsigned int rd;
  
  rd = rx & ry;
  print("test_and_c(): 0x%x & 0x%x = 0x%x\n", rx, ry, rd);
}

static void test_andi(void)
{
  unsigned int rx = 0x5a5a;
  unsigned int rc;
  
  asm volatile("andi %0, %1, %2" : "=r"(rc) : "r"(rx), "i"(0x5a00));
  print("test_andi(): 0x%x & 0x%x = 0x%x\n", rx, 0x5a00, rc);
}

static void test_andi_c(void)
{
  unsigned int rx = 0x5a5a;
  unsigned int rc;
  
  rc = rx & 0x5a00;
  print("test_andi_c(): 0x%x & 0x%x = 0x%x\n", rx, 0x5a00, rc);
}

static void test_nand(void)
{
  unsigned int rx = 0x5a5a;
  unsigned int ry = 0x5a00;
  unsigned int rd;
  
  asm volatile("nand %0, %1, %2" : "=r"(rd) : "r"(rx), "r"(ry));
  print("test_nand(): ~(0x%x & 0x%x) = 0x%x\n", rx, ry, rd);
}

static void test_nand_c(void)
{
  unsigned int rx = 0x5a5a;
  unsigned int ry = 0x5a00;
  unsigned int rd;
  
  rd = ~(rx & ry);
  print("test_nand_c(): ~(0x%x & 0x%x) = 0x%x\n", rx, ry, rd);
}

static void test_or(void)
{
  unsigned int rx = 0x5a5a;
  unsigned int ry = 0x5a00;
  unsigned int rd;
  
  asm volatile("or %0, %1, %2" : "=r"(rd) : "r"(rx), "r"(ry));
  print("test_or(): 0x%x | 0x%x = 0x%x\n", rx, ry, rd);
}

static void test_or_c(void)
{
  unsigned int rx = 0x5a5a;
  unsigned int ry = 0x5a00;
  unsigned int rd;
  
  rd = rx | ry;
  print("test_or_c(): 0x%x | 0x%x = 0x%x\n", rx, ry, rd);
}

static void test_or_move(void)
{
  unsigned int rx = 0x5a5a;
  unsigned int rd;
  
  asm volatile("move %0, %1" : "=r"(rd) : "r"(rx));
  print("test_move(): 0x%x | 0x0 = 0x%x\n", rx, rd);
}

static void test_ori(void)
{
  unsigned int rx = 0x5a5a;
  unsigned int rc;
  
  asm volatile("ori %0, %1, %2" : "=r"(rc) : "r"(rx), "i"(0x5a00));
  print("test_ori(): 0x%x | 0x%x = 0x%x\n", rx, 0x5a00, rc);
}

static void test_ori_c(void)
{
  unsigned int rx = 0x5a5a;
  unsigned int rc;
  
  rc = rx | 0x5a00;
  print("test_ori_c(): 0x%x | 0x0 = 0x%x\n", 0x5a00, rc);
}

static void test_ori_liu(void)
{
  unsigned int rc;
  
  asm volatile("liu %0, %1" : "=r"(rc) : "i"(0x5a00));
  print("test_ori_liu(): 0x%x | 0x0 = 0x%x\n", 0x5a00, rc);
}

static void test_nor(void)
{
  unsigned int rx = 0x5a5a;
  unsigned int ry = 0x5a00;
  unsigned int rd;
  
  asm volatile("nor %0, %1, %2" : "=r"(rd) : "r"(rx), "r"(ry));
  print("test_nor(): ~(0x%x | 0x%x) = 0x%x\n", rx, ry, rd);
}

static void test_nor_c(void)
{
  unsigned int rx = 0x5a5a;
  unsigned int ry = 0x5a00;
  unsigned int rd;
  
  rd = ~(rx | ry);
  print("test_nor_c(): ~(0x%x | 0x%x) = 0x%x\n", rx, ry, rd);
}

static void test_nor_not(void)
{
  unsigned int rx = 0x5a5a;
  unsigned int rd;
  
  asm volatile("not %0, %1" : "=r"(rd) : "r"(rx));
  print("test_nor_not(): ~0x%x = 0x%x\n", rx, rd);
}

static void test_nor_not_c(void)
{
  unsigned int rx = 0x5a5a;
  unsigned int rd;

  rd = ~rx;
  print("test_nor_not_c(): ~0x%x = 0x%x\n", rx, rd);
}


static void test_xor(void)
{
  unsigned int rx = 0x5a5a;
  unsigned int ry = 0x5a00;
  unsigned int rd;
  
  asm volatile("xor %0, %1, %2" : "=r"(rd) : "r"(rx), "r"(ry));
  print("test_xor(): 0x%x ^ 0x%x = 0x%x\n", rx, ry, rd);
}

static void test_xor_c(void)
{
  unsigned int rx = 0x5a5a;
  unsigned int ry = 0x5a00;
  unsigned int rd;
  
  rd = rx ^ ry;
  print("test_xor_c(): 0x%x ^ 0x%x = 0x%x\n", rx, ry, rd);
}


static void test_xori(void)
{
  unsigned int rx = 0x5a5a;
  unsigned int rc;
  
  asm volatile("xori %0, %1, %2" : "=r"(rc) : "r"(rx), "i"(0x5a00));
  print("test_xori(): 0x%x ^ 0x%x = 0x%x\n", rx, 0x5a00, rc);
}

static void test_xori_c(void)
{
  unsigned int rx = 0x5a5a;
  unsigned int rc;
  
  rc = rx ^ 0x5a5a;
  print("test_xori_c(): 0x%x ^ 0x%x = 0x%x\n", rx, 0x5a00, rc);
}


static void test_xnor(void)
{
  unsigned int rx = 0x5a5a;
  unsigned int ry = 0x5a00;
  unsigned int rd;
  
  asm volatile("xnor %0, %1, %2" : "=r"(rd) : "r"(rx), "r"(ry));
  print("test_xnor(): ~(0x%x ^ 0x%x) = 0x%x\n", rx, ry, rd);
}

static void test_xnor_c(void)
{
  unsigned int rx = 0x5a5a;
  unsigned int ry = 0x5a00;
  unsigned int rd;
  
  rd = ~(rx ^ ry);
  print("test_xnor_c(): ~(0x%x ^ 0x%x) = 0x%x\n", rx, ry, rd);
}

static void test_sll(void)
{
  unsigned int rx = 0x5a5a;
  unsigned int ry = 4;
  unsigned int rd;
  
  asm volatile("sll %0, %1, %2" : "=r"(rd) : "r"(rx), "r"(ry));
  print("test_sll(): 0x%x << 0x%x = 0x%x\n", rx, ry, rd);
}

static void test_sll_c(void)
{
  unsigned int rx = 0x5a5a;
  unsigned int ry = 4;
  unsigned int rd;
  
  rd = rx << ry;
  print("test_sll_c(): 0x%x << 0x%x = 0x%x\n", rx, ry, rd);
}

static void test_slli(void)
{
  unsigned int rx = 0x5a5a;
  unsigned int rc;
  
  asm volatile("slli %0, %1, %2" : "=r"(rc) : "r"(rx), "i"(0x4));
  print("test_slli(): 0x%x << 0x%x = 0x%x\n", rx, 0x4, rc);
}

static void test_slli_c(void)
{
  unsigned int rx = 0x5a5a;
  unsigned int rc;
  
  rc = rx << 0x4;
  print("test_slli_c(): 0x%x << 0x%x = 0x%x\n", rx, 0x4, rc);
}

static void test_srl(void)
{
  unsigned int rx = 0x5a5a;
  unsigned int ry = 4;
  unsigned int rd;
  
  asm volatile("srl %0, %1, %2" : "=r"(rd) : "r"(rx), "r"(ry));
  print("test_srl(): 0x%x >> 0x%x = 0x%x\n", rx, ry, rd);
}

static void test_srl_c(void)
{
  unsigned int rx = 0x5a5a;
  unsigned int ry = 4;
  unsigned int rd;
  
  rd = rx >> ry;
  print("test_srl_c(): 0x%x >> 0x%x = 0x%x\n", rx, ry, rd);
}

static void test_srli(void)
{
  unsigned int rx = 0x5a5a;
  unsigned int rc;
  
  asm volatile("srli %0, %1, %2" : "=r"(rc) : "r"(rx), "i"(0x4));
  print("test_srli(): 0x%x >> 0x%x = 0x%x\n", rx, 0x4, rc);
}

static void test_srli_c(void)
{
  unsigned int rx = 0x5a5a;
  unsigned int rc;
  
  rc = rx >> 4;
  print("test_srli_c(): 0x%x >> 0x%x = 0x%x\n", rx, 0x4, rc);
}

static void test_sra(void)
{
  int rx = 0xffff5a5a;
  int ry = 4;
  int rd;
  
  asm volatile("sra %0, %1, %2" : "=r"(rd) : "r"(rx), "r"(ry));
  print("test_sra(): 0x%x >> 0x%x = 0x%x\n", rx, ry, rd);
}

static void test_sra_c(void)
{
  int rx = 0xffff5a5a;
  int ry = 4;
  int rd;
  
  rd = rx >> ry;
  print("test_sra_c(): 0x%x >> 0x%x = 0x%x\n", rx, ry, rd);
}

static void test_srai(void)
{
  int rx = 0xffff5a5a;
  int rc;
  
  asm volatile("srai %0, %1, %2" : "=r"(rc) : "r"(rx), "i"(0x4));
  print("test_srai(): 0x%x >> 0x%x = 0x%x\n", rx, 4, rc);
}

static void test_srai_c(void)
{
  int rx = 0xffff5a5a;
  int rc;
  
  rc = rx >> 4;
  print("test_srai_c(): 0x%x >> 0x%x = 0x%x\n", rx, 4, rc);
}

static void test_seq(void)
{
  int rx = 12;
  int ry = 12;
  int rd;
  
  asm volatile("seq %0, %1, %2" : "=r"(rd) : "r"(rx), "r"(ry));
  print("test_seq(): %d == %d = %d\n", rx, ry, rd);
}

static void test_seq_c(void)
{
  int rx = 12;
  int ry = 12;
  int rd;

  rd = rx == ry;
  print("test_seq(): %d == %d = %d\n", rx, ry, rd);
}

static void test_seq_seqz(void)
{
  int rx = 12;
  int rd;
  
  asm volatile("seqz %0, %1" : "=r"(rd) : "r"(rx));
  print("test_seq_seqz(): %d == 0 = %d\n", rx, rd);
}

static void test_seqi(void)
{
  int rx = 12;
  int rc;
  
  asm volatile("seqi %0, %1, %2" : "=r"(rc) : "r"(rx), "i"(12));
  print("test_seqi(): %d == %d = %d\n", rx, 12, rc);
}

static void test_seqi_c(void)
{
  int rx = 12;
  int rc;

  rc = rx == 12;
  print("test_seqi(): %d == %d = %d\n", rx, 12, rc);
}

static void test_sne(void)
{
  int rx = 12;
  int ry = 12;
  int rd;
  
  asm volatile("sne %0, %1, %2" : "=r"(rd) : "r"(rx), "r"(ry));
  print("test_sne(): %d != %d = %d\n", rx, ry, rd);
}

static void test_sne_c(void)
{
  int rx = 12;
  int ry = 12;
  int rd;

  rd = rx != ry;
  print("test_sne_c(): %d != %d = %d\n", rx, ry, rd);
}

static void test_sne_snez(void)
{
  int rx = 12;
  int ry = 12;
  int rd;
  
  asm volatile("snez %0, %1" : "=r"(rd) : "r"(rx));
  print("test_sne_snez(): %d != %d = %d\n", rx, ry, rd);
}

static void test_snei(void)
{
  int rx = 12;
  int rc;
  
  asm volatile("snei %0, %1, %2" : "=r"(rc) : "r"(rx), "i"(12));
  print("test_snei(): %d != %d = %d\n", rx, 12, rc);
}

static void test_snei_c(void)
{
  int rx = 12;
  int rc;

  rc = rx != 12;
  print("test_snei_c(): %d != %d = %d\n", rx, 12, rc);
}

static void test_slt(void)
{
  int rx = 12;
  int ry = 21;
  int rd;
  
  asm volatile("slt %0, %1, %2" : "=r"(rd) : "r"(rx), "r"(ry));
  print("test_slt(): %d < %d = %d\n", rx, ry, rd);
}

static void test_slt_c(void)
{
  int rx = 12;
  int ry = 21;
  int rd;
  
  rd = rx < ry;
  print("test_slt_c(): %d < %d = %d\n", rx, ry, rd);
}

static void test_slt_sltz(void)
{
  int rx = 12;
  int rd;
  
  asm volatile("sltz %0, %1" : "=r"(rd) : "r"(rx));
  print("test_sltz(): %d < 0 = %d\n", rx, rd);
}

static void test_slt_sgt(void)
{
  int rx = 12;
  int ry = 21;
  int rd;
  
  asm volatile("sgt %0, %1, %2" : "=r"(rd) : "r"(ry), "r"(rx));
  print("test_slt_sgt(): %d > %d = %d\n", ry, rx, rd);
}

static void test_slt_sgt_c(void)
{
  int rx = 12;
  int ry = 21;
  int rd;

  rd = ry > rx;
  print("test_slt_sgt_c(): %d > %d = %d\n", ry, rx, rd);
}

static void test_slt_sgtz(void)
{
  int ry = 21;
  int rd;
  
  asm volatile("sgtz %0, %1" : "=r"(rd) : "r"(ry));
  print("test_slt_sgtz(): %d > 0 = %d\n", ry, rd);
}

static void test_slti(void)
{
  int rx = 21;
  int rc;
  
  asm volatile("slti %0, %1, %2" : "=r"(rc) : "r"(rx), "i"(12));
  print("test_slti(): %d < %d = %d\n", rx, 21, rc);
}

/* NOTE: not exist "rx > 21". */
static void test_slti_c(void)
{
  int rx = 12;
  int rc;

  rc = rx < 21;
  print("test_slti_c(): %d < %d = %d\n", rx, 21, rc);
  asm volatile("sync");
  rc = rx > 21;
  print("test_slti_c(): %d > %d = %d\n", rx, 21, rc);
}

static void test_sltu(void)
{
  unsigned int rx = 12;
  unsigned int ry = 21;
  unsigned int rd;
  
  asm volatile("sltu %0, %1, %2" : "=r"(rd) : "r"(rx), "r"(ry));
  print("test_sltu(): %u < %u = %u\n", rx, ry, rd);
}

static void test_sltu_c(void)
{
  unsigned int rx = 12;
  unsigned int ry = 21;
  unsigned int rd;

  rd = rx < ry;
  print("test_sltu_c(): %u < %u = %u\n", rx, ry, rd);
}

static void test_sgtu(void)
{
  unsigned int rx = 12;
  unsigned int ry = 21;
  unsigned int rd;

  asm volatile("sgtu %0, %1, %2" : "=r"(rd) : "r"(ry), "r"(rx));
  print("test_sgtu(): %u > %u = %u\n", ry, rx, rd);
}

static void test_sgtu_c(void)
{
  unsigned int rx = 12;
  unsigned int ry = 21;
  unsigned int rd;

  rd = ry > rx;
  print("test_sgtu_c(): %u > %u = %u\n", ry, rx, rd);
}

static void test_sge(void)
{
  int rx = 12;
  int ry = 21;
  int rd;
  
  asm volatile("sge %0, %1, %2" : "=r"(rd) : "r"(rx), "r"(ry));
  print("test_sge(): %d >= %d = %d\n", rx, ry, rd);
}

static void test_sge_c(void)
{
  int rx = 12;
  int ry = 21;
  int rd;

  rd = rx >= ry;
  print("test_sge_c(): %d >= %d = %d\n", rx, ry, rd);
}

static void test_sge_sgez(void)
{
  int rx = 12;
  int rd;
  
  asm volatile("sgez %0, %1" : "=r"(rd) : "r"(rx));
  print("test_sge_sgez(): %d >= 0 = %d\n", rx, rd);
}

static void test_sge_sle(void)
{
  int rx = 12;
  int ry = 21;
  int rd;
  
  asm volatile("sle %0, %1, %2" : "=r"(rd) : "r"(ry), "r"(rx));
  print("test_sge_sle(): %d <= %d = %d\n", rx, ry, rd);
}

static void test_sge_sle_c(void)
{
  int rx = 12;
  int ry = 21;
  int rd;

  rd = rx <= ry;
  print("test_sge_sle_c(): %d <= %d = %d\n", rx, ry, rd);
}

static void test_sge_slez(void)
{
  int ry = 21;
  int rd;
  asm volatile("slez %0, %1" : "=r"(rd) : "r"(ry));
  print("test_sge_slez(): %d >= 0 = %d\n", ry, rd);
}


static void test_sgei(void)
{
  int rx = 12;
  int rc;
  
  asm volatile("sgei %0, %1, %2" : "=r"(rc) : "r"(rx), "i"(12));
  print("test_sgei(): %d >= %d = %d\n", rx, 12, rc);
}

static void test_sgei_c(void)
{
  int rx = 12;
  int rc;

  rc = rx >= 12;
  print("test_sgei_c(): %d >= %d = %d\n", rx, 12, rc);
}

static void test_sgeu(void)
{
  unsigned int rx = 12;
  unsigned int ry = 12;
  unsigned int rd;
  
  asm volatile("sgeu %0, %1, %2" : "=r"(rd) : "r"(rx), "r"(ry));
  print("test_sgeu(): %d >= %d = %d\n", rx, ry, rd);
}

static void test_sgeu_c(void)
{
  unsigned int rx = 12;
  unsigned int ry = 12;
  unsigned int rd;
  
  rd = rx >= ry;
  print("test_sgeu_c(): %d >= %d = %d\n", rx, ry, rd);
}

static void test_sgeu_sleu(void)
{
  unsigned int rx = 12;
  unsigned int ry = 12;
  unsigned int rd;
  
  asm volatile("sleu %0, %1, %2" : "=r"(rd) : "r"(ry), "r"(rx));
  print("test_sgeu_sleu(): %d <= %d = %d\n", rx, ry, rd);
}

static void test_sgeu_sleu_c(void)
{
  unsigned int rx = 12;
  unsigned int ry = 12;
  unsigned int rd;

  rd = rx <= ry;
  print("test_sgeu_sleu_c(): %d <= %d = %d\n", rx, ry, rd);
}

static void test_seb(void)
{
  signed char rx = 0xf0;
  int rd;
  
  asm volatile("seb %0, %1" : "=r"(rd) : "r"(rx));
  print("test_seb(): SignExtend(0x%x) = 0x%x\n", rx, rd);
}

static void test_seb_c(void)
{
  signed char rx = 0xf0;
  int rd;

  rd = rx;
  print("test_seb_c(): SignExtend(0x%x) = 0x%x\n", rx, rd);
}

static void test_seh(void)
{
  short rx = 0xf000;
  int rd;
  
  asm volatile("seh %0, %1" : "=r"(rd) : "r"(rx));
  print("test_seh(): SignExtend(0x%x) = 0x%x\n", rx, rd);
}

static void test_seh_c(void)
{
  short rx = 0xf000;
  int rd;

  rd = rx;
  print("test_seh_c(): SignExtend(0x%x) = 0x%x\n", rx, rd);
}

static void test_cati(void)
{
  int rx = 0xf000;
  int rd;
  
  asm volatile("cati %0, %1, %2" : "=r"(rd) : "r"(rx), "i"(0x1234));
  print("test_cati(): {0x%x, 0x%x} = 0x%x\n", rx, 0x1234, rd);
}

static void test_cati_c(void)
{
  int rx = 0xf000;
  int rd;

  rd = (0x1234 << 16) | (rx & 0xffff);
  print("test_cati(): {0x%x, 0x%x} = 0x%x\n", rx, 0x1234, rd);
}

static void test_cati_lui(void)
{
  int rd;
  
  asm volatile("lui %0, %1" : "=r"(rd) : "i"(0x1234));
  print("test_cati_lui(): {0x%x, 0x%x} = 0x%x\n", 0x0, 0x1234, rd);
}

void test_integer_instructions(void)
{
  test_add();
  test_add_c();

  test_addi();
  test_addi_c();
  test_addi_li();
  
  test_sub();
  test_sub_c();
  test_sub_neg();
  
  test_mul();
  test_mul_c();
  
  test_muli();
  test_muli_c();
  
  test_div();
  test_div_c();
  
  test_divi();
  test_divi_c();
  
  test_divu();
  test_divu_c();
  
  test_rem();
  test_rem_c();
  
  test_remu();
  test_remu_c();
  
  test_and();
  test_and_c();
  
  test_andi();
  test_andi_c();
  
  test_nand();
  test_nand_c();
  
  test_or();
  test_or_c();
  test_or_move();
  
  test_ori();
  test_ori_c();
  test_ori_liu();
  
  test_nor();
  test_nor_c();
  test_nor_not();
  test_nor_not_c();
  
  test_xor();
  test_xor_c();
  
  test_xori();
  test_xori_c();
  
  test_xnor();
  test_xnor_c();
  
  test_sll();
  test_sll_c();
  
  test_slli();
  test_slli_c();
  
  test_srl();
  test_srl_c();
  
  test_srli();
  test_srli_c();
  
  test_sra();
  test_sra_c();
  
  test_srai();
  test_srai_c();
  
  test_seq();
  test_seq_c();
  test_seq_seqz();
  
  test_seqi();
  test_seqi_c();
  
  test_sne();
  test_sne_c();
  test_sne_snez();

  test_snei();
  test_snei_c();

  test_slt();
  test_slt_c();
  test_slt_sltz();
  test_slt_sgt();
  test_slt_sgt_c();
  test_slt_sgtz();

  test_slti();
  test_slti_c();
  test_sltu();
  test_sltu_c();
  
  test_sgtu();
  test_sgtu_c();
  
  test_sge();
  test_sge_c();
  test_sge_sgez();
  test_sge_sle();
  test_sge_sle_c();
  test_sge_slez();
  
  test_sgei();
  test_sgei_c();
    
  test_sgeu();
  test_sgeu_c();
  test_sgeu_sleu();
  test_sgeu_sleu_c();
  
  test_seb();
  test_seb_c();
  
  test_seh();
  test_seh_c();
  
  test_cati();
  test_cati_c();
  test_cati_lui();
}
