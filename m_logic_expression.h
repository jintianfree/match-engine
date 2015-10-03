#ifndef _M_LOGIC_EXPRESSION_C_
#define _M_LOGIC_EXPRESSION_C_

struct m_logic_sentence;

union sentence_info {
	int value;
	void *data;
	char operator;
};

struct m_logic_expression {
	struct m_logic_sentence *root;
};

typedef void (*parse_sentence_func_p)(const char *sentence, union sentence_info *info);
typedef void (*clean_sentence_func_p)(union sentence_info *info);
typedef int  (*sentence_value_func_p)(union sentence_info *info);

int  m_logic_expression_init(struct m_logic_expression *lexp, const char *exp, parse_sentence_func_p func);
void m_logic_expression_clean(struct m_logic_expression *lexp, clean_sentence_func_p func);
int  m_logic_expression_evaluate(struct m_logic_expression *lexp, sentence_value_func_p func);

#endif
