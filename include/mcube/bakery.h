/**
 * @file include/mcube/bakery.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_BAKERY_H__
#define __MCUBE_MCUBE_BAKERY_H__

#ifndef __ASSEMBLY__

/**
 * @fn void bakery_lock(int thid)
 * @brief lock by Lamport's bakery algorithm.
 *
 * @param thid Thread ID.
 */
void bakery_lock(int thid);

/**
 * @fn void bakery_unlock(int thid)
 * @brief unlock by Lamport's bakery algorithm.
 *
 * @param thid Thread ID.
 */
void bakery_unlock(int thid);


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_BAKERY_H__ */
