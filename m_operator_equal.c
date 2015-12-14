#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "m_log.h"
#include "m_variable.h"
#include "m_operation.h"
#include "m_operator_equal.h"

struct equal_operator_option {
	int start;
	int len;
	int case_sensitive;
};

int m_op_equal_uint32_value(struct m_operation *operation)
{
	void *p = NULL;

	switch(operation->var->store_type) {
	case MST_ADDRESS:
		p = operation->var->var_addr;
		break;
	case MST_POINTER_ADDRESS:
		p = *(void **)(operation->var->var_addr);
		break;
	}

	return *((uint32_t *)(p)) == (uint32_t)(operation->value_i);
}

void m_op_equal_uint32_clean(struct m_operation *operation)
{
	(void)operation;
	return;
}

int m_op_equal_uint32_init(struct m_operation *operation, const char *option, const char *value)
{
	(void)option;
	operation->value_i = atoi(value);
	operation->operator_option = NULL;

	return 0;
}

/* TODO: do not support option now! equal(var_name[][]: value) */
int m_operator_equal_init(struct m_variable *var, const char *option, const char *value, struct m_operation *operation)
{
    (void)option;
	int eno = 0;
    
	operation->op = &operator_equal;
	operation->var = var;

	switch(var->real_type) {
	case MRT_UINT32:
		if(m_op_equal_uint32_init(operation, NULL, value) != 0) {
			eno = -1;
			goto err;
		}
		break;
	default:
		eno = -2;
		goto err;
		break;
	}

	return 0;
err:
	switch(eno) {
	case -1:
        break;
	case -2:
		ERROR("equal do not support type %s now", m_real_type_2_str(var->real_type));
		break;
	}

	return -1;
}

int m_operator_equal_value(struct m_operation *operation)
{
	switch(operation->var->real_type) {
	case MRT_UINT32:
		return m_op_equal_uint32_value(operation); 
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
		m_op_equal_uint32_clean(operation); 
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

