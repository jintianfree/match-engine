


int op_int32_equal(void *var_addr, int var_len, void *value_addr, int value_len)
{
	return (int32_t *)var_addr == (int32_t *)value_addr;
}

int op_string_equal(void *var_addr, int var_len, void *value_addr, int value_len)
{
	if(var_len != value_len) {
		return 0;
	}

	return (strcmp((char *)var_addr, (char *value_addr)) == 0)
}

int op_string_contain(void *var_addr, int var_len, void *value_addr, int value_len)
{
	if(value_len > var_len) {
		return 0;
	}

	return (strstr((char *)var_addr, (char *value_addr)) != NULL)
}

void parse_sentence_func(const char *sentence, union sentence_info *info)
{
	if(sentence[0] == '0') {
		info->value = 0;
	} else {
		info->value = 1;
	}
}

int sentence_value_func(union sentence_info *info)
{
	return info->value;
}

struct expression_value {
	char *expression;
	int  value;
};


