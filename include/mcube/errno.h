/**
 * @file include/mcube/errno.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_ERRNO_H__
#define __MCUBE_MCUBE_ERRNO_H__


/*
 * The historical Unix errno 1->34 Range:
 */

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
 * @def EWOULDBLOCK
 * @brief Operation would block.
 */
#define EWOULDBLOCK EAGAIN

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
#define EXDEV  18

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
 * @brief Text file busy */
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
 * @brief Math result not representable (too large).
 */
#define ERANGE 34

/**
 * @def EDEADLK
 * @brief Resource deadlock would occur.
 */
#define EDEADLK 35

/**
 * @def EDEADLOCK
 * @brief Resource deadlock would occur.
 */
#define EDEADLOCK EDEADLK

/**
 * @def ENAMETOOLONG
 * @brief File name too long.
 */
#define ENAMETOOLONG 36

/**
 * @def ENOLCK
 * @brief No record locks available.
 */
#define ENOLCK 37

/**
 * @def ENOSYS
 * @brief Function not implemented.
 */
#define ENOSYS 38

/**
 * @def ENOTEMPTY
 * @brief Directory not empty.
 */
#define ENOTEMPTY 39

/**
 * @def ELOOP
 * @brief Too many symbolic links encountered.
 */
#define ELOOP 40


/**
 * @def ENOMSG
 * @brief No message of desired type.
 */
#define ENOMSG 42

/**
 * @def EIDRM
 * @brief Identifier removed.
 */
#define EIDRM 43

/**
 * @def ECHRNG
 * @brief Channel number out of range.
 */
#define ECHRNG 44

/**
 * @def EL2NSYNC
 * @brief Level 2 not synchronized.
 */
#define EL2NSYNC 45

/**
 * @def EL3HLT
 * @brief Level 3 halted.
 */
#define EL3HLT 46

/**
 * @def EL3RST
 * @brief Level 3 reset.
 */
#define EL3RST 47

/**
 * @def ELNRNG
 * @brief Link number out of range.
 */
#define ELNRNG 48

/**
 * @def EUNATCH
 * @brief Protocol driver not attached.
 */
#define EUNATCH 49

/**
 * @def ENOCSI
 * @brief No CSI structure available.
 */
#define ENOCSI 50

/**
 * @def EL2HLT
 * @brief Level 2 halted.
 */
#define EL2HLT 51

/**
 * @def EBADE
 * @brief Invalid exchange.
 */
#define EBADE 52

/**
 * @def EBADR
 * @brief Invalid request descriptor.
 */
#define EBADR 53

/**
 * @def EXFULL
 * @brief Exchange full.
 */
#define EXFULL 54

/**
 * @def ENOANO
 * @brief No anode.
 */
#define ENOANO 55

/**
 * @def EBADRQC
 * @brief Invalid request code.
 */
#define EBADRQC 56

/**
 * @def EBADSLT
 * @brief Invalid slot.
 */
#define EBADSLT 57

/**
 * @def EBFONT
 * @brief Bad font file format.
 */
#define EBFONT 59

/**
 * @def ENOSTR
 * @brief Device not a stream.
 */
#define ENOSTR 60

/**
 * @def ENODATA
 * @brief No data available.
 */
#define ENODATA 61

/**
 * @def ETIME
 * @brief Timer expired.
 */
#define ETIME 62

/**
 * @def ENOSR
 * @brief Out of streams resources.
 */
#define ENOSR 63

/**
 * @def ENONET
 * @brief Machine is not on the network.
 */
#define ENONET 64

/**
 * @def ENOPKG
 * @brief Package not installed.
 */
#define ENOPKG 65

/**
 * @def EREMOTE
 * @brief Object is remote.
 */
#define EREMOTE 66

/**
 * @def ENOLINK
 * @brief Link has been severed.
 */
#define ENOLINK 67

/**
 * @def EADV
 * @brief Advertise error.
 */
#define EADV 68

/**
 * @def ESRMNT
 * @brief Srmount error.
 */
#define ESRMNT 69

