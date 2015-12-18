#include <assert.h>
#include "m_operation.h"
#include "m_logic_expression.h"
#include "m_logic_operation.h"

int parse_sentence_func(const char *sentence, union sentence_info *info, void *arg)
{
	struct m_operation *mop = NULL;	
	struct m_variable_list *head = (struct m_variable_list *)arg;

	if((mop = m_operation_init(sentence, head)) == NULL) {
		return -1;
	}

	info->data = mop;

	return 0;
}
	

int sentence_value_func(union sentence_info *info)
{
	int value = 0;
	struct m_operation *mop = NULL;	

	assert(info->data);

	mop = (struct m_operation *)info->data;

	value = m_operation_value(mop);

	return value;
}

void sentence_clean_func(union sentence_info *info)
{
	struct m_operation *mop = NULL;	

	assert(info->data);

	mop = (struct m_operation *)info->data;

	m_operation_clean(mop);

	return;
}

struct m_sentence_handle handle = {
	.parse = parse_sentence_func,
	.clean = sentence_clean_func,
	.value = sentence_value_func,
};


int m_logic_operation_init(struct m_logic_operation *mlo, const char *exp, struct m_variable_list *head)
{
	assert(mlo && exp && head);

	return m_logic_expression_init(&mlo->exp, &handle, exp, head);
}

int m_logic_operation_value(struct m_logic_operation *mlo)
{
	assert(mlo);

	return m_logic_expression_evaluate(&mlo->exp);
}

void m_logic_operation_clean(struct m_logic_operation *mlo)
{
	assert(mlo);

	m_logic_expression_clean(&mlo->exp);

	return;
}

