/**
 * @file include/mcube/string.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_STRING_H__
#define __MCUBE_MCUBE_STRING_H__

#ifndef __ASSEMBLY__

#if !CONFIG_ARCH_SIM

/**
 * @fn void *memset(void *s, int c, size_t n)
 * @brief fills the first @a n bytes of the memory area pointed to by @a s
 * with the constant byte @a c.
 *
 * @param s Memory area.
 * @param c Byte.
 * @param n Bytes.
 * @return pointer to the memory area @a s.
 */
void *memset(void *s, int c, size_t n);

/**
 * @fn void *memset32(void *s, uint32_t c, size_t n)
 * @brief fills the first @a n bytes of the memory area pointed to by @a s
 * with the constant 4-byte @a c.
 *
 * @param s Memory area.
 * @param c Byte.
 * @param n Bytes.
 * @return pointer to the memory area @a s.
 */
void *memset32(void *s, uint32_t c, size_t n);

/**
 * @fn void *memset64(void *s, uint64_t c, size_t n)
 * @brief fills the first @a n bytes of the memory area pointed to by @a s
 * with the constant 8-byte @a c.
 *
 * @param s Memory area.
 * @param c Byte.
 * @param n Bytes.
 * @return pointer to the memory area @a s.
 */
void *memset64(void *s, uint64_t c, size_t n);

/**
 * @fn int memcmp(const void *s1, const void *s2, size_t n)
 * @brief compare the first @a n bytes (each interpreted as unsigned char) of the
 * memory areas @a s1 and @a s2.
 *
 * @param s1 Pointer to @a char.
 * @param s2 Pointer to @a char.
 * @param n Number of bytes comparing @a s1 to @a s2.
 * @return Integer less than, equal to, or greater than zero if the first @a n bytes
 * of @a s1 is found, respectively, to be less than, to match, or be greater than
 * the first @a n bytes of s2.@n
 * For a nonzero return value, the sign is determined by the sign of the difference
 * between the first pair of bytes (interpreted as @c unsigned @c char)
 * that differ in @a s1 and @a s2.
 */
int memcmp(const void *s1, const void *s2, size_t n);

/**
 * @fn void *memcpy(void *dst, const void *src, size_t n)
 * @brief copy @a n bytes from memory area @a src to memory area @a dst.
 * The memory areas must not overlap.
 * @param dst is a pointer to @a void.
 * @param src is a pointer to @a void.
 * @param n is the number fo bytes copying @a src to @a dst.
 * @return Pointer to @a dst.
 */
void *memcpy(void *dst, const void *src, size_t n);


/*
 * memcpy(), minus the checks
 *
 * Sanity checks overhead cannot be tolerated for HOT copying
 * paths like screen scrolling.
 *
 * This is also useful for code implicitly called by panic():
 * a sanity check failure there will lead to a stack overflow.
 */

/**
 * @fn void *memcpy_forward(void *dst, const void *src, size_t len)
 * @brief copy @a n bytes from memory area @a src to memory area @a dst.
 * We do tolerate overlapping regions here if @a src > @a dst. In
 * such case, (@a src - @a dst) must be bigger than movsq's block
 * copy factor: 8 bytes.
 *
 * @param dst is a pointer to @a void.
 * @param src is a pointer to @a void.
 * @param n is the number fo bytes copying @a src to @a dst.
 * @return Pointer to @a dst.
 */
void *memcpy_forward(void *dst, const void *src, size_t len);

/**
 * @fn void *memcpy_forward_nocheck(void *dst, const void *src, size_t len)
 * @brief copy @a n bytes from memory area @a src to memory area @a dst without check.
 * We do tolerate overlapping regions here if @a src > @a dst. In
 * such case, (@a src - @a dst) must be bigger than movsq's block
 * copy factor: 8 bytes.
 *
 * @param dst is a pointer to @a void.
 * @param src is a pointer to @a void.
 * @param n is the number fo bytes copying @a src to @a dst.
 * @return Pointer to @a dst.
 */
void *memcpy_forward_nocheck(void *dst, const void *src, size_t len);

/**
 * @fn void *memcpy_nocheck(void *restrict dst, const void *restrict src, size_t len)
 * @brief copy @a n bytes from memory area @a src to memory area @a dst without check.
 * We do tolerate overlapping regions here if @a src > @a dst. In
 * such case, (@a src - @a dst) must be bigger than movsq's block
 * copy factor: 8 bytes.
 *
 * @param dst is a pointer to @a void.
 * @param src is a pointer to @a void.
 * @param n is the number fo bytes copying @a src to @a dst.
 * @return Pointer to @a dst.
 */
void *memcpy_nocheck(void *restrict dst, const void *restrict src, size_t len);


/**
 * @fn void *memmove(void *dst, const void *src, size_t n)
 * @brief copy @a n bytes from memory area @a src to memory area @a dst.
 * The memory areas may overlap: copying takes place as though the bytes in @a src are first
 * copied into a temporary array that does not overlap @a src or @a dst,
 * and the bytes are then copied from the temporary array to @a dst.
 * @param dst Pointer to @c void.
 * @param src Pointer to @c void.
 * @param n Number of bytes copying @a src to @a dst.
 * @return Pointer to @a dst.
 */
