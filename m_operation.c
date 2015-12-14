#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "m_log.h"
#include "m_variable.h"
#include "m_operation.h"
#include "m_operator_equal.h"

struct m_operator *operators[] = {
	&operator_equal,
	NULL,
};

/* operator(var_name[operation]:value) */
struct m_operation *m_operation_init(const char *str, struct m_variable_list *head)
{
	int eno = 0;
	char *p = NULL;
	char *op = NULL;
	char *var_name = NULL;
	char *value = NULL;
	char *option = NULL;

	assert(str);

	p = strdup(str);
	if(p == NULL) {
		eno = -1;
		goto err;
	}

	if((op = strtok(p, "(:)")) == NULL) {
		eno = -2;
		goto err;
	}

	if((var_name = strtok(NULL, "(:)")) == NULL) {
		eno = -2;
		goto err;
	}

	if((value = strtok(NULL, "(:)")) == NULL) {
		eno = -2;
		goto err;
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

	struct m_variable *var = NULL;
	if((var = m_variable_name_2_var(head, var_name)) == NULL) {
		eno  = -4;
		goto err;
	}

	struct m_operation *operation = NULL;
	if((operation = malloc(sizeof(struct m_operation))) == NULL) {
		eno = -5;
		goto err;
	}

	
	if((*operator)->init(var, option, value, operation) != 0) {
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
		ERROR("illegal operation %s \n", str);
		break;
	case -3:
		ERROR("do not support operator %s \n", op);
		break;
	case -4:
		ERROR("unknown var name %s \n", var_name);
		break;
	case -5:
		ERROR("malloc error @%s:%d \n", __func__, __LINE__);
		break;
	case -6:
		break;
		ERROR("parse %s error \n", str);
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

int m_operation_value(struct m_operation *operation)
{
	assert(operation && operation->op);

	return operation->op->value(operation);
}

void m_operation_clean(struct m_operation *operation)
{
	assert(operation && operation->op);

	operation->op->clean(operation);

	return;
}

