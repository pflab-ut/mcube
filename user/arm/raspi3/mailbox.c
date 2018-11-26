/**
 * @file user/arm/raspi3/mailbox.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

#include "homer.c"

void raspi3_mailbox_main(void)
{
  print("get_serial_number() = 0x%016lu\n", get_serial_number());
  print("random number = %u\n", rand_raspi3(0, UINT_MAX));
}


void raspi3_frame_buffer_main(void)
{
  struct frame_buffer fb = {
    .width = 1024,
    .height = 768,
    .virtual_width = 1024,
    .virtual_height = 768,
    .depth = 32,
    .state = PIXEL_ORDER_STATE_RGB,
    .pointer = 4096,
    .size = 0,
    .pitch = 0
  };
  print("raspi3_frame_buffer_main()\n");
  init_frame_buffer(&fb);
  //  fb_show_picture(homer_data, homer_width, homer_height);
  fb_print(10, 5, "Hello World!");
}

void raspi3_sd_main(void)
{
#if CONFIG_FS_FAT
  unsigned int cluster;
  // initialize EMMC and detect SD card type
  if (init_sd() == SD_OK) {
    // read the master boot record after our bss segment
#if 0
    extern unsigned char __end;
    if (sd_readblock(0, &__end, 1)) {
      // dump it to serial console
      memdump(&__end, 512);
    }
#endif
    // read the master boot record and find our partition
    if (fat_getpartition()) {
      /* XXX: fat_getcluster() does not work well if fat_listdirectory() is called. */
      // list root directory entries
      //      fat_listdirectory();
      /* find out file in root directory entries */
      cluster = fat_getcluster("HOGE    TXT");
      if (cluster) {
        // read into memory
        memdump(fat_readfile(cluster), 512);
      }
    } else {
      printk("FAT partition not found???\n");
    }
  }
#endif /* CONFIG_FS_FAT */
}

void raspi3_int_main(void)
{
  unsigned long cpu = get_cpu_id();
  int i;
  printk("raspi3_int_main()\n");
  printk("cpu = %d\n", cpu);

  for (i = 0; i < 4; i++) {
    mmio_out32(LP_CORE_MAILBOXES_INTERRUPT_CTRL(i), 0xf);
  }

  //  mmio_out32(LP_CORE0_MAILBOX0_WRITE_SET, 0x1);
  //  mmio_out32(LP_CORE0_MAILBOX1_WRITE_SET, 0x1);
  //  mmio_out32(LP_CORE0_MAILBOX2_WRITE_SET, 0x1);
  //  mmio_out32(LP_CORE0_MAILBOX3_WRITE_SET, 0x1);

  mmio_out32(LP_CORE_MAILBOX_WRITE_SET(1, 0), 0x1);
  //  mmio_out32(LP_CORE1_MAILBOX1_WRITE_SET, 0x1);
  //  mmio_out32(LP_CORE1_MAILBOX2_WRITE_SET, 0x1);
  //  mmio_out32(LP_CORE1_MAILBOX3_WRITE_SET, 0x1);
}


int user_raspi3_main(void)
{
  //  raspi3_mailbox_main();
  //  raspi3_frame_buffer_main();
  //  raspi3_sd_main();
  raspi3_int_main();
  return 0;
}
