/**
 * @file include/x86/errno.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_ERRNO_H__
#define __MCUBE_X86_ERRNO_H__

#ifndef __ASSEMBLY__

/*
 * Standard Unix symbols for Error Numbers (errno)
 *
 * Copyright (C) 2012 Ahmed S. Darwish <darwish.07@gmail.com>
 *
 * "No function in this document sets errno to 0 to indicate an error.
 * Only these symbolic names should be used in programs, since the actual
 * val of the err number is unspecified." --Single UNIX Specv3, POSIX:2001
 *
 * The errno values range 1->34 seems to be standardized _only_ as a his-
 * torical artifact in the UNIX jungle. Even Linus in linux-0.01 wonders
 * about such values origin, .. and just copies them verbatim from Minix.
 *
 * In light of the above, we just copy the _values_ from Linux & BSD :-)
 */

/*
 * Descriptions are copied  verbatim from the "Single Unix Specification,
 * Version 1", Namely the "X/Open CAE Specification": 'System Interfaces
 * and Headers, Issue 4, Version 2 (1994)'.
 */
static inline const char *errno_description(int err)
{
  switch (err) {
  case 0:
    return "Success";
  case -ENOENT:
    return "ENOENT: A component of a specified pathname does not "
           "exist, or the pathname is an empty string";
  case -ENOTDIR:
    return "ENOTDIR: A component of the specified pathname exists, "
           "but it is not a directory, where a directory was "
           "expected";
  case -ENAMETOOLONG:
    return "ENAMETOOLONG: The length of a pathname exceeds "
           "{PATH_MAX}, or a pathname component is longer than "
           "{NAME_MAX}";
  case -EBADF:
    return "EBADF:  A file descriptor argument is out of range, "
           "refers to no open file, or a read (write) request is "
           "made to a file that is only open for writing (reading)";
  case -EFBIG:
    return "EFBIG: The size of a file would exceed the maximum "
           "file size of an implementation";
  case -ENOSPC:
    return "ENOSPC: During the write( ) function on a regular "
           "file or when extending a directory, there is no free "
           "space left on the device";
  default:
    return "Un-stringified error";
  }
}

static inline const char *errno_to_str(int err)
{
  switch(err) {
  case 0:
    return "Success";
  case -EPERM:
    return "EPERM";
  case -EINVAL:
    return "EINVAL";
  case -ENOENT:
    return "ENOENT";
  case -ENOTDIR:
    return "ENOTDIR";
  case -EISDIR:
    return "EISDIR";
  case -ENAMETOOLONG:
    return "ENAMETOOLONG";
  case -EBADF:
    return "EBADF";
  case -EEXIST:
    return "EEXIST";
  case -EFBIG:
    return "EFBIG";
  case -ENOSPC:
    return "ENOSPC";
  case -ESPIPE:
    return "ESPIPE";
  case -EOVERFLOW:
    return "EOVERFLOW";
  default:
    return "Un-stringified";
  }
}

#define errno(err) errno_to_str(err)

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_ERRNO_H__ */