void *memmove(void *dst, const void *src, size_t n);

/**
 * @fn size_t strlen(const char *s)
 * @brief calculate the length of the string @a s,
 * excluding the terminating null byte('\0').
 *
 * @param s Pointer to @c char.
 * @return Number of bytes in the string @a s.
 */
size_t strlen(const char *s);

/**
 * @fn size_t strnlen(const char *str, int n)
 * @brief returns the number of characters in the string pointed to by @a s,
 * excluding the terminating null byte ('\0'), but at most @a n.
 * In doing this, strnlen() looks only at the first n characters in the string
 * pointed to by @a s and never beyond @a s + @a n.
 *
 * @param s Pointer to @c char.
 * @param n Maximum length.
 * @return Number of bytes in the string @a s.
 */
size_t strnlen(const char *str, size_t n);


/**
 * @fn int strcmp(const char *s, const char *t)
 * @brief compare the two strings @a s1 and @a s2.
 *
 * @param s1 is a pointer to @c char.
 * @param s2 is a pointer to @c char.
 * @return Integer less than, equal to, or greater than zero
 * if @a s1 (or the first @a n bytes thereof) is found, respectively,
 * to be less than, to match, or be greater than @a s2.
 */
int strcmp(const char *s, const char *t);

/**
 * @fn int strncmp(const char *s1, const char *s2, size_t n)
 * @brief the strncmp() function is similar to the strcmp() function, except
 * it compares the only first (at most) @a n bytes of @a s1 and @a s2.
 *
 * @param s1 is a pointer to @c char.
 * @param s2 is a pointer to @c char.
 * @param n is the number of bytes comparing @a s1 to @a s2.
 * @return Integer less than, equal to, or greater than zero
 * if @a s1 (or the first @a n bytes thereof) is found, respectively,
 * to be less than, to match, or be greater than @a s2.
 */
int strncmp(const char *s1, const char *s2, size_t n);

/**
 * @fn char *strcpy(char *s, const char *t)
 * @brief copy the string pointed to by @a src, including the terminating null byte
 * ('\0'), to the buffer pointed to by @a dst.
 * The strings may not overlap, and the destination string @a dst must be large enough
 * to receive the copy.
 *
 * @param dst is a pointer to @c char.
 * @param src is a pointer to @c char.
 * @return Pointer to the destination string @a dst.
 */
char *strcpy(char *s, const char *t);

/**
 * @fn char *strncpy(char *s, const char *t, size_t n)
 * @brief The strncpy() function is similar to the strcpy() function, except that
 * at most @a n bytes of @a src are copied.
 * @warning If there is no null byte among the first @a n bytes of @a src,
 * the string placed in @a dst will  not  be  null-terminated.
 *
 * @param dst Pointer to @c char.
 * @param src Pointer to @c char.
 * @param n Number of bytes copying @a src to @a dst.
 * @return Pointer to the destination string @a dst.
 */
char *strncpy(char *s, const char *t, size_t n);

/**
 * @fn char *strcat(char *s, const char *t)
 * @brief The strcat() function appends the @a src string to the @a dst string,
 * overwriting the terminating null byte ('\0') at the end of @a dst,
 * and then adds a terminating null byte.
 * The strings may not overlap, and the @a dst string must have enough space for the result.
 * If @a dst is not large enough, program behavior is unpredictable.
 *
 * @param dst is a pointer to @c char.
 * @param src is a pointer to @c char.
 * @return Pointer to the destination string @a dst.
 */
char *strcat(char *dst, const char *src);

/**
 * @fn char *strncat(char *s, const char *t, size_t n)
 * @brief The strncat() function is similar to the strcat() function,
 * except that it will use at most n bytes from src; and
 * src does not need to be null-terminated if it contains @a n or more bytes.
 *
 * @param dst is a pointer to @c char.
 * @param src is a pointer to @c char.
 * @param n is the number of bytes appending @a src to @a dst.
 * @return Pointer to the destination string @a dst.
 */
char *strncat(char *dst, const char *src, size_t n);

/**
 * @fn char *strchr(const char *s, int c)
 * @brief The strchr() function returns a pointer to the first occurrence
 * of the character @a c in the string @a s.
 *
 * @param s is a pointer to @c char.
 * @param c is a character where it is located.
 * @return Pointer to the matched characture or NULL if the character is not found.
 */
char *strchr(const char *s, int c);

/**
 * @fn char *strtok(char *str, const char *delim)
 * @brief The strtok() function breaks a string into a sequence
 * of zero or more nonempty tokens.
 * On the first call to strtok() the string to be parsed should be specified in @a str.
 * In each subsequent call that should parse the same string, str must be NULL.
 *
 * @param str is a pointer to @c char.
 * @param delim specifies a set of bytes that delimit the tokens in the parsed string.
 * The caller may specify different strings in delim in successive calls
 * that parse the same string.
 * @return Pointer to @a dst.
 */
char *strtok(char *str, const char *delim);

#endif /* !CONFIG_ARCH_SIM */

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_STRING_H__ */
