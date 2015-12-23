#include <stdio.h>
#include "m_common.h"
#include "m_variable_table.h"
#include "../m_operator_null.c"

struct test_struct {
	uint32_t uint32;
	char array32[32];
	size_t array32_len;
	char *pointer;
	size_t pointer_len;
};

struct m_variable_descr vars[] = {
	{"uint32", MST_MEMORY, MRT_UINT32, offsetof(struct test_struct, uint32), 0},
	{"array32", MST_MEMORY, MRT_STRING, offsetof(struct test_struct, array32), offsetof(struct test_struct, array32_len)},
	{"pointer", MST_ADDRESS, MRT_STRING, offsetof(struct test_struct, pointer), offsetof(struct test_struct, pointer_len)},
	{"", 0, 0, 0, 0},
};

int main()
{
	/*
	 *
	 */
	struct m_variable_table_manager mgr ;
	m_variable_table_manager_init(&mgr);
	struct m_variable_table *table = m_variable_table_new(&mgr);

	size_t base = m_variable_descr_register(&mgr, vars, sizeof(struct test_struct));
	struct test_struct *p = table->base + base;

	m_variable_table_print(table);
	m_variable_descr_unregister(&mgr, vars2);
	m_variable_table_manager_clean(&mgr);

	struct m_operation op1;
	struct m_operation op2;
	struct m_operation op3;
    
	m_operator_null_init(&vars[0], NULL, NULL, &op1);
	m_operator_null_init(&vars[1], NULL, NULL, &op2);
	m_operator_null_init(&vars[2], NULL, NULL, &op3);

	a = 2;
	printf("%d \n", m_operator_null_value(&op1));
	a = 0;
	printf("%d \n", m_operator_null_value(&op1));

	str_len = 2;
	printf("%d \n", m_operator_null_value(&op2));
	str_len = 0;
	printf("%d \n", m_operator_null_value(&op2));

	p = NULL;
	printf("%d \n", m_operator_null_value(&op3));

	p = str;
	p_len = 0;
	printf("%d \n", m_operator_null_value(&op3));
	p_len = 1;
	printf("%d \n", m_operator_null_value(&op3));

	return 0;
}
