/**
 * @file include/mcube/errno_base.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_ERRNO_BASE_H__
#define __MCUBE_MCUBE_ERRNO_BASE_H__

/**
 * @def EPERM
 * @brief Operation not permitted.
 */
#define EPERM 1

/**
 * @def ENOENT
 * @brief No such file or directory.
 */
#define ENOENT 2

/**
 * @def ESRCH
 * @brief No such process.
 */
#define ESRCH 3

/**
 * @def EINTR
 * @brief Interrupted system call.
 */
#define EINTR 4

/**
 * @def EIO
 * @brief I/O error.
 */
#define EIO 5

/**
 * @def ENXIO
 * @brief No such device or address.
 */
#define ENXIO 6

/**
 * @def E2BIG
 * @brief Argument list too long.
 */
#define E2BIG 7

/**
 * @def ENOEXEC
 * @brief Exec format error.
 */
#define ENOEXEC 8

/**
 * @def EBADF
 * @brief Bad file number.
 */
#define EBADF 9

/**
 * @def ECHILD
 * @brief No child processes.
 */
#define ECHILD 10

/**
 * @def EAGAIN
 * @brief Try again.
 */
#define EAGAIN 11

/**
 * @def ENOMEM
 * @brief Out of memory.
 */
#define ENOMEM 12

/**
 * @def EACCES
 * @brief Permission denied.
 */
#define EACCES 13

/**
 * @def EFAULT
 * @brief Bad address.
 */
#define EFAULT 14

/**
 * @def ENOTBLK
 * @brief Block device required.
 */
#define ENOTBLK 15

/**
 * @def EBUSY
 * @brief Device or resource busy.
 */
#define EBUSY 16

/**
 * @def EEXIST
 * @brief File exists.
 */
#define EEXIST 17

/**
 * @def EXDEV
 * @brief Cross-device link.
 */
#define EXDEV 18

/**
 * @def ENODEV
 * @brief No such device.
 */
#define ENODEV 19

/**
 * @def ENOTDIR
 * @brief Not a directory.
 */
#define ENOTDIR 20

/**
 * @def EISDIR
 * @brief Is a directory.
 */
#define EISDIR 21

/**
 * @def EINVAL
 * @brief Invalid argument.
 */
#define EINVAL 22

/**
 * @def ENFILE
 * @brief File table overflow.
 */
#define ENFILE 23

/**
 * @def EMFILE
 * @brief Too many open files.
 */
#define EMFILE 24

/**
 * @def ENOTTY
 * @brief Not a typewriter.
 */
#define ENOTTY 25

/**
 * @def ETXTBSY
 * @brief Text file busy.
 */
#define ETXTBSY 26

/**
 * @def EFBIG
 * @brief File too large.
 */
#define EFBIG 27

/**
 * @def ENOSPC
 * @brief No space left on device.
 */
#define ENOSPC 28

/**
 * @def ESPIPE
 * @brief Illegal seek.
 */
#define ESPIPE 29

/**
 * @def EROFS
 * @brief Read-only file system.
 */
#define EROFS 30

/**
 * @def EMLINK
 * @brief Too many links.
 */
#define EMLINK 31

/**
 * @def EPIPE
 * @brief Broken pipe.
 */
#define EPIPE 32

/**
 * @def EDOM
 * @brief Math argument out of domain of func.
 */
#define EDOM 33

/**
 * @def ERANGE
 * @brief Math result not representable.
 */
#define ERANGE 34


#endif  /* __MCUBE_MCUBE_ERRNO_BASE_H__ */
