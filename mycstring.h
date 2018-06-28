

#ifndef _MYCSTRING_H_
#define _MYCSTRING_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdint.h>
extern char * strcpy(char *,const char *);
extern char * strncpy(char *,const char *, size_t);
size_t strlcpy(char *, const char *, size_t);
extern char * strcat(char *, const char *);
extern char * strncat(char *, const char *, size_t);
extern size_t strlcat(char *, const char *, size_t);
extern int strcmp(const char *,const char *);
extern int strncmp(const char *,const char *,size_t);
extern int strcasecmp(const char *s1, const char *s2);
extern int strncasecmp(const char *s1, const char *s2, size_t n);
extern char * strchr(const char *,int);
extern char * strchrnul(const char *,int);
extern char * strnchr(const char *, size_t, int);
extern char * strrchr(const char *,int);

extern char *  skip_spaces(const char *);

extern char *strim(char *);

static inline char *strstrip(char *str)
{
	return strim(str);
}
extern char * strstr(const char *, const char *);
extern char * strnstr(const char *, const char *, size_t);
extern size_t strlen(const char *);
extern size_t strnlen(const char *,size_t);
extern char * strpbrk(const char *,const char *);
extern char * strsep(char **,const char *);
extern size_t strspn(const char *,const char *);
extern size_t strcspn(const char *,const char *);

extern void * memset(void *,int,size_t);

extern void *memset16(uint16_t *, uint16_t, size_t);

extern void *memset32(uint32_t *, uint32_t, size_t);

extern void *memset64(uint64_t *, uint64_t, size_t);
static inline void *memset_l(unsigned long *p, unsigned long v,
		size_t n)
{
	if (sizeof(p) == 4)
		return memset32((uint32_t *)p, v, n);
	else
		return memset64((uint64_t *)p, v, n);
}

static inline void *memset_p(void **p, void *v, size_t n)
{
	if (sizeof(p) == 4)
		return memset32((uint32_t *)p, (uintptr_t)v, n);
	else
		return memset64((uint64_t *)p, (uintptr_t)v, n);
}
extern void * memcpy(void *,const void *,size_t);
extern void * memmove(void *,const void *,size_t);
extern void * memscan(void *,int,size_t);
extern int memcmp(const void *,const void *,size_t);
extern void * memchr(const void *,int,size_t);
static inline  int memcpy_mcsafe(void *dst, const void *src,
		size_t cnt)
{
	memcpy(dst, src, cnt);
	return 0;
}
static inline void memcpy_flushcache(void *dst, const void *src, size_t cnt)
{
	memcpy(dst, src, cnt);
}

char *strreplace(char *s, char old, char newstr);

int match_string(const char * const *array, size_t n, const char *string);



#ifdef __cplusplus
}
#endif

#endif