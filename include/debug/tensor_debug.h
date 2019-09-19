/**
 * @file include/debug/tensor_debug.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_TENSOR_DEBUG_H__
#define __MCUBE_TENSOR_DEBUG_H__

#ifndef __ASSEMBLY__

#if CONFIG_OPTION_DEBUG

/**
 * @fn static inline void print_2d_tensor(unsigned long *tensor, int t1, int t2)
 * @brief print 2D tensor.
 *
 * @param tensor Tensor.
 * @param t1 Number of elements in the first tensor.
 * @param t2 Number of elements in the second tensor.
 */
static inline void print_2d_tensor(unsigned long *tensor, int t1, int t2)
{
  for (int i1 = 0; i1 < t1; i1++) {
    for (int i2 = 0; i2 < t2; i2++) {
      print("tensor[%d][%d] = %lu\n", i1, i2, tensor[i1 * t2 + i2]);
    }
  }
}


/**
 * @def pdebug_2d_tensor(tensor, t1, t2)
 * @brief Print debug for 2D tensor.
 *
 * @param tensor Tensor.
 * @param t1 Number of elements in the first tensor.
 * @param t2 Number of elements in the second tensor.
 */
#define pdebug_2d_tensor(tensor, t1, t2) do {                           \
    PDEBUG_WHERE();                                                     \
    print_2d_tensor();                                                  \
  } while (0)


/**
 * @fn static inline void print_3d_tensor(unsigned long *tensor, int t1, int t2,
 *                                        int t3)
 * @brief print 3D tensor.
 *
 * @param tensor Tensor.
 * @param t1 Number of elements in the first tensor.
 * @param t2 Number of elements in the second tensor.
 * @param t3 Number of elements in the third tensor.
 */
static inline void print_3d_tensor(unsigned long *tensor, int t1, int t2,
                                   int t3)
{
  for (int i1 = 0; i1 < t1; i1++) {
    for (int i2 = 0; i2 < t2; i2++) {
      for (int i3 = 0; i3 < t3; i3++) {
        print("tensor[%d][%d][%d] = %lu\n", i1, i2, i3,
              tensor[i1 * t2 * t3 + i2 * t3 + i3]);
      }
    }
  }
}


/**
 * @def pdebug_3d_tensor(tensor, t1, t2, t3)
 * @brief Print debug for 3D tensor.
 *
 * @param tensor Tensor.
 * @param t1 Number of elements in the first tensor.
 * @param t2 Number of elements in the second tensor.
 * @param t3 Number of elements in the third tensor.
 */
#define pdebug_3d_tensor(tensor, t1, t2, t3) do {                       \
    PDEBUG_WHERE();                                                     \
    print_3d_tensor(tensor, t1, t2, t3);                                \
  } while (0)

#else
/**
 * @def pdebug_2d_tensor(tensor, t1, t2)
 * @brief Print debug for 2D tensor.
 *
 * @param tensor Tensor.
 * @param t1 Number of elements in the first tensor.
 * @param t2 Number of elements in the second tensor.
 */
#define pdebug_2d_tensor(tensor, t1, t2)

/**
 * @def pdebug_3d_tensor(tensor, t1, t2, t3)
 * @brief Print debug for 3D tensor.
 *
 * @param tensor Tensor.
 * @param t1 Number of elements in the first tensor.
 * @param t2 Number of elements in the second tensor.
 * @param t3 Number of elements in the third tensor.
 */
#define pdebug_3d_tensor(tensor, t1, t2, t3)
#endif /* CONFIG_OPTION_DEBUG */

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_TENSOR_DEBUG_H__ */
