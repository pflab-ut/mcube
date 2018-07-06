/*
 *            multiboot.c
 * written by Shinichi Awamoto, 2017
 * 
 * multiboot specification関連の関数の定義
 */

#include "multiboot2.h"
#include "common.h"

// TODO check multiboot2 magic number

// multiboot_info について
// multiboot_info にはentryの直後でebxレジスタが代入されている。
// この値は本来32bit物理アドレスだが、現在は1GBをストレートマップ
// しているため、1GB以下の空間については物理＝リニア＝論理とみなせる。
// かつ、64bitのうちの上位32bitについては0で埋めたとみなせば、
// 期待通りアクセスできる。
// よって、uint32_tをポインタ(64bit)にキャストしても正常に動作する。

// コードはmultiboot specification 2 仕様書より

uint32_t multiboot_info;

// ACPI RSDPテーブルを取得するための関数
// 戻り値：見つからなかった場合はNULLを返す
struct rsdp_descriptor *multiboot_get_rsdp_desc() {

  for (struct multiboot_tag *tag = (struct multiboot_tag *)(uint64_t)(multiboot_info + 8); tag->type != MULTIBOOT_TAG_TYPE_END; tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag + ((tag->size + 0b111) & ~0b111))) {
    switch(tag->type) {
    case MULTIBOOT_TAG_TYPE_ACPI_OLD:
      {
        struct multiboot_tag_old_acpi *acpi = (struct multiboot_tag_old_acpi *)tag;
        return (struct rsdp_descriptor *)acpi->rsdp;
      }
      break;
    case MULTIBOOT_TAG_TYPE_ACPI_NEW:
      {
        struct multiboot_tag_new_acpi *acpi = (struct multiboot_tag_new_acpi *)tag;
        return (struct rsdp_descriptor *)acpi->rsdp;
      }
      break;
    default:
      break;
    }
  }

  return NULL;
}

// framebuffer tagの取得
// 戻り値：見つからなかった場合はNULLを返す
struct multiboot_tag_framebuffer_common *multiboot_get_framebuffer() {
  for (struct multiboot_tag *tag = (struct multiboot_tag *)(uint64_t)(multiboot_info + 8); tag->type != MULTIBOOT_TAG_TYPE_END; tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag + ((tag->size + 0b111) & ~0b111))) {
    switch(tag->type) {
    case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
      {
        return (struct multiboot_tag_framebuffer_common *)tag;
      }
      break;
    default:
      break;
    }
  }

  return NULL;
}
