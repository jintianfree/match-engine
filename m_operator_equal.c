#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include "m_log.h"
#include "m_variable.h"
#include "m_operation.h"
#include "m_operator_equal.h"

/* [n,m][I,B]*/
struct equal_operator_option {
	int start;          /* [start, end) */
	int end;
	int ignore_case;    /* I -- ignoring the case of the characters */
	int binary_string;  /* B -- binary string  AC EF 0A */
};

struct equal_operator_option *parse_m_operator_equal_option(const char *option)
{
	int eno = 0;
	char *str = NULL;
	struct equal_operator_option *pop = NULL;
	struct equal_operator_option op = {0, 0, 0, 0};
    
	if(option == NULL) {
		goto end;
	}

	str = malloc(strlen(option) + 1);	/* +1 for '/0' */
	if(str == NULL) {
		eno = -1;
		goto err;
	}

	/* remove ' ' & '\t' */
	size_t i = 0, j = 0;
	for(i = 0; i < strlen(option); i++) {
		if(option[i] == ' ' || option[i] == '\t') {
			continue;
		}

		str[j++] = option[i];
	}
	str[j] = 0;

	if(strlen(str) == 0) {
		goto end;
	}

	char *p1 = strchr(str, '[');
	char *p2 = strchr(str, ']');
	char *p3 = strchr(str, ',');
	char *pend = NULL;

	if(p1 == NULL || p2 == NULL || p1 > p2 || (p3 && p3 < p1)) {
		eno = -2;
		goto err;
	}

	if(p2 - p1 > 1) {

		if(p3 != NULL && p3 < p1) {	/* ,[n] */
			eno = -3;
			goto err;
		}

		if(p3 != NULL && p3 > p2) {	/*[m][,] */
			p3 = NULL;
		}

		if(p3 == NULL) {		/* [m] */
			op.start = strtoul(p1 + 1, &pend, 10);
			if(errno != 0 || pend == p1 + 1) {
				eno = -3;
				goto err;
			}
		} else if(p1 + 1 == p3 && p3 + 1 == p2) {	/* [,] */
			;
		} else if(p1 + 1 == p3) {	/* [,n] */
			op.end = strtoul(p3 + 1, &pend, 10);
			if(errno != 0 || pend == p3 + 1) {
				eno = -3;
				goto err;
			}
		} else if(p3 + 1 == p2) {	/* [m,] */
			op.start = strtoul(p1 + 1, &pend, 10);
			if(errno != 0 || pend == p1 + 1) {
				eno = -3;
				goto err;
			}
		} else {			/* [m,n] */
			op.start = strtoul(p1 + 1, &pend, 10);
			if(errno != 0 || pend == p1 + 1) {
				eno = -3;
				goto err;
			}

			op.end = strtoul(p3 + 1, &pend, 10);
			if(errno != 0 || pend == p3 + 1) {
				eno = -3;
				goto err;
			}
		}


		if(op.start && op.end && op.start > op.end) {
			eno = -3;
			goto err;
		}
	} else {				/* [] */
		;
	}

	if(strchr(option, 'I') != NULL) {
		op.ignore_case = 1;
	}

	if(strchr(option, 'B') != NULL) {
		op.binary_string = 1;
	}

	if((pop = malloc(sizeof(struct equal_operator_option))) == NULL) {
		eno = -4;
		goto err;
	}

	memcpy(pop, &op, sizeof(struct equal_operator_option));

	if(str) {
		free(str);
		str = NULL;
	}
end:
	return pop;
err:
	switch(eno) {
	case -1:
	case -4:
		ERROR("malloc error @%s:%d \n", __func__, __LINE__);
		break;
	case -2:
	case -3:
		ERROR("illegal option %s \n", option);
		break;
	}

	return (void *)-1;
}

int m_operator_equal_init_int(struct m_variable *var, const char *value, struct m_operation *operation)
{
	int eno = 0;
	long long int lli = 0;
	unsigned long long  int ulli = 0;

	errno = 0;

	switch(var->real_type) {
		case MRT_UINT8:
		case MRT_UINT16:
		case MRT_UINT32:
		case MRT_UINT64:
			ulli = strtoull(value, NULL, 10);
			operation->value_i = ulli;
			operation->operator_option = NULL;
			break;
		case MRT_INT8:
		case MRT_INT16:
		case MRT_INT32:
		case MRT_INT64:
			lli = strtoll(value, NULL, 10);
			operation->value_i = lli;
			operation->operator_option = NULL;
			break;
		default:
			break;
	}


	if(errno != 0) {
		eno = -1;
		goto err;
	}

	switch(var->real_type) {
		case MRT_UINT8:
			if(ulli > UINT8_MAX) {
				eno = -2;
				goto err;
			}
			break;
		case MRT_UINT16:
			if(ulli > UINT16_MAX) {
				eno = -2;
				goto err;
			}
			break;
		case MRT_UINT32:
			if(ulli > UINT32_MAX) {
				eno = -2;
				goto err;
			}
			break;
		case MRT_UINT64:
			break;
		case MRT_INT8:
			if(lli > INT8_MAX || lli < INT8_MIN) {
				eno = -2;
				goto err;
			}
			break;
		case MRT_INT16:
			if(lli > INT16_MAX || lli < INT16_MIN) {
				eno = -2;
				goto err;
			}
			break;
		case MRT_INT32:
			if(lli > INT32_MAX || lli < INT32_MIN) {
				eno = -2;
				goto err;
			}
			break;
		case MRT_INT64:
			break;
		default:
			break;
	}

	return 0;
err:
	switch(eno) {
		case -1:
			ERROR("parse value (%s) error \n", value);
			break;
		case -2:
			ERROR("value (%s) overflow  type (%s) \n", value, m_real_type_2_str(var->real_type));
			break;
	}

	return -1;
}