/**
 * @def ECOMM
 * @brief Communication error on send.
 */
#define ECOMM 70

/**
 * @def EPROTO
 * @brief Protocol error.
 */
#define EPROTO 71

/**
 * @def EMULTIHOP
 * @brief Multihop attempted.
 */
#define EMULTIHOP 72

/**
 * @def EDOTDOT
 * @brief RFS specific error.
 */
#define EDOTDOT 73

/**
 * @def EBADMSG
 * @brief Not a data message.
 */
#define EBADMSG 74

/**
 * @def EOVERFLOW
 * @brief Value too large for defined data type.
 */
#define EOVERFLOW 75

/**
 * @def ENOTUNIQ
 * @brief Name not unique on network.
 */
#define ENOTUNIQ 76

/**
 * @def EBADFD
 * @brief File descriptor in bad state.
 */
#define EBADFD 77

/**
 * @def EREMCHG
 * @brief Remote address changed.
 */
#define EREMCHG 78

/**
 * @def ELIBACC
 * @brief Can not access a needed shared library.
 */
#define ELIBACC 79

/**
 * @def ELIBBAD
 * @brief Accessing a corrupted shared library.
 */
#define ELIBBAD 80

/**
 * @def ELIBSCN
 * @brief .lib section in a.out corrupted.
 */
#define ELIBSCN 81

/**
 * @def ELIBMAX
 * @brief Attempting to link in too many shared libraries.
 */
#define ELIBMAX 82

/**
 * @def ELIBEXEC
 * @brief Cannot exec a shared library directly.
 */
#define ELIBEXEC 83

/**
 * @def EILSEQ
 * @brief Illegal byte sequence.
 */
#define EILSEQ 84

/**
 * @def ERESTART
 * @brief Interrupted system call should be restarted.
 */
#define ERESTART 85

/**
 * @def ESTRPIPE
 * @brief Streams pipe error.
 */
#define ESTRPIPE 86

/**
 * @def EUSERS
 * @brief Too many users.
 */
#define EUSERS 87

/**
 * @def ENOTSOCK
 * @brief Socket operation on non-socket.
 */
#define ENOTSOCK 88

/**
 * @def EDESTADDRREQ
 * @brief Destination address required.
 */
#define EDESTADDRREQ 89

/**
 * @def EMSGSIZE
 * @brief Message too long.
 */
#define EMSGSIZE 90

/**
 * @def EPROTOTYPE
 * @brief Protocol wrong type for socket.
 */
#define EPROTOTYPE 91

/**
 * @def ENOPROTOOPT
 * @brief Protocol not available.
 */
#define ENOPROTOOPT 92

/**
 * @def EPROTONOSUPPORT
 * @brief Protocol not supported.
 */
#define EPROTONOSUPPORT 93

/**
 * @def ESOCKTNOSUPPORT
 * @brief Socket type not supported.
 */
#define ESOCKTNOSUPPORT 94

/**
 * @def EOPNOTSUPP
 * @brief Operation not supported on transport endpoint.
 */
#define EOPNOTSUPP 95

/**
 * @def EPFNOSUPPORT
 * @brief Protocol family not supported.
 */
#define EPFNOSUPPORT 96

/**
 * @def EAFNOSUPPORT
 * @brief Address family not supported by protocol.
 */
#define EAFNOSUPPORT 97

/**
 * @def EADDRINUSE
 * @brief Address already in use.
 */
#define EADDRINUSE 98

/**
 * @def EADDRNOTAVAIL
 * @brief Cannot assign requested address.
 */
#define EADDRNOTAVAIL 99

/**
 * @def ENETDOWN
 * @brief Network is down.
 */
#define ENETDOWN 100

/**
 * @def ENETUNREACH
 * @brief Network is unreachable.
 */
#define ENETUNREACH 101

/**
 * @def ENETRESET
 * @brief Network dropped connection because of reset.
 */
#define ENETRESET 102

/**
 * @def ECONNABORTED
 * @brief Software caused connection abort.
 */
#define ECONNABORTED 103

