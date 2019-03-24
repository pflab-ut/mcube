/**
 * @file user/axis/user_axis.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>
#include "user_axis.h"

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

static void print_mat(void)
{
  int i, j, k;

  for (i = 0; i < PARALLEL_NUM; i++) {
    for (j = 0; j < MAT_SIZE; j++) {
      for (k = 0; k < MAT_SIZE; k++) {
        print("print_mat() array[%d][%d][%d] = %lu\n", i, j, k, array[i][j][k]);
      }
    }
  }
}



#define INIT_NUM 10

#define INIT_VAL(i, j, k) (unsigned long) (INIT_NUM * (i) + ((j) * INIT_NUM / 10) + k)


static void init_array(void)
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

static void init_src_array(void)
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



static void do_mul(volatile unsigned long dst[MAT_SIZE][MAT_SIZE],
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


static void wait_until_loop_completion(void)
{
  int loop;
  int i;

  do {
    loop = 0;

    for (i = 0; i < PARALLEL_NUM; i++) {
      if (array[i][MAT_SIZE - 1][MAT_SIZE - 1] == INIT_VAL(i, MAT_SIZE - 1,
                                                           MAT_SIZE - 1)) {
        loop = 1;
      }
    }
  } while (loop == 1);
}


#define PUSH_TO_SLAVE_CLUSTERS
//#define PULL_FROM_MASTER_CLUSTER



static void do_sequential(void)
{
  int i;
  unsigned long cpu_id;
  unsigned long begin, end;
  print("do_sequential()\n");
  cpu_id = get_cluster_and_cpu_ids();
  set_cpu_id(&own, cpu_id);

  //  print("own.x = %d own.y = %d\n", own.x, own.y);
  if (own.x == 0 && own.y == 0) {
    init_array();
    begin = get_time_stamp_counter();

    for (i = 0; i < PARALLEL_NUM; i++) {
      do_mul(array[i], src[i], src2[i]);
    }

    end = get_time_stamp_counter();
    print("begin = %lu end = %lu end - begin = %lu\n", begin, end, end - begin);
    print_mat();
  }
}

static void do_callback(volatile int index)
{
  volatile unsigned long high_addr;
  volatile unsigned long low_addr;
  volatile int i, j;

#if defined(PUSH_TO_SLAVE_CLUSTERS)

  /* set address array[index] in (0, 0) */
  //  print("do %d\n", index);
  //  print("do_callback(): do while (%d, %d)\n", own.x, own.y);
  /* wait until array[own.cluster_id] in (own.x, own.y) becomes zero. */
  do {
    //    read(own.local_cpu_id, high_addr, low_addr, &data, 0);
#if 0
    print("do_callback(): array[%d][%d][%d] = %d\n",
          index, MAT_SIZE - 1, MAT_SIZE - 1, array[index][MAT_SIZE - 1][MAT_SIZE - 1]);
#endif
  } while (array[index][MAT_SIZE - 1][MAT_SIZE - 1] == 0);

  //  print("start (%d, %d)\n", own.x, own.y);
  /* calculate array[own.cluster_id] in (own.x, own.y) */
  do_mul(array[index], src[index], src2[index]);

  //  print("write\n");
  for (i = 0; i < MAT_SIZE; i++) {
    //    print("i = %d\n", i);
    for (j = 0; j < MAT_SIZE; j++) {
      //      print("j = %d\n", j);
      /* send array[index] in (own.x, own.y) to that in (0, 0) */
      encode_cluster_address(&high_addr, &low_addr,
                             0, 0, (unsigned long) &array[index][i][j]);
      write_to_cluster(own.local_cpu_id, high_addr, low_addr, &array[index][i][j], 0);
    }
  }

  //  print("write end\n");
#elif defined(PULL_FROM_MASTER_CLUSTER)
  print("do %d\n", index);

  for (i = 0; i < MAT_SIZE; i++) {
    for (j = 0; j < MAT_SIZE; j++) {
      /* set array[own.cluster_id] in (0, 0) */
      encode_cluster_address(&high_addr, &low_addr,
                             0, 0, (unsigned long) &array[index][i][j]);

      //      print("high_addr = 0x%x low_addr = 0x%x\n", high_addr, low_addr);
      /* wait until array[own.cluster_id] becomes zero. */
      //      print("do_callback(): (%d, %d) %d\n", own.x, own.y, own.local_cpu_id);
      do {

        /* read array[index] from (0, 0) */
        read_from_cluster(own.local_cpu_id, high_addr, low_addr, &array[index][i][j],
                          0);
        //        print("array[%d][%d][%d] = %d\n", index, i, j, array[index][i][j]);
      } while (array[index][i][j] == 0);
    }
  }

  //  print("start (%d, %d)\n", own.x, own.y);
  do_mul(array[index], src[index], src2[index]);
  print("write\n");

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

static int is_cluster_active(struct cluster *c)
{
  if (c->cluster_id < CLUSTER_NUM) {
    return 1;
  }

  return 0;
}


static void do_parallel(void)
{
  unsigned long cpu_id;
  int i;
  unsigned long begin, end;
  cpu_id = get_cluster_and_cpu_ids();
  //  print("cpu_id = 0x%x\n", cpu_id);
  set_cpu_id(&own, cpu_id);

  //  print("own.x = %d own.y = %d\n", own.x, own.y);
  if (own.x == 0 && own.y == 0) {
    print("begin\n");
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
      print("%d / %d = %d\n", i, LOOP_PER_CLUSTER, i / LOOP_PER_CLUSTER);
      get_cluster_from_index(&dst, i / LOOP_PER_CLUSTER, 0);
      print("do_parallel(): dst.x = %lu dst.y = %lu\n", dst.x, dst.y);

      for (j = 0; j < MAT_SIZE; j++) {
        for (k = 0; k < MAT_SIZE; k++) {
          print("do_parallel(): array[%d][%d][%d] = %lu\n", i, j, k, array[i][j][k]);
          encode_cluster_address(&high_addr, &low_addr,
                                 dst.x, dst.y, (unsigned long) &array[i][j][k]);
          write_to_cluster(dst.local_cpu_id, high_addr, low_addr, &array[i][j][k], 0);
        }
      }
    }

#endif

    //    print("mul\n");
    for (i = 0; i < LOOP_PER_CLUSTER; i++) {
      /* increment own array data */
      do_mul(array[own.cluster_id * LOOP_PER_CLUSTER + i],
             src[own.cluster_id * LOOP_PER_CLUSTER + i],
             src2[own.cluster_id * LOOP_PER_CLUSTER + i]);
    }

    //    print("do while (0, 0)\n");
    /* wait until all values of array are updated values. */
    wait_until_loop_completion();

    //    print("end\n");
    end = get_time_stamp_counter();
    print("begin = %lu end = %lu end - begin = %lu\n", begin, end, end - begin);
    print("MAT_SIZE = %d PARALLEL_NUM = %d CLUSTER_NUM = %d\n",
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
  do_sequential();
  do_parallel();
}

