/**
 * @file include/mcube/unistd.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_UNISTD_H__
#define __MCUBE_MCUBE_UNISTD_H__


#ifndef __ASSEMBLY__

#if !CONFIG_ARCH_SIM

int link(const char *oldpath, const char *newpath);
int unlink(const char *pathname);
ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);

#endif /* !CONFIG_ARCH_SIM */


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_UNISTD_H__ */
