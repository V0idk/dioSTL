#include "mycstring.h"

static inline int isspace_(unsigned char c){
  return c ==  ' ';
}

static inline unsigned char tolower_(unsigned char c)
{
	if (c >= 'A'&& c <= 'Z')
		c -= 'A'-'a';
	return c;
}

static inline unsigned char toupper_(unsigned char c)
{
	if (c >= 'a'&& c <= 'z')
		c -= 'a'-'A';
	return c;
}
/**
 * strncasecmp - Case insensitive, length-limited string comparison
 * @s1: One string
 * @s2: The other string
 * @len: the maximum number of characters to compare
 */
 
int strncasecmp(const char *s1, const char *s2, size_t len)
{
	//https://www.wikiwand.com/en/Yes,_Virginia,_there_is_a_Santa_Claus
	/* Yes, Virginia, it had better be unsigned */
	//tolower_等函数unsigned
	unsigned char c1, c2;
	if (!len)
		return 0;
	//do少比较一次,找到首个不相等的即可
	do {
		c1 = *s1++;
		c2 = *s2++;
		if (!c1 || !c2)
			break;
		if (c1 == c2)
			continue;
		c1 = tolower_(c1);
		c2 = tolower_(c2);
		if (c1 != c2)
			break;
	} while (--len);
	return (int)c1 - (int)c2;
}

int strcasecmp(const char *s1, const char *s2)
{
	int c1, c2;

	do {
		c1 = tolower_(*s1++);
		c2 = tolower_(*s2++);
	} while (c1 == c2 && c1 != 0);
	return c1 - c2;
}

/**
 * strcpy - Copy a %NUL terminated string
 * @dest: Where to copy the string to
 * @src: Where to copy the string from
 */

char *strcpy(char *dest, const char *src)
{
	char *tmp = dest;
	while ((*dest++ = *src++) != '\0')
		continue;
	return tmp;
}

/**
 * 复制字符串count个字符 , 若源长度超过count不保证\0结尾. 若少于则保证\0填充所有,返回原字符串
 */

/*
char *strncpy(char *dest, const char *src, size_t count)
{
	char *tmp = dest;

	while (count) {
		if ((*tmp = *src) != 0)
			src++;
		tmp++;
		count--;
	}
	return dest;
}
*/
//优化版本
char *strncpy(char *dest, const char *src, size_t count)
{
	char *tmp = dest;

	while (count--) {
		if ((*tmp++ = *src++) == 0){
			while(count--){
				*tmp++ = '\0';
			}
			break;
		}
	}
	return dest;
}


/**
 * 复制src字符串到固定大小的dest中,且总是以\0结尾;返回源字符串长度
 */
size_t strlcpy(char *dest, const char *src, size_t size)
{
	size_t ret = strlen(src);

	if (size) {
		size_t len = (ret >= size) ? size - 1 : ret;
		memcpy(dest, src, len);
		dest[len] = '\0';
	}
	return ret;
}

/**
 * 添加一个null结尾的src到dest中.返回目标字符串
 */

char *strcat(char *dest, const char *src)
{
	char *tmp = dest;

	while (*dest)
		dest++;
	while ((*dest++ = *src++) != '\0')
		;
	return tmp;
}

/**
 * count最大复制字节数,返回原字符串
 */
char *strncat(char *dest, const char *src, size_t count)
{
	char *tmp = dest;
/*
	if (count) {
		while (*dest)
			dest++;
		while ((*dest++ = *src++) != 0) {
			if (--count == 0) {
				*dest = '\0';
				break;
			}
		}
	}
*/
	//重构
	while (*dest)
		dest++;
	while(count--){
		if((*dest++ = *src++) == 0){
			*dest = '\0';
			break;
		}
	}
	
	return tmp;
}

/**
 * src字符串接到固定大小count的dest中,保证dest以\0结尾, 返回新大小
 */
size_t strlcat(char *dest, const char *src, size_t count)
{
	size_t dsize = strlen(dest);
	size_t len = strlen(src);
	size_t res = dsize + len;


	dest += dsize;
	count -= dsize;
	if (len >= count)
		len = count-1;
	memcpy(dest, src, len);
	dest[len] = 0;
	return res;
}

/**
 * strcmp - 返回0,-1,1.
 * https://stackoverflow.com/questions/1356741/strcmp-and-signed-unsigned-chars
 * C99规定这种比较函数的返回值应该解释unsigned char
 */

int strcmp(const char *cs, const char *ct)
{
	unsigned char c1, c2;

	while (1) {
		c1 = *cs++;
		c2 = *ct++;
		if (c1 != c2)
			return c1 < c2 ? -1 : 1;
		if (!c1)
			break;
	}
	return 0;
}

