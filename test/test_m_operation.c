#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "m_variable.h"
#include "m_operation.h"

int main()
{
	int var1 = 0;
	char var2[1024] = {0};
	size_t var2_len = 0;
	char *var3 = NULL;
	size_t var3_len = 0;

	struct m_variable_list head = M_VARIABLE_LIST_NULL;

	struct m_variable vars[] = {
		{"var1", MST_ADDRESS, MRT_UINT32, &var1, NULL, M_VARIABLE_LIST_NULL},
		{"var2", MST_ADDRESS, MRT_STRING, &var2, &var2_len, M_VARIABLE_LIST_NULL},
		{"var3", MST_POINTER_ADDRESS, MRT_STRING, &var3, &var3_len, M_VARIABLE_LIST_NULL},
		{"", 0, 0, NULL, NULL, M_VARIABLE_LIST_NULL},
	};

	m_variable_list_register(&head, vars);


	struct m_operation *op1 = NULL;
	struct m_operation *op2 = NULL;
	struct m_operation *op3 = NULL;
	struct m_operation *op4 = NULL;
	struct m_operation *op5 = NULL;

	assert((op1 = m_operation_init("equa", &head)) == NULL);
	assert((op1 = m_operation_init("equal", &head)) == NULL);
	assert((op1 = m_operation_init("equal(", &head)) == NULL);
	assert((op1 = m_operation_init("equal(var", &head)) == NULL);
	assert((op1 = m_operation_init("equal(var1", &head)) == NULL);
	assert((op1 = m_operation_init("equal((var1", &head)) == NULL);
	assert((op1 = m_operation_init("equal((var1)", &head)) == NULL);
	assert((op1 = m_operation_init("equal(var1:)", &head)) == NULL);
	assert((op1 = m_operation_init("equal(var1::)", &head)) == NULL);
	assert((op1 = m_operation_init("equal(var1:1", &head)) == NULL);
	assert((op1 = m_operation_init("equal(var1:1))", &head)) == NULL);


	assert((op1 = m_operation_init("equal(var1:2)", &head)) != NULL);
	assert((op2 = m_operation_init("null(var1)", &head)) != NULL);
	assert((op3 = m_operation_init("null(var2)", &head)) != NULL);
	assert((op4 = m_operation_init("null(var3)", &head)) != NULL);
	assert((op5 = m_operation_init("equal(var3:equal(var3:abc))", &head)) != NULL);

	var1 = 2;
	var3 = var2;
	strcpy(var2, "equal(var3:abc)");
	var3_len = var2_len = strlen("equal(var3:abc)");

	assert(m_operation_value(op1) == 1);
	assert(m_operation_value(op2) == 0);
	assert(m_operation_value(op3) == 0);
	assert(m_operation_value(op4) == 0);
	assert(m_operation_value(op5) == 1);


	m_operation_clean(op1);
	m_operation_clean(op2);
	m_operation_clean(op3);
	m_operation_clean(op4);
	m_operation_clean(op5);

	m_variable_list_unregister(&head, vars);

	return 0;
}
