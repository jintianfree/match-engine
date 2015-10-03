#include "../m_logic_expression.c"

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

int main()
{
	struct m_logic_expression exp;

	struct expression_value right_expression[] = {
		{"0", 0},
		{"1", 1},
		{"!0", 1},
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

	struct expression_value wrong_expression[] = {
		{" 1 &", -1},
		{"{1 & 1", -1},
		{"{1 & 1} & {1 & 0} ! {!{1 & 0} | {0 & 0}} | !{1 & 1 & 0}", -1},
		{NULL, 0}
	};

	int i = 0;
	while(right_expression[i].expression != NULL) {
		assert(m_logic_expression_init(&exp, right_expression[i].expression, parse_sentence_func) == 0);
		assert(m_logic_expression_evaluate(&exp, sentence_value_func) == right_expression[i].value);
		m_logic_expression_clean(&exp, NULL);
		i++;
	}

	i = 0;
	while(wrong_expression[i].expression != NULL) {
		assert(m_logic_expression_init(&exp, wrong_expression[i].expression, parse_sentence_func) < 0);
		i++;
	}

	return 0;
}
