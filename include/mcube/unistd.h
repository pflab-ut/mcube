/**
 * @file include/mcube/unistd.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_UNISTD_H__
#define __MCUBE_MCUBE_UNISTD_H__


#ifndef __ASSEMBLY__

#if !CONFIG_ARCH_SIM

/**
 * @fn int link(const char *oldpath, const char *newpath)
 * @brief create a new link (also known as a hard link) to an existing file.
 * If newpath exists, it will not be overwritten.
 * @param oldpath Old path.
 * @param newpath New path.
 * @return On success, zero is returned.
 * On error, -1 is returned, and errno is set appropriately.
 */
int link(const char *oldpath, const char *newpath);

/**
 * @fn int unlink(const char *pathname)
 * @brief delete a name from the filesystem.
 * If that name was the last link to a file and no processes have the file open,
 * the file is deleted and the space it was using is  made available for reuse.
 * If the name was the last link to a file but any processes still have the file open,
 * the file will remain in existence until the last file descriptor
 * referring to it is closed.
 * If the name referred to a symbolic link, the link is removed.
 * If the name referred to a socket, FIFO, or device, the name
 * for it is removed but processes which have the object open may continue to use it.
 *
 * @param pathname Path name.
 * @return On success, zero is returned.
 * On error, -1 is returned, and errno is set appropriately.
 */
int unlink(const char *pathname);

/**
 * @fn ssize_t read(int fd, void *buf, size_t count)
 * @brief attempt to read up to @a count bytes from file descriptor @a fd
 * into the buffer starting at @a buf.
 *
 * @param fd File descriptor.
 * @param buf Buffer.
 * @param count Count bytes.
 * @return On success, the number of bytes read is returned (zero indicates end of file),
 * and the file position is advanced by this number.
 * It is not an error if this number is smaller than the number of bytes requested;
 * this may happen for example because fewer bytes are actually available right now
 * (maybe because we were close to end-of-file, or because we are reading from a pipe,
 * or from a terminal), or because read() was interrupted by a signal.
 */
ssize_t read(int fd, void *buf, size_t count);

/**
 * @fn ssize_t write(int fd, const void *buf, size_t count)
 * @brief write up to @a count bytes from the buffer starting at @a buf
 * to the file referred to by the file descriptor @a fd.
 *
 * @param fd File descriptor.
 * @param buf Buffer.
 * @param count Count bytes.
 */
ssize_t write(int fd, const void *buf, size_t count);

/**
 * @fn int close(int fd)
 * @brief close a file descriptor, so that it no longer refers to any file and may be reused.
 *
 * @param fd File descriptor.
 * @return On success, zero is returned.
 * On error, -1 is returned, and errno is set appropriately.
 */
int close(int fd);

#endif /* !CONFIG_ARCH_SIM */


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_UNISTD_H__ */
