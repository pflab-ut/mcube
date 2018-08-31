/**
 * @file user/axis/user_axis.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


#define PARALLEL_NUM 8
#define CLUSTER_NUM 8

//#define PARALLEL_NUM 4
//#define CLUSTER_NUM 1

//#define PARALLEL_NUM 2
//#define CLUSTER_NUM 2

#define LOOP_PER_CLUSTER (PARALLEL_NUM / CLUSTER_NUM)

#define MAT_SIZE 2
//#define MAT_SIZE 4
//#define MAT_SIZE 8

struct cluster own;

volatile unsigned long array[PARALLEL_NUM][MAT_SIZE][MAT_SIZE];

volatile unsigned long src[PARALLEL_NUM][MAT_SIZE][MAT_SIZE];
volatile unsigned long src2[PARALLEL_NUM][MAT_SIZE][MAT_SIZE];

void print_mat(void)
{
  int i, j, k;
  for (i = 0; i < PARALLEL_NUM; i++) {
    for (j = 0; j < MAT_SIZE; j++) {
      for (k = 0; k < MAT_SIZE; k++) {
        printk("print_mat() array[%d][%d][%d] = %lu\n", i, j, k, array[i][j][k]);
      }
    }
  }
}



#define INIT_NUM 10

#define INIT_VAL(i, j, k) (INIT_NUM * (i) + ((j) * INIT_NUM / 10) + k)


void init_array(void)
{
  int i, j, k;
  for (i = 0; i < PARALLEL_NUM; i++) {
    for (j = 0; j < MAT_SIZE; j++) {
      for (k = 0; k < MAT_SIZE; k++) {
        array[i][j][k] = INIT_VAL(i, j, k);
        src[i][j][k] = INIT_VAL(i, j, k);
        src2[i][j][k] = INIT_VAL(i, j, k);
      }
    }
  }
}

void init_src_array(void)
{
  int i, j, k;
  for (i = 0; i < PARALLEL_NUM; i++) {
    for (j = 0; j < MAT_SIZE; j++) {
      for (k = 0; k < MAT_SIZE; k++) {
        array[i][j][k] = 0;
        src[i][j][k] = INIT_VAL(i, j, k);
        src2[i][j][k] = INIT_VAL(i, j, k);
      }
    }
  }
}



void mul(volatile unsigned long dst[MAT_SIZE][MAT_SIZE],
         volatile unsigned long a[MAT_SIZE][MAT_SIZE],
         volatile unsigned long b[MAT_SIZE][MAT_SIZE])
{
  int i, j, k;
  for (i = 0; i < MAT_SIZE; i++) {
    for (k = 0; k < MAT_SIZE; k++) {
      for (j = 0; j < MAT_SIZE; j++) {
        dst[i][j] = (dst[i][j] + a[i][k] * b[k][j]);
      }
    }
  }
}


void wait_until_loop_completion(void)
{
  int loop;
  int i;
  do {
    loop = 0;
    for (i = 0; i < PARALLEL_NUM; i++) {
      if (array[i][MAT_SIZE-1][MAT_SIZE-1] == INIT_VAL(i, MAT_SIZE - 1, MAT_SIZE - 1)) {
        loop = 1;
      }
    }
  } while (loop == 1);
}


//#define PUSH_TO_SLAVE_CLUSTERS
#define PULL_FROM_MASTER_CLUSTER



void do_sequential(void)
{
  int i;
	unsigned long cpu_id;
  unsigned long begin, end;
  printk("do_sequential()\n");
  cpu_id = get_cpu_id();
	set_cpu_id(&own, cpu_id);
  //  printk("own.x = %d own.y = %d\n", own.x, own.y);
  if (own.x == 0 && own.y == 0) {
    init_array();
    begin = get_time_stamp_counter();
    
    for (i = 0; i < PARALLEL_NUM; i++) {
      mul(array[i], src[i], src2[i]);
    }
    end = get_time_stamp_counter();
    printk("begin = %lu end = %lu end - begin = %lu\n", begin, end, end - begin);
    //    print_mat();
  }
}

void do_callback(volatile int index)
{
	volatile unsigned long high_addr;
	volatile unsigned long low_addr;
  volatile int i, j;
  
#if defined(PUSH_TO_SLAVE_CLUSTERS)
  /* set address array[index] in (0, 0) */
  //  printk("do %d\n", index);
  //  printk("do_callback(): do while (%d, %d)\n", own.x, own.y);
  /* wait until array[own.cluster_id] in (own.x, own.y) is INIT_NUM. */
  do {
    //    read(own.local_cpu_id, high_addr, low_addr, &data, 0);
#if 0
    printk("do_callback(): array[%d][%d][%d] = %d\n",
           index, MAT_SIZE - 1, MAT_SIZE - 1, array[index][MAT_SIZE-1][MAT_SIZE-1]);
#endif
  } while (array[index][MAT_SIZE-1][MAT_SIZE-1] == 0);
  //  printk("start (%d, %d)\n", own.x, own.y);
  /* calculate array[own.cluster_id] in (own.x, own.y) */
  mul(array[index], src[index], src2[index]);
  //  printk("write\n");
  for (i = 0; i < MAT_SIZE; i++) {
    //    printk("i = %d\n", i);
    for (j = 0; j < MAT_SIZE; j++) {
      //      printk("j = %d\n", j);
      /* send array[index] in (own.x, own.y) to that in (0, 0) */
      encode_cluster_address(&high_addr, &low_addr,
                             0, 0, (unsigned long) &array[index][i][j]);
      write_to_cluster(own.local_cpu_id, high_addr, low_addr, &array[index][i][j], 0);
    }
  }
  //  printk("write end\n");
