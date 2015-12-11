#include <stdint.h>
#include <string.h>
#include <errno.h>
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

char *strndup(const char *s, size_t n)
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

int nprintf(const char *str, int len)
{
    int i = 0;
    
    while(i < len) {
        printf("%c", str[i++]);
    }
    
    return len;
}

/* "AB EF CD"  -> {AB,EF,CD} */
int bytes_string_2_bytes(char *str, uint8_t bytes[], int b_len)
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
	
	for(p = strtok(s, " \t"); p != NULL && len < b_len; p = strtok(NULL, " \t")) {
		int p_len = 0;
		long num = 0;
		char *end_ptr = NULL;

		p_len = strlen(p);
		if(p_len <= 0 || p_len > 2) {
			eno = -2;
			goto err;
		}

		num = strtol(p, &end_ptr, 16);
		if(errno == ERANGE || end_ptr != NULL) {
			eno = -2;
			goto err;
		}

		bytes[len++] = (uint8_t)(num & 0x0f);
		bytes[len++] = (uint8_t)(num & 0xf0);
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
