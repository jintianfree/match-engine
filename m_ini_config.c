#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include "m_log.h"
#include "m_ini_parser.h"
#include "m_ini_config.h"

static int convert_value(const char *value, struct m_ini_config_descr *descr)
{
	assert(value && descr);

	int len = 0;
	char *end = NULL;
	long long int lli = 0;
	unsigned long long int ulli = 0;

	errno = 0;
	switch(descr->type) {
	case ICVT_STRING:
		len = strlen(value);
		if(len > descr->max_len - 1) {
			ERROR("length of %s (%d) >= max len of %s (%d) \n",
					value, len, descr->key, descr->max_len);
			goto err;
		}

		strncpy(descr->addr, value, len);
		((char *)descr->addr)[len] = 0;
		break;
	case ICVT_INT:
		lli = strtoll(value, &end, 10);
		if(errno != 0 || (end && *end != 0)|| lli > INT_MAX || lli < INT_MIN) {
			goto err;
		}
		*(int *)descr->addr = (int)lli;
		break;
	case ICVT_UINT:
		ulli = strtoull(value, &end, 10);
		if(errno != 0 || (end && *end != 0) || ulli > UINT_MAX) {
			goto err;
		}
		*(unsigned int *)descr->addr = (unsigned int)ulli;
		break;
	case ICVT_LONG:
		lli = strtoll(value, &end, 10);
		if(errno != 0 || (end && *end != 0) || lli > LONG_MAX || lli < LONG_MIN) {
			goto err;
		}
		*(long *)descr->addr = (long)lli;
		break;
	case ICVT_ULONG:
		ulli = strtoull(value, &end, 10);
		if(errno != 0 || (end && *end != 0) || ulli > ULONG_MAX) {
			goto err;
		}
		*(unsigned long *)descr->addr = (unsigned long)ulli;
		break;
	case ICVT_BOOL:
		if(strcasecmp(value, "yes") == 0 ||
				strcasecmp(value, "true") == 0 ||
				strcasecmp(value, "1") == 0){
			*(char *)descr->addr = 1;
		} else if(strcasecmp(value, "no") == 0 ||
				strcasecmp(value, "false") == 0 ||
				strcasecmp(value, "0") == 0) {
			*(char *)descr->addr = 0;
		} else {
			goto err;
		}

		break;
	default:
		break;
	}

	return 0;

err:
	return -1;
}

void m_ini_config_print_descr(struct m_ini_config_descr descr[])
{
	struct m_ini_config_descr *d = descr;

	while(d->addr != NULL) {
		printf("[%s] %s : ", d->section, d->key);
	
		switch(d->type) {
		case ICVT_STRING:
			printf("%s \n", (char *)d->addr);
			break;
		case ICVT_INT:
			printf("%d \n", *(int *)d->addr);
			break;
		case ICVT_UINT:
			printf("%u \n", *(unsigned int *)d->addr);
			break;
		case ICVT_LONG:
			printf("%ld \n", *(long *)d->addr);
			break;
		case ICVT_ULONG:
			printf("%lu \n", *(unsigned long *)d->addr);
			break;
		case ICVT_BOOL:
			printf("%s \n", *(char *)d->addr == 0 ? "No" : "Yes");
			break;
		default:
			printf("\n");
		}

		d++;
	}

	return;
}

/*
 * [abc]
 * a = value1
 * b = value2
 *
 * [lmn]
 * n = value1
 * m = value2
 */
int m_ini_config_read(const char *filename, struct m_ini_config_descr descr[])
{
	assert(filename && descr);

	int eno = 0;
	char *value = NULL;
	struct ini *ini = NULL;
	struct read_ini *rini = NULL;
	struct m_ini_config_descr *d = NULL;

	if((ini = read_ini(&rini, filename)) == NULL) {
		eno = -1;
		goto err;
	}

	for(d = descr; d->addr != NULL; d++) {
		value = ini_get_value(ini, d->section, d->key);
		if(value == NULL) {
			if(d->necessary) {
				eno = -2;
				goto err;
			}

			value = d->default_value;
		}

		if(convert_value(value, d) != 0) {
			eno  = -3;
			goto err;
		}
	}

	destroy_ini(ini);
	cleanup_readini(rini);

	return 0;

err:
	switch(eno) {
	case -2:
		ERROR("can not find %s in %s, which must configed \n", 
				descr->section, descr->key);
		break;
	case -3:
		ERROR("parse value:%s error on file :%s section:%s key: %s, "
				"consist of illegal char or string|integer overflow \n", 
				filename, value, descr->section, descr->key);
		break;
	}

	if(ini) {
		destroy_ini(ini);
		cleanup_readini(rini);
	}

	return -1;
}


/*
 * [pair1]
 * id = 1
 * name = name1
 * 
 * [pair2]
 * id=2
 * name = name2
 * 
 * [pair3]
 * id = 3
 * name = name3
 *
 * ...
 *
 */
struct m_ini_config *m_ini_config_init(const char *filename)
{
	struct ini *ini = NULL;
	struct read_ini *rini = NULL;
	struct m_ini_config *config = NULL;

	if((config = malloc(sizeof(struct m_ini_config))) == NULL) {
		goto err;
	}

	if((ini = read_ini(&rini, filename)) == NULL) {
		goto err;
	}

	config->ini = ini;
	config->rini = rini;
	config->next_section = 0;

	return config;
err:
	return NULL;
}

int m_ini_config_next(struct m_ini_config *config, struct m_ini_config_descr descr[])
{
	assert(config);

	int eno = 0;
	char *value = NULL;
	struct section *section = NULL;
	struct m_ini_config_descr *d = NULL;

	if(config->next_section >= config->ini->num_sections) {
		return 0;
	}

	section = config->ini->sections[config->next_section];
	config->next_section++;

	for(d = descr; d->addr != NULL; d++) {
		value = ini_get_value_in_section(section, d->key);
		if(value == NULL) {
			if(d->necessary) {
				eno = -1;
				goto err;
			}

			value = d->default_value;
		}

		if(convert_value(value, d) != 0) {
			eno = -2;
			goto err;
		}

	}

	return 1;

err:
	switch(eno) {
	case -1:
		ERROR("can not find %s, which must configed \n", d->key);
		break;
	case -2:
		ERROR("parse value:%s error on key: %s, "
				"consist of illegal char or string|integer overflow \n", 
				value, d->key);
		break;
	}
	return -1;
}

void m_ini_config_clean(struct m_ini_config *config)
{
	assert(config);

	destroy_ini(config->ini);
	cleanup_readini(config->rini);

	free(config);
}

