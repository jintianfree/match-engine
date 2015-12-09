
#include "m_variable.h"

typedef int (*op_func_p)();
typedef int (*parse_func_p)();

struct m_operator {
	char name[16];
	parse_func_p parse_func;
	op_func_p op_func[MT_NUM_TYPES];
};

struct m_operator *m_ops[] = {
	NULL;
};


struct m_operation {
	struct m_variable *var;
	op_func_p *func;
	void *value;
	int value_len;
	void *operator_option;
};

/* op(var_name[][]:value) */
struct m_operation *m_operation_init(const char *s, struct m_variable_list *list)
{
	int len = 0;
	char *p = NULL;
	struct m_operator *op = NULL;

	assert(s);

	len = strlen(s);
	p = malloc(len + 1);	/* +1 for \0 */
	if(p == NULL) {
		goto err;
	}

	strcpy(p, s);
	p[len] = 0;

	char *op_name = strtok(p, "(");
	if(op_name == NULL) {
		goto err;
	}

	op = m_ops[0];
	while(op != NULL) {
		if(strcmp(op->name, op_name) == 0) {
			break;
		}
	}

	if(op) {
		op->parse_func(s);
	} else {
	}

	free(p);
err:
	if(p) {
		free(p);
		p = NULL;
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

