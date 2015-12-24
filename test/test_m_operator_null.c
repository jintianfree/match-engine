#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "m_common.h"
#include "m_operation.h"
#include "m_variable_table.h"
#include "m_operator_null.h"

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
	struct m_variable_table_manager *mgr = NULL;
	mgr = m_variable_table_manager_init();

	size_t base = m_variable_descr_register(mgr, vars, sizeof(struct test_struct));

	struct m_variable_table *table = m_variable_table_new(mgr);
	m_variable_table_zero(table);

	struct m_operation op1;
	struct m_operation op2;
	struct m_operation op3;
    
	m_operator_null.init(	/* null(uint32) */
			m_variable_descr_find_by_name(mgr, "uint32"),
			NULL, NULL, &op1);

	m_operator_null.init(	/* null(array32) */
			m_variable_descr_find_by_name(mgr, "array32"),
			NULL, NULL, &op2);

	m_operator_null.init(	/* null(pointer) */
			m_variable_descr_find_by_name(mgr, "pointer"),
			NULL, NULL, &op3);

	printf("%d \n", m_operator_null.value(&op1, table));
	printf("%d \n", m_operator_null.value(&op2, table));
	printf("%d \n", m_operator_null.value(&op3, table));

	VAR_TABLE_2_STRUCT(struct test_struct, p, table, base);
	p->uint32 = 32;
	strcpy(p->array32, "abc");
	p->array32_len = 3;
	p->pointer = p->array32;
	p->pointer_len = 3;

	printf("%d \n", m_operator_null.value(&op1, table));
	printf("%d \n", m_operator_null.value(&op2, table));
	printf("%d \n", m_operator_null.value(&op3, table));

	m_operator_null.clean(&op1);
	m_operator_null.clean(&op2);
	m_operator_null.clean(&op3);

	m_variable_descr_unregister(mgr, vars);
	m_variable_table_manager_clean(mgr);

	return 0;
}
