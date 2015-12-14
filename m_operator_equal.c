#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "m_log.h"
#include "m_variable.h"
#include "m_operation.h"
#include "m_operator_equal.h"

struct equal_operator_option {
	int start;
	int len;
	int case_sensitive;
};


int m_operator_equal_int(struct m_variable *var, const char *value, struct m_operation *operation)
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
		if(ulli > 0xff) {
			eno = -2;
			goto err;
		}
		break;
	case MRT_UINT16:
		if(ulli > 0xffff) {
			eno = -2;
			goto err;
		}
		break;
	case MRT_UINT32:
		if(ulli > 0xffffffff) {
			eno = -2;
			goto err;
		}
		break;
	case MRT_UINT64:
		break;
	case MRT_INT8:
		if(lli > 0x7f || lli < -0x7f) {
			eno = -2;
			goto err;
		}
		break;
	case MRT_INT16:
		if(lli > 0x7fff || lli < -0x7fff) {
			eno = -2;
			goto err;
		}
		break;
	case MRT_INT32:
		if(lli > 0x7fffffff || lli < -0x7fffffff) {
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
		if(m_operator_equal_int(var, value, operation) != 0) {
			eno = -2;
			goto err;
		}
		break;
	default:
		eno = -3;
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
		ERROR("equal do not support type %s now", m_real_type_2_str(var->real_type));
		break;
	}

	return -1;
}

int m_operator_equal_value(struct m_operation *operation)
{
	void *p = NULL;
	size_t *plen = NULL;

	switch(operation->var->store_type) {
	case MST_ADDRESS:
		p = operation->var->var_addr;
		break;
	case MST_POINTER_ADDRESS:
		p = *(void **)(operation->var->var_addr);
		break;
	}

	plen = operation->var->var_len;


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
	default:
		break;
	}

	return 0;
}

void m_operator_equal_clean(struct m_operation *operation)
{
	switch(operation->var->real_type) {
	case MRT_UINT32:
		break;
	default:
		break;
	}
}

struct m_operator operator_equal = {
	.name = "equal",
	.init = m_operator_equal_init,
	.clean = m_operator_equal_clean,
	.value = m_operator_equal_value,
};

