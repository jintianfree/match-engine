#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "m_variable_table.h"
#include "m_logic_operation.h"

struct test_struct {
	uint32_t var1;
	char var2[1024];
	size_t var2_len;
	char *var3;
	size_t var3_len;
};


int main()
{
	size_t base = 0;
	struct m_variable_table_manager *mgr = NULL;
	struct m_variable_table *table = NULL;
	struct m_variable_descr descrs[] = {
		{"var1", MST_MEMORY, MRT_UINT32, offsetof(struct test_struct, var1), 0},
		{"var2", MST_MEMORY, MRT_STRING, offsetof(struct test_struct, var2), offsetof(struct test_struct, var2_len)},
		{"var3", MST_ADDRESS, MRT_STRING, offsetof(struct test_struct, var3), offsetof(struct test_struct, var3_len)},
		{"", 0, 0, 0, 0},
	};

	mgr = m_variable_table_manager_init();
	base = m_variable_descr_register(mgr, descrs, sizeof(struct test_struct));

	table = m_variable_table_new(mgr);
	m_variable_table_zero(table);

	VAR_TABLE_2_STRUCT(struct test_struct, p, table, base);

	struct m_logic_operation logic_op;
	m_logic_operation_init(&logic_op, "equal(var1:2)&equal(var2:abc)&!null(var3)", mgr);
	m_logic_operation_value(&logic_op, table);

	p->var1 = 2;
	assert(m_logic_operation_value(&logic_op, table) == 0);

	strcpy(p->var2, "abc");
	p->var2_len = 3;
	assert(m_logic_operation_value(&logic_op, table) == 0);

	p->var3 = p->var2;
	p->var3_len = 3;
	assert(m_logic_operation_value(&logic_op, table) == 1);

	m_logic_operation_clean(&logic_op);

	m_variable_descr_unregister(mgr, descrs);
	m_variable_table_manager_clean(mgr);

	return 0;
}
