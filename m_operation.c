
#include "m_variable.h"

struct m_operator *operators[] = {
	NULL;
};

/* op(var_name[][]:value) */
struct m_operation *m_operation_init(const char *s, struct m_variable_list *list)
{
	int eno = 0;
	char *p = NULL;
	char *operator_name = NULL;
	struct m_operator *operator = NULL;
	struct m_operation *operation = NULL;

	assert(s);

	p = strdup(s);
	if(p == NULL) {
		eno = -1;
		goto err;
	}

	if((operator_name = strtok(p, "(")) == NULL)
		eno = -2;
		goto err;
	}

	operator = operators;
	while(operator != NULL) {
		if(strcmp(operator->name, operator_name) == 0) {
			break;
		}

		operator++;
	}

	if(!operator) {
		eno = -3;
		goto err;
	}

	if((operation = malloc(sizeof(struct m_operation))) == NULL) {
		eno = -4;
		goto err;
	}

	operation->op = operator;
	
	if(operator->init(s, operation, list) != 0) {
		eno = -5;
		goto err;
	}

	if(p) {
		free(p);
		p = NULL;
	}

	return 0;
err:
	switch(eno) {
	case -1:
		break;
	case -2:
		break;
	case -3:
		break;
	case -4:
		break;
	case -5:
		break;
	}

	if(p) {
		free(p);
		p = NULL;
	}

	if(operation) {
		free(operation);
		operation = NULL;
	}

	return -1;
}

int m_operation_value(struct m_operation *operation)
{
	return 0;
}

void m_operation_clean(struct m_operation *operation)
{
	return;
}