/* TODO: do not support option now! equal(var_name[][]: value) */
int m_operator_equal_init(struct m_variable *var, const char *option, const char *value, struct m_operation *operation)
{
	(void)option;

	int eno = 0;

	if(value == NULL) {
		eno = -1;
		goto err;
	}

	operation->op = &operator_equal;
	operation->var = var;

	switch(var->real_type) {
		case MRT_UINT8:
		case MRT_INT8:
		case MRT_UINT16:
		case MRT_INT16:
		case MRT_UINT32:
		case MRT_INT32:
		case MRT_UINT64:
		case MRT_INT64:
			if(m_operator_equal_init_int(var, value, operation) != 0) {
				eno = -2;
				goto err;
			}
			break;
		case MRT_STRING:
			operation->operator_option = parse_m_operator_equal_option(option);
			if(operation->operator_option == (void *)-1) {
				eno = -3;
				goto err;
			}
			if((operation->value_p = strdup(value)) == NULL) {
				eno = -4;
				goto err;
			}
			operation->value_len = strlen(value);

			break;
		default:
			eno = -5;
			goto err;
			break;
	}


	return 0;
err:
	switch(eno) {
		case -1:
			ERROR("null value \n");
			break;
		case -2:
			break;
		case -3:
			break;
		case -4:
			ERROR("malloc error @%s:%d \n", __func__, __LINE__);
			break;
		case -5:
			ERROR("equal do not support type %s now", m_real_type_2_str(var->real_type));
			break;
	}

	return -1;
}

int m_operator_equal_value(struct m_operation *operation)
{
	void *p = NULL;
	size_t plen = 0;
	struct equal_operator_option *option = NULL;

	switch(operation->var->store_type) {
		case MST_ADDRESS:
			p = operation->var->var_addr;
			break;
		case MST_POINTER_ADDRESS:
			p = *(void **)(operation->var->var_addr);
			break;
	}

	if(p == NULL) {
		return 0;
	}

	if(operation->var->var_len) {
		plen = *(operation->var->var_len);
	}   

	switch(operation->var->real_type) {
		case MRT_UINT8:
		case MRT_INT8:
			return *((uint8_t *)(p)) == (uint8_t)(operation->value_i);
			break;
		case MRT_UINT16:
		case MRT_INT16:
			return *((uint16_t *)(p)) == (uint16_t)(operation->value_i);
			break;
		case MRT_UINT32:
		case MRT_INT32:
			return *((uint32_t *)(p)) == (uint32_t)(operation->value_i);
			break;
		case MRT_UINT64:
		case MRT_INT64:
			return *((uint64_t *)(p)) == (uint64_t)(operation->value_i);
			break;
		case MRT_STRING:	
			option = (struct equal_operator_option *)operation->operator_option;
			if(option) {
				size_t start = option->start;
				size_t end = option->end == 0 ? plen : (size_t)option->end;

				if(start > plen || end > plen) {
					return 0;
				}

				p += start;
				plen = (end - start);
			}

			if(plen != operation->value_len) {
				return 0;
			}

			if(option && option->ignore_case) {
				return (strncasecmp((char *)p, (char *)operation->value_p, plen) == 0);
			} else {
				return (strncmp((char *)p, (char *)operation->value_p, plen) == 0);
			}

			break;
		default:
			break;
	}

	return 0;
}

void m_operator_equal_clean(struct m_operation *operation)
{
	switch(operation->var->real_type) {
		case MRT_STRING:
			if(operation->value_p) {
				free(operation->value_p);
				operation->value_p = NULL;
			}
			break;
		default:
			break;
	}

	operation->var = NULL;
	operation->op = NULL;
	operation->value_i = 0;
	if(operation->operator_option) {
		free(operation->operator_option);
		operation->operator_option = NULL;
	}
}

struct m_operator operator_equal = {
	.name = "equal",
	.init = m_operator_equal_init,
	.clean = m_operator_equal_clean,
	.value = m_operator_equal_value,
};

