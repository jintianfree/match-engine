#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "m_log.h"
#include "m_variable_table.h"
#include "m_operation.h"
#include "m_operator_equal.h"
#include "m_operator_null.h"

struct m_operator *operators[] = {
	&m_operator_equal,
	&m_operator_null,
	NULL,
};

/* operator(var_name[option]:value) */
struct m_operation *m_operation_init(const char *str, struct m_variable_table_manager *manager)
{
	int eno = 0;
	char *p = NULL;
	char *op = NULL;
	char *var_name = NULL;
	char *value = NULL;
	char *option = NULL;

	struct m_variable_descr *descr = NULL;
	struct m_operation *operation = NULL;

	assert(str);

	p = strdup(str);
	if(p == NULL) {
		eno = -1;
		goto err;
	}

	if(p[strlen(p) - 1] != ')') {
		eno = -2;
		goto err;
	}

	/* operator(var_name[option]:value) */
	char *pos = p;

	op = pos;
	if((pos = strchr(pos, '(')) != NULL) {
		*pos = '\0';
		pos += 1;
	} else {
		eno = -2;
		goto err;
	}


	var_name = pos;
	if((pos = strchr(pos, ':')) != NULL) {
		*pos = 0;
		pos += 1;

		if(*pos == ')' || *pos == '\0') {
			eno = -2;
			goto err;
		}

		value = pos;
		value[strlen(value) - 1] = '\0'; /* remove ')' */
	} else {
		if(*var_name == '\0') {
			eno = -2;
			goto err;
		}
		var_name[strlen(var_name) - 1] = '\0'; /* remove ')' */
		value = NULL;
	}
	

	char *o = strchr(var_name, '[');	
	if(o != NULL) {
		option = strdup(o);
		*o = '\0';	/* var_name[option] -> var_name */
	}

	struct m_operator **operator = operators;
	while(*operator != NULL) {
		if(strcmp((*operator)->name, op) == 0) {
			break;
		}

		operator++;
	}

	if(!*operator) {
		eno = -3;
		goto err;
	}

	if((descr = m_variable_descr_find_by_name(manager, var_name)) == NULL) {
		eno  = -4;
		goto err;
	}

	if((operation = malloc(sizeof(struct m_operation))) == NULL) {
		eno = -5;
		goto err;
	}

	
	if((*operator)->init(descr, option, value, operation) != 0) {
		eno = -6;
		goto err;
	}

	if(p) {
		free(p);
		p = NULL;
	}

	if(option) {
		free(option);
		option = NULL;
	}

	return operation;
err:
	switch(eno) {
	case -1:
		ERROR("malloc error @%s:%d \n", __func__, __LINE__);
		break;
	case -2:
		ERROR("illegal operation (%s) \n", str);
		break;
	case -3:
		ERROR("do not support operator (%s) in (%s) \n", op, str);
		break;
	case -4:
		ERROR("unknown var name (%s) in (%s) \n", var_name, str);
		break;
	case -5:
		ERROR("malloc error @%s:%d \n", __func__, __LINE__);
		break;
	case -6:
		ERROR("parse (%s) error \n", str);
        break;
	}

	if(p) {
		free(p);
		p = NULL;
	}

	if(option) {
		free(option);
		option = NULL;
	}

	if(operation) {
		free(operation);
		operation = NULL;
	}

	return NULL;
}

int m_operation_value(struct m_operation *operation, struct m_variable_table *table)
{
	assert(operation && operation->op && table);

	return operation->op->value(operation, table);
}

void m_operation_clean(struct m_operation *operation)
{
	assert(operation && operation->op);

	operation->op->clean(operation);
    
    free(operation);
    operation = NULL;
    
	return;
}

