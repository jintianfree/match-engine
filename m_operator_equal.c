#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "m_log.h"
#include "m_variable.h"
#include "m_operation.h"

struct equal_operator_option {
	int start;
	int len;
	int case_sensitive;
};

int m_op_equal_uint32_value(struct m_operation *operation)
{
	return *((uint32_t *)(operation->var->var_addr)) == (uint32_t)(operation->value_i);
}

void m_op_equal_uint32_clean(struct m_operation *operation)
{
	(void)operation;
	return;
}

int m_op_equal_uint32_init(struct m_operation *operation, char *option, char *value)
{
	(void)option;
	operation->value_i = atoi(value);
	operation->operator_option = NULL;

	return 0;
}

/* TODO: do not support option now! equal(var_name[][]: value) */
int m_operator_equal_init(const char *str, struct m_operation *operation, struct m_variable_list *head)
{
	int eno = 0;
	char *p = NULL;
	struct m_variable *var = NULL;

	p = strdup(str);
	if(p == NULL) {
		eno = -1;
		goto err;
	}

	/* equal(var_name:value) */
	char *op = strtok(p, "(:)");
	if(op == NULL) {
		eno = -2;
		goto err;
	}

	char *var_name = strtok(NULL, "(:)");
	if(var_name == NULL) {
		eno = -2;
		goto err;
	}

	/* TODO: 
	 * char *option =
	 */

	char *value = strtok(NULL, "(:)");
	if(value == NULL) {
		eno = -2;
		goto err;
	}

	if((var = name_2_var(head, var_name)) == NULL) {
		eno  = -3;
		goto err;
	}

	operation->var = var;

	switch(var->type) {
	case MT_UINT32:
		if(m_op_equal_uint32_init(operation, NULL, value) != 0) {
			eno = -4;
			goto err;
		}
		break;
	default:
		eno = -5;
		goto err;
		break;
	}

	if(p) {
		free(p);
		p = NULL;
	}

	return 0;
err:
	switch(eno) {
	case -1:
		ERROR("malloc error @%s:%d \n", __func__, __LINE__);
		break;
	case -2:
		ERROR("illegal str %s \n", str);
		break;
	case -3:
		ERROR("coundn't find var %s \n", var_name);
		break;
	case -4:
		ERROR("%s do not support type %d \n", op, var->type);
		break;
	case -5:
		ERROR("%s parse value error \n", str);
		break;
	}

	if(p) {
		free(p);
		p = NULL;
	}

	return -1;
}

int m_operator_equal_value(struct m_operation *operation)
{
	switch(operation->var->type) {
	case MT_UINT32:
		return m_op_equal_uint32_value(operation); 
		break;
	default:
		break;
	}

	return 0;
}

void m_operator_equal_clean(struct m_operation *operation)
{
	switch(operation->var->type) {
	case MT_UINT32:
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

