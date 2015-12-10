
struct equal_operator_option {
	int start;
	int len;
	int case_sensitive;
};

/* "AB EF CD"  -> {AB,EF,CD} */
int bytes_string_2_bytes(char *str, s_len, uint8_t bytes[], int b_len)
{
	char *p = NULL;
	
	for(p = strtok(str, ' \t'); p != NULL; p = strtok(NULL, ' \t')) {
		strtol(p, NULL, 16);
		if(errno == ERANGE)
	}
	
err:
	return -1;
}

/* TODO: do not support option now! equal(var_name[][]: value) */
int m_op_equal_parse(const char *s, struct m_operation *operation, struct m_variable_list *head)
{
	int eno = 0;
	int len = 0;
	char *p = NULL;
	struct m_variable *var = NULL;

	len = strlen(s);

	p = malloc(len + 1);	/* +1 for \0 */
	if(p == NULL) {
		eno = -1;
		goto err;
	}

	strcpy(p, s);
	p[len] = 0;

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

	char *value = strtok(NULL, "(:)");
	if(value == NULL) {
		eno = -2;
		goto err;
	}

	if((var = name_2_var(var_name)) == NULL) {
		eno  = -3;
		goto err;
	}

	operation->var = var;

	switch(var->type) {
	case MT_UINT32:
		operation->func = m_op_equal_uint32;
		operation->value = atoi(value);
		operation->value_len = 4;
		break;
	case MT_STRING:
		operation->func = m_op_equal_string;
		operation->value = atoi(value);
		operation->value_len = 4;
		break;
	case MT_BYTES:
		operation->func = m_op_equal_bytes;
		operation->value = atoi(value);
		operation->value_len = 4;
		break;
	default:
		eno = -4;
		goto err;
		break;
	}
	
	operation->operator_option = NULL;

	return 0;
err:
	return -1;
}

void m_op_equal_clean(struct m_operation *operation)
{
}

int m_op_equal_uint32(void *var, int var_len, void *value, int value_len)
{
	return (*(uint32_t *)var == *(uint32_t *)value);
}


int m_op_equal_string(void *var, int var_len, void *value, int value_len)
{
	if(var_len != value_len) {
		return 0;
	}

	return (strncmp((char *)var, (char *)value, var_len) == 0);
}

int m_op_equal_bytes(void *var, int var_len, void *value, int value_len)
{
	if(var_len != value_len) {
		return 0;
	}

	return (memcmp(var, value, var_len) == 0);
}

struct m_operation m_op_equal = {
	.name = "equal",
	.parse_func = NULL,
	.op_func = {
		NULL,
		NULL,
		m_op_equal_uint32,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		m_op_equal_string,
		m_op_equal_bytes,
		NULL,
		NULL,
	},
};

