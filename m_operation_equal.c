
struct equal_operator_option {
	int start;
	int len;
	int case_sensitive;
};

/* equal(var_name[][]: value) */
int m_op_equal_parse(const char *s, struct m_operation *operation, struct m_variable_list *head)
{
	struct m_variable *var = NULL;

	len = strlen(s);
	p = malloc(len + 1);	/* +1 for \0 */
	if(p == NULL) {
		goto err;
	}

	strcpy(p, s);
	p[len] = 0;


	char *op = strtok(p, "(:)";
	char *var_name = strtok(NULL, "(:)";
	char *value = strtok(NULL, "(:)";

	var = name_2_var(var_name);

	operation->var = var;

	switch(var->type) {
	case MT_UINT8:
		operation->func = m_op_equal_uint8;
		operation->value = atoi(value);
		operation->value_len = 1;
		break;
	case MT_UINT16:
		operation->func = m_op_equal_uint16;
		operation->value = atoi(value);
		operation->value_len = 2;
		break;
	case MT_UINT32:
		operation->func = m_op_equal_uint32;
		operation->value = atoi(value);
		operation->value_len = 4;
		break;
	}
	
	operation->operator_option = NULL;

	return 0;
}

void m_op_equal_clean(struct m_operation *operation)
{
}
/* equal great little contain range null */

int m_op_equal_uint8(void *var, int var_len, void *value, int value_len)
{
	return (*(uint8_t *)var == *(uint8_t *)value);
}

int m_op_equal_uint16(void *var, int var_len, void *value, int value_len)
{
	return (*(uint16_t *)var == *(uint16_t *)value);
}

int m_op_equal_uint32(void *var, int var_len, void *value, int value_len)
{
	return (*(uint32_t *)var == *(uint32_t *)value);
}

int m_op_equal_uint64(void *var, int var_len, void *value, int value_len)
{
	return (*(uint64_t *)var == *(uint64_t *)value);
}

int m_op_equal_int8(void *var, int var_len, void *value, int value_len)
{
	return (*(int8_t *)var == *(int8_t *)value);
}

int m_op_equal_int16(void *var, int var_len, void *value, int value_len)
{
	return (*(int16_t *)var == *(int16_t *)value);
}

int m_op_equal_int32(void *var, int var_len, void *value, int value_len)
{
	return (*(int32_t *)var == *(int32_t *)value);
}

int m_op_equal_int64(void *var, int var_len, void *value, int value_len)
{
	return (*(int64_t *)var == *(int64_t *)value);
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
		m_op_equal_uint8,
		m_op_equal_uint16,
		m_op_equal_uint32,
		m_op_equal_uint64,
		m_op_equal_int8,
		m_op_equal_int16,
		m_op_equal_int32,
		m_op_equal_int64,
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