/**
 * count:最多比较的次数
 */
int strncmp(const char *cs, const char *ct, size_t count)
{
	unsigned char c1, c2;

	while (count) {
		c1 = *cs++;
		c2 = *ct++;
		if (c1 != c2)
			return c1 < c2 ? -1 : 1;
		if (!c1)
			break;
		count--;
	}
	return 0;
}

/**
 * strchr - 找到某个字符位置,返回指针;没找到则返回0
 */
char *strchr(const char *s, int c)
{
	for (; *s != (char)c; ++s)
		if (*s == '\0')
			return NULL;
	return (char *)s;
}

/**
 * strchrnul -同上,但没找到则返回尾部.
 */
char *strchrnul(const char *s, int c)
{
	while (*s && *s != (char)c)
		s++;
	return (char *)s;
}

/**
 * strrchr - 找到最右出现的字符
 */
char *strrchr(const char *s, int c)
{
	const char *last = NULL;
	do {
		if (*s == (char)c)
			last = s;
	} while (*s++);
	return (char *)last;
}

/**
 * strnchr - 指定字符串大小为counnt
 */
char *strnchr(const char *s, size_t count, int c)
{
	for (; count-- && *s != '\0'; ++s)
		if (*s == (char)c)
			return (char *)s;
	return NULL;
}
/**
 * skip_spaces - 跳过开头空白,返回非空白首指针
 */
char *skip_spaces(const char *str)
{
	while (isspace_(*str))
		++str;
	return (char *)str;
}


/**
 * strim - 去掉首尾空格(只加一个\0,非填充),返回首部非空指针
 */
char *strim(char *s)
{
	size_t size;
	char *end;

	size = strlen(s);
	if (!size)
		return s;

	end = s + size - 1;
	//去尾部空格
	while (end >= s && isspace_(*end))
		end--;
	*(end + 1) = '\0';

	return skip_spaces(s);
}
/**
 * strlen - 计算字符串长度, 此处采用指针相减,避免了多一个变量赋值的方法.
 */
size_t strlen(const char *s)
{
	const char *sc;

	for (sc = s; *sc != '\0'; ++sc)
		continue;
	return sc - s;
}

/**
 * strnlen - 限定字符串最大长度
 */
size_t strnlen(const char *s, size_t count)
{
	const char *sc;

	for (sc = s; count-- && *sc != '\0'; ++sc)
		/* nothing */;
	return sc - s;
}

/**
 * strspn - 检索字符串 s 中出现第一个不在字符串 accept的字符时, 已匹配的长度大小
 */

/*
size_t strspn(const char *s, const char *accept)
{
	const char *p;
	const char *a;
	size_t count = 0;

	for (p = s; *p != '\0'; ++p) {
		for (a = accept; *a != '\0'; ++a) {
			if (*p == *a)
				break;
		}
		//p未在accept出现则返回count
		if (*a == '\0')
			return count;
		++count;
	}
	return count;
}

*/

//优化

size_t strspn(const char *s, const char *accept)
{
	const char *p;
	const char *a;

	for (p = s; *p != '\0'; ++p) {
		for (a = accept; *a != '\0'; ++a) {
			if (*p == *a)
				break;
		}
		if (*a == '\0')
			return p-s;
	}
	return p-s;
}

/**
 * strcspn - 返回s开头不在集合reject的字符合数
 */

/*
size_t strcspn(const char *s, const char *reject)
{
	const char *p;
	const char *r;
	size_t count = 0;

	for (p = s; *p != '\0'; ++p) {
		for (r = reject; *r != '\0'; ++r) {
			if (*p == *r)
				return count;
		}
		++count;
	}
	return count;
}

*/


size_t strcspn(const char *s, const char *reject)
{
	const char *p;
	const char *r;

	for (p = s; *p != '\0'; ++p) {
		for (r = reject; *r != '\0'; ++r) {
			if (*p == *r)
				return p-s;
		}
	}
	return p-s;
}

/**
 * strpbrk - 在cs中找出最先等于ct中任一字符的位置并返回,找不到返回NULL
 */
char *strpbrk(const char *cs, const char *ct)
{
	const char *sc1, *sc2;

	for (sc1 = cs; *sc1 != '\0'; ++sc1) {
		for (sc2 = ct; *sc2 != '\0'; ++sc2) {
			if (*sc1 == *sc2)
				return (char *)sc1;
		}
	}
	return NULL;
}

/**
 * strsep - 用ct的分隔符 分隔 字符串s成两半. 只分隔ct中第一个在s中出现的字符.
 * 返回左边的字符串.同时指针s被修改为指向右边的字符串.因此需要声明为char**修改指针
 * 	char a[100]="abcdefghijk";
		char *p =a;
    char b[]="mi";
		char* n=  strsep(&p, b);

		结果: i消失分隔: n指向abcdefgh,p指向jk
 */
