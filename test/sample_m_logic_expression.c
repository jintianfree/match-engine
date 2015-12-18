#include <stdint.h>
#include "../m_logic_expression.c"

typedef int (*op_func_p)(void *var_addr, int var_len, void *value_addr, int value_len);

struct var_descr {
	char var_name[64];
	void *var_addr;
	int *var_len;
	int var_type;	/* 0 int 1 string */
};

struct op_info {
	struct var_descr *var;
	void *value_addr;
	int value_len;
	op_func_p op_func;
};

int op_int32_equal(void *var_addr, int var_len, void *value_addr, int value_len)
{
	(void)var_len;
	(void)value_len;

	return *(int32_t *)var_addr == *(int32_t *)value_addr;
}

int op_string_equal(void *var_addr, int var_len, void *value_addr, int value_len)
{
	if(var_len != value_len) {
		return 0;
	}

	return (strcmp((char *)var_addr, (char *)value_addr) == 0);
}

int op_string_contain(void *var_addr, int var_len, void *value_addr, int value_len)
{
	if(value_len > var_len) {
		return 0;
	}

	return (strstr((char *)var_addr, (char *)value_addr) != NULL);
}

/* parse equal(int_var:0)  equal(string_var:abc) contain(string_var:abc) */
int parse_sentence_func(const char *sentence, union sentence_info *info, void *arg)
{
	char buf[2048];
	struct var_descr *vars = (struct var_descr *)arg;

	strncpy(buf, sentence, 2048);
	buf[2047] = 0;

	char *op = strtok(buf, "(:)");
	char *var_name = strtok(NULL, "(:)");
	char *var_value = strtok(NULL, "(:)");

	if(!op || !var_name || !var_value) {
		printf("ERROR: unknown sentence (%s) \n", sentence);
		return -1;
	} else {
		printf("%s %s %s \n", op, var_name, var_value);
	}

	struct op_info *opi = malloc(sizeof(struct op_info));
	if(!opi) {
		printf("ERROR: malloc error \n");
		return -1;
	}

	while(vars->var_addr != NULL) {
		if(strcmp(vars->var_name, var_name) == 0) {
			break;
		}
		vars++;
	}

	if(vars == NULL) {
		printf("ERROR: unknown var (%s) in (%s) \n", var_name, sentence);
	}

	if(strcmp(op, "equal") == 0) {
		switch(vars->var_type) {
		case 1: /* string */
			opi->var = vars;
			opi->value_addr = strdup(var_value);
			opi->value_len = strlen(var_value);
			opi->op_func = op_string_equal;
			break;
		case 0: /* int */
			opi->var = vars;
			opi->value_addr = malloc(sizeof(int32_t));
			*(int32_t *)(opi->value_addr) = atoi(var_value);
			opi->value_len = 4;
			opi->op_func = op_int32_equal;
			break;
		default:
			break;
			printf("ERROR: unknown var_nam (%s) in (%s) \n", var_name, sentence);
		}

	} else if(strcmp(op, "contain") == 0) {
		switch(vars->var_type) {
		case 1:
			opi->var = vars;
			opi->value_addr = strdup(var_value);
			opi->value_len = strlen(var_value);
			opi->op_func = op_string_contain;
			break;
		default:
			printf("ERROR: contain support string only (%s) \n", sentence);
		}
	} else {
		printf("ERROR: unknown method (%s) in (%s) \n", op, sentence);
	}

	info->data = opi;

	return 0;
}

int sentence_value_func(union sentence_info *info)
{
	struct op_info *opi = (struct op_info *)info->data;
	return opi->op_func(opi->var->var_addr, *(opi->var->var_len), opi->value_addr, opi->value_len);
}

struct m_sentence_handle handle = {
	.parse = parse_sentence_func,
	.clean = NULL,
	.value = sentence_value_func,
};

int main()
{
	struct m_logic_expression exp;
	
	int int_var = 0;
	int  int_var_len;
	char string_var[1024];
	int string_var_len = 0;

	struct var_descr vars[] = {
		{"int_var", &int_var, &int_var_len, 0},
		{"string_var", string_var, &string_var_len, 1},
		{"", NULL, NULL, 0},
	};

	m_logic_expression_init(&exp, &handle, "(equal(int_var:0)|!equal(string_var:123))&(contain(string_var:2)|equal(int_var:1)", vars);
	int_var = 0;
	strcpy(string_var, "123");
	string_var_len = strlen(string_var);

	m_logic_expression_evaluate(&exp);

	int_var = 1;
	m_logic_expression_evaluate(&exp);
	m_logic_expression_clean(&exp);
	return 0;
}

