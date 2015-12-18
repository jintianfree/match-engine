#include "../m_logic_expression.c"

int parse_sentence_func(const char *sentence, union sentence_info *info, void *arg)
{
	(void)arg;
	if(sentence[0] == '0') {
		/* info->value = 0; */
		info->data = NULL;	/* test memory leak */
	} else if(sentence[0] == '1'){
		/* info->value = 1; */
		if((info->data = malloc(5)) == NULL) {
			return -1;
		}
	} else {
		return -1;
	}

	return 0;
}

int sentence_value_func(union sentence_info *info)
{
	return (info->data != NULL);
}

void sentence_clean_func(union sentence_info *info)
{
	if(info->data) {
		free(info->data);
		info->data = NULL;
	}
}

struct m_sentence_handle handle = {
	.parse = parse_sentence_func,
	.clean = sentence_clean_func,
	.value = sentence_value_func,
};

int main()
{
	struct m_logic_expression exp;

	struct {
		char *expression;
		int  value;
	} right_expression[] = {
		{"0", 0},
		{"{0}", 0},
		{"1", 1},
		{"{1}", 1},
		{"!0", 1},
		{"{!0}", 1},
		{"!{0}", 1},
		{"!1", 0},
		{"0 & 0", 0},
		{"0 & 1", 0},
		{"1 & 1", 1},
		{"0 | 0", 0},
		{"0 | 1", 1},
		{"1 | 1", 1},
		{"1 | 0 & 0", 1}, 		/* & > | */
		{"1 | {0 & 0}", 1},		/* & > | */
		{"{1 | 0} & 0", 0},	/* & > | */
		{"!1 & 0", 0},
		{"!{1 & 0}", 1},
		{"!{0 | 1}", 0},
		{"{1 & 1} & {1 & 0} & {!{1 & 0} | {0 & 0}} | !{0 & 1 & 1}", 1},
		{"{1 & 1} & {1 & 0} | {!{1 & 0} | {0 & 0}} | !{1 & 1 & 0}", 1},
		{NULL, 0}
	};

	struct {
		char *expression;
		int  value;
	} wrong_expression[] = {
		{"", -1},
		{" ", -1},
		{"  1 &  ", -1},
		{" 1 &", -1},
		{"{1 & 1", -1},
		{"{1 & 1} & {1 & 0} ! {!{1 & 0} | {0 & 0}} | !{1 & 1 & 0}", -1},
		{"a", -1},
		{"{a}", -1},
		{"{ 1 & b }", -1},
		{"{ 1 & 1 & b }", -1},
		{"1 & 1 & 1 & 1 & 1 & 1 & b", -1},
		{NULL, 0}
	};

	int i = 0;
	while(right_expression[i].expression != NULL) {
		assert(m_logic_expression_init(&exp, &handle, right_expression[i].expression, NULL) == 0);
		assert(m_logic_expression_evaluate(&exp) == right_expression[i].value);
		m_logic_expression_clean(&exp);
		i++;
	}

	i = 0;
	while(wrong_expression[i].expression != NULL) {
		assert(m_logic_expression_init(&exp, &handle, wrong_expression[i].expression, NULL) < 0);
		i++;
	}

	return 0;
}
