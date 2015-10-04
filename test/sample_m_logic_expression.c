
#include "../m_logic_expression.c"

int op_int32_equal(void *var_addr, int var_len, void *value_addr, int value_len)
{
	printf("%d %d \n", *(int32_t *)var_addr, *(int32_t *)value_addr);
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


char string_var[1024];
int  int_var = 0;

typedef int (*op_func_p)(void *var_addr, int var_len, void *value_addr, int value_len);

struct xx {
	void *var_addr;
	int var_len;
	void *value_addr;
	int value_len;
	op_func_p op_func;
};

/* parse equal(int_var:0)  equal(string_var:abc) contain(string_var:abc) */
void parse_sentence_func(const char *sentence, union sentence_info *info)
{
	char buf[2048];
	strncpy(buf, sentence, 2048);
	buf[2047] = 0;

	printf("%s \n", sentence);
	char *op = strtok(buf, "(:)");
	printf("op %s \n", op);
	char *var_name = strtok(NULL, "(:)");
	printf("var_name %s \n", var_name);
	char *var_value = strtok(NULL, "(:)");
	printf("var_value %s \n", var_value);

	struct xx *x = malloc(sizeof(struct xx));
	if(!x) {
		printf("ERROR: malloc error \n");
		return;
	}

	if(!op || !var_name || !var_value) {
		printf("ERROR: unknown sentence (%s) \n", sentence);
		return;
	} else {
		printf("%s %s %s \n", op, var_name, var_value);
	}

	if(strcmp(op, "equal") == 0) {
		if(strcmp(var_name, "string_var") == 0) {
			x->var_addr = string_var;
			x->var_len = 0;
			x->value_addr = strdup(var_value);
			x->value_len = strlen(var_value);
			x->op_func = op_string_equal;
		} else if(strcmp(var_name, "int_var") == 0) {
			x->var_addr = &int_var;
			x->var_len = 4;
			x->value_addr = malloc(sizeof(int32_t));
			*(int32_t *)(x->value_addr) = atoi(var_value);
			x->value_len = 4;
			x->op_func = op_int32_equal;
		} else {
			printf("ERROR: unknown var_nam (%s) in (%s) \n", var_name, sentence);
		}
	} else if(strcmp(op, "contain") == 0) {
			x->var_addr = string_var;
			x->var_len = 0;
			x->value_addr = strdup(var_value);
			x->value_len = strlen(var_value);
			x->op_func = op_string_contain;
	} else {
		printf("ERROR: unknown method (%s) in (%s) \n", op, sentence);
	}

	info->data = x;

	return;
}

int sentence_value_func(union sentence_info *info)
{
	struct xx *x = (struct xx *)info->data;
	return x->op_func(x->var_addr, x->var_len, x->value_addr, x->value_len);
}


int main()
{
	struct m_logic_expression exp;
	
	m_logic_expression_init(&exp, "equal(int_var:0)|!equal(int_var:0)", parse_sentence_func);
	int_var = 0;
	m_logic_expression_evaluate(&exp, sentence_value_func);

	int_var = 1;
	m_logic_expression_evaluate(&exp, sentence_value_func);
	//m_logic_expression_clean(&exp, NULL);
	return 0;
}
