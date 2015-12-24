#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "m_log.h"
#include "m_operation.h"
#include "m_logic_expression.h"
#include "m_logic_operation.h"

int parse_sentence_func(const char *sentence, union sentence_info *info, void *arg)
{
	struct m_operation *mop = NULL;	
	struct m_variable_table_manager *manager = (struct m_variable_table_manager *)arg;

	if((mop = m_operation_init(sentence, manager)) == NULL) {
		return -1;
	}

	info->data = mop;

	return 0;
}
	

int sentence_value_func(union sentence_info *info, void *arg)
{
	int value = 0;
	struct m_operation *mop = NULL;	
	struct m_variable_table *table = NULL;

	assert(info->data && arg);

	mop = (struct m_operation *)info->data;
	table = (struct m_variable_table *)arg;

	value = m_operation_value(mop, table);

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


struct m_logic_operation {
	struct m_logic_expression exp;
};

struct m_logic_operation *m_logic_operation_init(const char *exp, struct m_variable_table_manager *manager)
{
	assert(exp && manager);

	struct m_logic_operation *mlo = NULL;
	
	if((mlo = malloc(sizeof(struct m_logic_operation))) == NULL) {
		goto err;
	}

	if(m_logic_expression_init(&mlo->exp, &handle, exp, manager) != 0) {
		goto err;
	}

	return mlo;
err:
	ERROR("m_logic_operation_init error \n");
	return NULL;

}

int m_logic_operation_value(struct m_logic_operation *mlo, struct m_variable_table *table)
{
	assert(mlo);

	return m_logic_expression_evaluate(&mlo->exp, table);
}

void m_logic_operation_clean(struct m_logic_operation *mlo)
{
	assert(mlo);

	m_logic_expression_clean(&mlo->exp);

	free(mlo);

	return;
}