char *strsep(char **s, const char *ct)
{
	char *sbegin = *s;
	char *end;

	if (sbegin == NULL)
		return NULL;

	end = strpbrk(sbegin, ct);
	if (end)
		*end++ = '\0';
	*s = end;
	return sbegin;
}


/**
 * match_string - 返回字符串数组中匹配的字符串在数组中的下标. 该数组以null结尾
 * @array:	array of strings
 * @n:		最大字符串个数,若中间有null则截止
 * @string:	string to match with
 *
 * Return:
 * index of a @string in the @array if matches, or %-EINVAL otherwise.
 */
int match_string(const char * const *array, size_t n, const char *string)
{
	size_t index;
	const char *item;

	for (index = 0; index < n; index++) {
		item = array[index];
		if (!item)
			break;
		//相等则返回
		if (!strcmp(item, string))
			return index;
	}

	return -1;
}
/**
 * memset - count个char单位覆盖
 */
void *memset(void *s, int c, size_t count)
{
	char *xs = (char *)s;

	while (count--)
		*xs++ = c;
	return s;
}

/**
 * memset16() - count个u16覆盖
 */
void *memset16(uint16_t *s, uint16_t v, size_t count)
{
	uint16_t *xs = s;
	while (count--)
		*xs++ = v;
	return s;
}

/**
 * memset32() - count个u32覆盖
 */
void *memset32(uint32_t *s, uint32_t v, size_t count)
{
	uint32_t *xs = s;

	while (count--)
		*xs++ = v;
	return s;
}

/**
 * memset64() - count个u64覆盖
 */
void *memset64(uint64_t *s, uint64_t v, size_t count)
{
	uint64_t *xs = s;

	while (count--)
		*xs++ = v;
	return s;
}


/**
 * memcpy - char单位复制. 注意指针转换
 */
void *memcpy(void *dest, const void *src, size_t count)
{
	char *tmp = (char *)dest;
	const char *s = (const char *)src;

	while (count--)
		*tmp++ = *s++;
	return dest;
}


/**
 * memmove - 同memcpy,但允许重叠. 能够保证源串在被覆盖之前将重叠区域的字节拷贝到目标区域中。
 */
void *memmove(void *dest, const void *src, size_t count)
{
	char *tmp;
	const char *s;

	if (dest <= src) {
		//源在目标后面,不会覆盖
		tmp = dest;
		s = src;
		while (count--)
			*tmp++ = *s++;
	} else {
		//源在目标前面,必须从后面开始复制
		tmp = dest;
		tmp += count;
		s = src;
		s += count;
		while (count--)
			*--tmp = *--s;
	}
	return dest;
}

/**
 * memcmp - 区域比较, count是字节数. 返回第一个不相等字符的差值.
 */
int memcmp(const void *cs, const void *ct, size_t count)
{
	const unsigned char *su1, *su2;
	int res = 0;

	for (su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
		if ((res = *su1 - *su2) != 0)
			break;
	return res;
}

/**
 * memscan - 给定内存区域找到某个字符的指针位置,找不到则返回尾部
 */
void *memscan(void *addr, int c, size_t size)
{
	unsigned char *p = addr;

	while (size) {
		if (*p == c)
			return (void *)p;
		p++;
		size--;
	}
  	return (void *)p;
}

/**
 * strstr - 找到子字符串s2在s1的位置. 未找到返回null
 */
char *strstr(const char *s1, const char *s2)
{
	size_t l1, l2;

	l2 = strlen(s2);
	if (!l2)
		return (char *)s1;
	
	l1 = strlen(s1);
	while (l1 >= l2) {
		l1--;
		if (!memcmp(s1, s2, l2))
			return (char *)s1;
		s1++;
	}
	return NULL;
}

/**
 * strnstr - 限制s1长度为len
 */
char *strnstr(const char *s1, const char *s2, size_t len)
{
	size_t l2;

	l2 = strlen(s2);
	if (!l2)
		return (char *)s1;
	while (len >= l2) {
		len--;
		if (!memcmp(s1, s2, l2))
			return (char *)s1;
		s1++;
	}
	return NULL;
}

/**
 * memchr - 同memscan,但找不到返回NULL
 */
void *memchr(const void *s, int c, size_t n)
{
	const unsigned char *p = s;
	while (n-- != 0) {
    if ((unsigned char)c == *p++) {
			return (void *)(p - 1);
		}
	}
	return NULL;
}
/**
 * strreplace - 替换某个字符
 */
char *strreplace(char *s, char old, char newchar)
{
	for (; *s; ++s)
		if (*s == old)
			*s = newchar;
	return s;
}
