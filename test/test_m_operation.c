#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "m_variable_table.h"
#include "m_operation.h"

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

	VAR_TABLE_2_STRUCT(struct test_struct, p, table, base);

	struct m_operation *op1 = NULL;
	struct m_operation *op2 = NULL;
	struct m_operation *op3 = NULL;
	struct m_operation *op4 = NULL;
	struct m_operation *op5 = NULL;

	assert((op1 = m_operation_init("equa", mgr)) == NULL);
	assert((op1 = m_operation_init("equal", mgr)) == NULL);
	assert((op1 = m_operation_init("equal(", mgr)) == NULL);
	assert((op1 = m_operation_init("equal(var", mgr)) == NULL);
	assert((op1 = m_operation_init("equal(var1", mgr)) == NULL);
	assert((op1 = m_operation_init("equal((var1", mgr)) == NULL);
	assert((op1 = m_operation_init("equal((var1)", mgr)) == NULL);
	assert((op1 = m_operation_init("equal(var1:)", mgr)) == NULL);
	assert((op1 = m_operation_init("equal(var1::)", mgr)) == NULL);
	assert((op1 = m_operation_init("equal(var1:1", mgr)) == NULL);
	assert((op1 = m_operation_init("equal(var1:1))", mgr)) == NULL);

	assert((op1 = m_operation_init("equal(var1:2)", mgr)) != NULL);
	assert((op2 = m_operation_init("null(var1)", mgr)) != NULL);
	assert((op3 = m_operation_init("null(var2)", mgr)) != NULL);
	assert((op4 = m_operation_init("null(var3)", mgr)) != NULL);
	assert((op5 = m_operation_init("equal(var3:equal(var3:abc))", mgr)) != NULL);

	p->var1 = 2;
	p->var3 = p->var2;
	strcpy(p->var2, "equal(var3:abc)");
	p->var3_len = p->var2_len = strlen("equal(var3:abc)");

	assert(m_operation_value(op1, table) == 1);
	assert(m_operation_value(op2, table) == 0);
	assert(m_operation_value(op3, table) == 0);
	assert(m_operation_value(op4, table) == 0);
	assert(m_operation_value(op5, table) == 1);

	m_operation_clean(op1);
	m_operation_clean(op2);
	m_operation_clean(op3);
	m_operation_clean(op4);
	m_operation_clean(op5);

	m_variable_descr_unregister(mgr, descrs);
	m_variable_table_manager_clean(mgr);

	return 0;
}
