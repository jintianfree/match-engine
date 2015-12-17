#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "m_log.h"
#include "m_variable.h"
#include "m_operation.h"
#include "m_operator_null.h"

int m_operator_null_init(struct m_variable *var, const char *option, const char *value, struct m_operation *operation)
{
	(void)option;
	(void)value;

	int eno = 0;

	operation->op = &operator_null;
	operation->var = var;

	switch(var->real_type) {
	case MRT_UINT8:
	case MRT_UINT16:
	case MRT_UINT32:
	case MRT_UINT64:
	case MRT_INT8:
	case MRT_INT16:
	case MRT_INT32:
	case MRT_INT64:
	case MRT_STRING:
	case MRT_BYTES:
		break;
	default:
		eno = -1;
		goto err;
		break;
	}

	return 0;
err:
	switch(eno) {
	case -1:
		ERROR("null do not support type %s now", m_real_type_2_str(var->real_type));
		break;
	}

	return -1;
}

int m_operator_null_value(struct m_operation *operation)
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
		return (p == NULL) || (*(uint8_t *)p == 0);
		break;
	case MRT_UINT16:
		return (p == NULL) || (*(uint16_t *)p == 0);
		break;
	case MRT_UINT32:
		return (p == NULL) || (*(uint32_t *)p == 0);
		break;
	case MRT_UINT64:
		return (p == NULL) || (*(uint64_t *)p == 0);
		break;
	case MRT_INT8:
		return (p == NULL) || (*(int8_t *)p == 0);
		break;
	case MRT_INT16:
		return (p == NULL) || (*(int16_t *)p == 0);
		break;
	case MRT_INT32:
		return (p == NULL) || (*(int32_t *)p == 0);
		break;
	case MRT_INT64:
		return (p == NULL) || (*(int64_t *)p == 0);
		break;
	case MRT_STRING:
	case MRT_BYTES:
		return (p == NULL) || (*plen == 0);
		break;
	default:
		break;
	}

	return 0;
}

void m_operator_null_clean(struct m_operation *operation)
{
	(void)operation;
	return;
}

struct m_operator operator_null = {
	.name = "null",
	.init = m_operator_null_init,
	.clean = m_operator_null_clean,
	.value = m_operator_null_value,
};

