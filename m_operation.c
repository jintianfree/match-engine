
#include "m_variable.h"

typedef int (*op_func_p)();
typedef int (*parse_func_p)();

struct m_operation {
	char name[16];
	parse_func_p parse_func;
	op_func_p op_func[MT_NUM_TYPES];
};

struct m_op_descr {
	int start;
	int len;
	int nocase;
};

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

/* op[][]:(value) */
int m_operation_parse(const char *s)
{
	int len = 0;

	len = strlen(s);

	char *p = malloc(len + 1);

	strcpy(p, s);
	p[len] = 0;


	strtok(p, "[:");
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

