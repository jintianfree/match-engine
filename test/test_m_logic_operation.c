#include <string.h>
#include "m_variable.h"
#include "m_logic_operation.h"

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


	struct m_logic_operation logic_op;
	m_logic_operation_init(&logic_op, "equal(var1:2)&equal(var2:abc)&!null(var3)", &head);
	m_logic_operation_value(&logic_op);

	var1=2;
	m_logic_operation_value(&logic_op);

	strcpy(var2, "abc");
	var2_len = 3;
	m_logic_operation_value(&logic_op);

	var3 = var2;
	var3_len = 3;
	m_logic_operation_value(&logic_op);

	m_variable_list_unregister(&head, vars);

	return 0;
}
