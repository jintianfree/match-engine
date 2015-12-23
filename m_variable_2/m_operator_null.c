#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "m_log.h"
#include "m_variable_descr.h"
#include "m_operation.h"
#include "m_operator_null.h"

int m_operator_null_init(struct m_variable_descr *descr, const char *option, const char *value, struct m_operation *operation)
{
	(void)option;
	(void)value;

	int eno = 0;

	operation->op = &operator_null;
	operation->descr = descr;

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
		ERROR("null do not support type %s now", m_variable_real_type_2_str(descr->real_type));
		break;
	}

	return -1;
}

int m_operator_null_value(struct m_operation *operation, struct m_variable_table *table)
{
	void *p = NULL;
	size_t *plen = 0;

	struct m_variable_descr *descr = operation->descr;
	switch(descr->store_type) {
	case MST_MEMORY:
		p = table->base + descr->var_offset;
		break;
	case MST_ADDRESS:
		p = *(void **)(table->base + descr->var_offset);
		break;
	}

	plen = table->base + descr->var_len_offset;

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

