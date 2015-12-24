#ifndef _M_LOGIC_EXPRESSION_C_
#define _M_LOGIC_EXPRESSION_C_

struct m_logic_sentence;

union sentence_info {
	int value;
	void *data;
	char op;	/* ! or & or |  */
};

typedef int  (*sentence_parse)(const char *sentence, union sentence_info *info, void *arg);
typedef void (*sentence_clean)(union sentence_info *info);
typedef int  (*sentence_value)(union sentence_info *info, void *arg);

struct m_sentence_handle {
	sentence_parse parse;
	sentence_clean clean;
	sentence_value value;
};

struct m_logic_expression {
	struct m_logic_sentence *root;
	struct m_sentence_handle *handle;
};

/* WARN: & | ! {} blank   must use \& \| \! \{ \}  \  instead */
int  m_logic_expression_init(struct m_logic_expression *lexp, struct m_sentence_handle *handle, const char *exp, void *arg);
void m_logic_expression_clean(struct m_logic_expression *lexp);
int  m_logic_expression_evaluate(struct m_logic_expression *lexp, void *arg);

#endif