#elif defined(PULL_FROM_MASTER_CLUSTER)
  printk("do %d\n", index);
  
  for (i = 0; i < MAT_SIZE; i++) {
    for (j = 0; j < MAT_SIZE; j++) {
      /* set array[own.cluster_id] in (0, 0) */
      encode_cluster_address(&high_addr, &low_addr,
                             0, 0, (unsigned long) &array[index][i][j]);
      //      printk("high_addr = 0x%x low_addr = 0x%x\n", high_addr, low_addr);
      /* wait until array[own.cluster_id] is INIT_NUM. */
      //      printk("do_callback(): (%d, %d) %d\n", own.x, own.y, own.local_cpu_id);
      do {
        // delay
        // not work well if printk is removed.
        //        printk("");
        /*
        if (index == 6) {
          //          asm volatile("sync");
          printk("array[%d][%d][%d] = %d\n", index, i, j, array[index][i][j]);
        }
        */
        
        /* read array[index] from (0, 0) */
        read_from_cluster(own.local_cpu_id, high_addr, low_addr, &array[index][i][j], 0);
        //        printk("array[%d][%d][%d] = %d\n", index, i, j, array[index][i][j]);
      } while (array[index][i][j] == 0);
    }
  }
  //  printk("start (%d, %d)\n", own.x, own.y);
  mul(array[index], src[index], src2[index]);
  printk("write\n");
  for (i = 0; i < MAT_SIZE; i++) {
    for (j = 0; j < MAT_SIZE; j++) {
      /* set array[own.cluster_id] in (0, 0) */
      encode_cluster_address(&high_addr, &low_addr,
                             0, 0, (unsigned long) &array[index][i][j]);
      /* send array[index] to (0, 0) */
      write_to_cluster(own.local_cpu_id, high_addr, low_addr, &array[index][i][j], 0);
    }
  }
  
#else
#error "Unknown Communication Policy"
#endif
}

int is_cluster_active(struct cluster *c)
{
  if (c->cluster_id < CLUSTER_NUM) {
    return 1;
  }
  return 0;
}


void do_parallel(void)
{
	unsigned long cpu_id;
  int i;
  unsigned long begin, end;
  cpu_id = get_cpu_id();
  //  printk("cpu_id = 0x%x\n", cpu_id);
	set_cpu_id(&own, cpu_id);
  //  printk("own.x = %d own.y = %d\n", own.x, own.y);
  if (own.x == 0 && own.y == 0) {
    printk("begin\n");
    init_array();
    /* initialize array data as INIT_VAL. */
    begin = get_time_stamp_counter();
#if defined(PUSH_TO_SLAVE_CLUSTERS)
    /* send data to other clusters (push) */
    for (i = LOOP_PER_CLUSTER; i < PARALLEL_NUM; i++) {
      unsigned long high_addr;
      unsigned long low_addr;
      struct cluster dst;
      int j, k;
      printk("%d / %d = %d\n", i, LOOP_PER_CLUSTER, i / LOOP_PER_CLUSTER);
      get_cluster_from_index(&dst, i / LOOP_PER_CLUSTER, 0);
      printk("do_parallel(): dst.x = %lu dst.y = %lu\n", dst.x, dst.y);
      for (j = 0; j < MAT_SIZE; j++) {
        for (k = 0; k < MAT_SIZE; k++) {
          printk("do_parallel(): array[%d][%d][%d] = %lu\n", i, j, k, array[i][j][k]);
          encode_cluster_address(&high_addr, &low_addr,
                                 dst.x, dst.y, (unsigned long) &array[i][j][k]);
          write_to_cluster(dst.local_cpu_id, high_addr, low_addr, &array[i][j][k], 0);
        }
      }
    }
#endif

    //    printk("mul\n");
    for (i = 0; i < LOOP_PER_CLUSTER; i++) {
      /* increment own array data */
      mul(array[own.cluster_id * LOOP_PER_CLUSTER + i],
          src[own.cluster_id * LOOP_PER_CLUSTER + i],
          src2[own.cluster_id * LOOP_PER_CLUSTER + i]);
    }
    
    //    printk("do while (0, 0)\n");
    /* wait until all values of array are updated values. */
    wait_until_loop_completion();
    
    //    printk("end\n");
    end = get_time_stamp_counter();
    printk("begin = %lu end = %lu end - begin = %lu\n", begin, end, end - begin);
    printk("MAT_SIZE = %d PARALLEL_NUM = %d CLUSTER_NUM = %d\n",
           MAT_SIZE, PARALLEL_NUM, CLUSTER_NUM);
  } else if (is_cluster_active(&own)) {
    init_src_array();
    for (i = 0; i < LOOP_PER_CLUSTER; i++) {      
      do_callback(own.cluster_id * LOOP_PER_CLUSTER + i);
    }
  }
}



void cluster_main(void)
{
  //  do_sequential();
  do_parallel();
}

