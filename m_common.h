#ifndef _M_COMMON_H_
#define _M_COMMON_H_

#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "m_log.h"

#ifndef offsetof
# define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif
  /**
   * container_of - cast a member of a structure out to the containing structure
   * @ptr:    the pointer to the member.
   * @type:   the type of the container struct this is embedded in.
   * @member: the name of the member within the struct.
   *
   */
#ifndef container_of
# define container_of(ptr, type, member) ({          \
        const typeof(((type *)0)->member)*__mptr = (ptr);    \
        (type *)((char *)__mptr - offsetof(type, member)); })
#endif

#ifdef _WIN32

static inline char *strndup(const char *s, size_t n)
{
    char *p = malloc(n + 1);
    if(!p) {
        return NULL;
    }
    
    strncpy(p, s, n + 1);
    p[n] = 0;
    
    return p;
}

#endif

static inline int nprintf(const char *str, int len)
{
    int i = 0;
    
    while(i < len) {
        printf("%c", str[i++]);
    }
    
    return len;
}

static inline void print_buffer(unsigned char *buf, int nbuf)
{
	int i, j;
	int col = 16;
	unsigned char ch;

	for (i = 0; i < nbuf; i++) {
		printf("%x%x ", buf[i] >> 4, buf[i] & 0xf);

		if ((i + 1) % col == 0) {
			for (j = 0; j < col; j++) {
				ch = *(buf + i + j - (col-1));

				printf("%c", isprint (ch) ? ch : '.');
			}

			printf("\n");
		}
	}

	for (j = 0; j < col - i % col; j++) {
		printf("   ");
	}

	for (j = 0; j < i % col; j++) {
		ch = *(buf + i + j - i % col);
		printf("%c", isprint (ch) ? ch : '.');
	}

	printf("\n");

	return;
}

/* "AB EF CD"  -> {AB,EF,CD} */
static inline int bytes_string_2_bytes(const char *str, uint8_t bytes[], int b_len)
{
	int eno = 0;
	int len = 0;
	char *s = NULL;
	char *p = NULL;
		
	s = strdup(str);
	if(s == NULL) {
		eno = -1;
		goto err;
	}
	
	errno = 0;
	for(p = strtok(s, " \t"); p != NULL && len < b_len; p = strtok(NULL, " \t")) {
		long num = 0;
		char *end_ptr = NULL;

		num = strtol(p, &end_ptr, 16);
		if(num > UINT8_MAX || errno != 0 || (end_ptr != NULL && *end_ptr != 0)) {
			eno = -2;
			goto err;
		}

		bytes[len++] = (uint8_t)num;
	}
	
	if(p) {
		eno = -3;
		goto err;
	}

	if(s) {
		free(s);
		s = NULL;
	}

	return len;
err:
	switch(eno) {
	case -1:
		ERROR("malloc failed @%s:%d \n", __func__, __LINE__);
		break;
	case -2:
		ERROR("illegal binary strings %s \n", str);
		break;
	case -3:
		ERROR("do not have enough bytes[] to contain the string %s \n", str);
		break;
	}

	if(s) {
		free(s);
		s = NULL;
	}

	return -1;
}

#endif
