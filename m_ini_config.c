#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <assert.h>
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

	switch(descr->type) {
	case ICVT_STRING:
		len = strlen(value);
		if(len > descr->max_len) {
			len = descr->max_len;
		}

		strncpy(descr->addr, value, len);
		((char *)descr->addr)[len - 1] = 0;
		break;
	case ICVT_INT:
		lli = strtoll(value, &end, 10);
		if(end != NULL || lli > INT_MAX || lli < INT_MIN) {
			goto err;
		}
		*(int *)descr->addr = (int)lli;
		break;
	case ICVT_UINT:
		ulli = strtoull(value, &end, 10);
		if(end != NULL || ulli > UINT_MAX) {
			goto err;
		}
		*(unsigned int *)descr->addr = (unsigned int)ulli;
		break;
	case ICVT_LONG:
		lli = strtoll(value, &end, 10);
		if(end != NULL || lli > LONG_MAX || lli < LONG_MIN) {
			goto err;
		}
		*(long *)descr->addr = (long)ulli;
		break;
	case ICVT_ULONG:
		ulli = strtoull(value, &end, 10);
		if(end != NULL || ulli > ULONG_MAX) {
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
	ERROR("parse value:%s error on session:%s key: %s, "
			"consist of illegal char or integer overflow", 
			value, descr->section, descr->key);

	return -1;
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
int m_ini_config_read(const char *filename, struct m_ini_config_descr *descr)
{
	assert(filename && descr);

	char *value = NULL;
	struct ini *ini = NULL;
	struct read_ini *rini = NULL;
	struct m_ini_config_descr *d = NULL;

	if((ini = read_ini(&rini, filename)) == NULL) {
		goto err;
	}

	for(d = descr; d->addr != NULL; d++) {
		value = ini_get_value(ini, d->section, d->key);
		if(value == NULL) {
			value = d->default_value;
		}

		if(convert_value(value, d) != 0) {
			goto err;
		}
	}

	destroy_ini(ini);
	cleanup_readini(rini);

	return 0;

err:
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

int m_ini_config_next(struct m_ini_config *config, struct m_ini_config_descr *descr)
{
	char *value = NULL;
	struct section *section = NULL;
	struct m_ini_config_descr *d = NULL;

	if(config->next_section >= config->ini->num_sections) {
		return 1;
	}

	section = config->ini->sections[config->next_section];

	for(d = descr; d->addr != NULL; d++) {
		value = ini_get_value_in_section(section, d->key);
		if(value == NULL) {
			value = d->default_value;
		}

		if(convert_value(value, d) != 0) {
			goto err;
		}

	}

	return 0;

err:
	return -1;
}

void m_ini_config_clean(struct m_ini_config *config)
{
	destroy_ini(config->ini);
	cleanup_readini(config->rini);

	free(config);
}