/**
 * @def ECONNRESET
 * @brief Connection reset by peer.
 */
#define ECONNRESET 104

/**
 * @def ENOBUFS
 * @brief No buffer space available.
 */
#define ENOBUFS 105

/**
 * @def EISCONN
 * @brief Transport endpoint is already connected.
 */
#define EISCONN 106

/**
 * @def ENOTCONN
 * @brief Transport endpoint is not connected.
 */
#define ENOTCONN 107

/**
 * @def ESHUTDOWN
 * @brief Cannot send after transport endpoint shutdown.
 */
#define ESHUTDOWN 108

/**
 * @def ETOOMANYREFS
 * @brief Too many references: cannot splice.
 */
#define ETOOMANYREFS 109

/**
 * @def ETIMEDOUT
 * @brief Connection timed out.
 */
#define ETIMEDOUT 110

/**
 * @def ECONNREFUSED
 * @brief Connection refused.
 */
#define ECONNREFUSED 111

/**
 * @def EHOSTDOWN
 * @brief Host is down.
 */
#define EHOSTDOWN 112

/**
 * @def EHOSTUNREACH
 * @brief No route to host.
 */
#define EHOSTUNREACH 113

/**
 * @def EALREADY
 * @brief Operation already in progress.
 */
#define EALREADY 114

/**
 * @def EINPROGRESS
 * @brief Operation now in progress.
 */
#define EINPROGRESS 115

/**
 * @def ESTALE
 * @brief Stale NFS file handle.
 */
#define ESTALE 116

/**
 * @def EUCLEAN
 * @brief Structure needs cleaning.
 */
#define EUCLEAN 117

/**
 * @def ENOTNAM
 * @brief Not a XENIX named type file.
 */
#define ENOTNAM 118

/**
 * @def ENAVAIL
 * @brief No XENIX semaphores available.
 */
#define ENAVAIL 119

/**
 * @def EISNAM
 * @brief Is a named type file.
 */
#define EISNAM 120

/**
 * @def EREMOTEIO
 * @brief Remote I/O error.
 */
#define EREMOTEIO 121

/**
 * @def EDQUOT
 * @brief Quota exceeded.
 */
#define EDQUOT 122

/**
 * @def ENOMEDIUM
 * @brief No medium found.
 */
#define ENOMEDIUM 123

/**
 * @def EMEDIUMTYPE
 * @brief Wrong medium type.
 */
#define EMEDIUMTYPE 124

/**
 * @def ECANCELED
 * @brief Operation Canceled.
 */
#define ECANCELED 125

/**
 * @def ENOKEY
 * @brief Required key not available.
 */
#define ENOKEY 126

/**
 * @def EKEYEXPIRED
 * @brief Key has expired.
 */
#define EKEYEXPIRED 127

/**
 * @def EKEYREVOKED
 * @brief Key has been revoked.
 */
#define EKEYREVOKED 128

/**
 * @def EKEYREJECTED
 * @brief Key was rejected by service.
 */
#define EKEYREJECTED 129


/* for robust mutexes */
/**
 * @def EOWNERDEAD
 * @brief Owner died.
 */
#define EOWNERDEAD 130

/**
 * @def ENOTRECOVERABLE
 * @brief State not recoverable.
 */
#define ENOTRECOVERABLE 131

#ifndef __ASSEMBLY__

#if !CONFIG_ARCH_SIM

/**
 * @var errno
 * @brief Error number.
 */
extern int errno;

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

/**
 * @fn static inline const char *errno_to_str(int err)
 * @brief Descriptions are copied verbatim from the "Single Unix Specification,
 * Version 1", Namely the "X/Open CAE Specification": 'System Interfaces
 * and Headers, Issue 4, Version 2 (1994)'.
 *
 * @param err Error number.
 * @return Error number description.
 */
static inline const char *errno_to_str(int err)
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


#endif /* !CONFIG_ARCH_SIM */

#endif /* !__ASSEMBLY__ */

#endif  /* __MCUBE_MCUBE_ERRNO_H__ */
