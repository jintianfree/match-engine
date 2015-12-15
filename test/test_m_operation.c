#include <stdio.h>
#include <string.h>
#include "m_variable.h"
#include "m_operation.h"

int main()
{
	int a = 0;
	struct m_variable_list head = M_VARIABLE_LIST_NULL;
	struct m_operation *op1 = NULL;
	struct m_operation *op2 = NULL;
    //struct m_operation *op3 = NULL;

	struct m_variable vars[] = {
		{"var1", MST_ADDRESS, MRT_UINT32, &a, NULL, M_VARIABLE_LIST_NULL},
		{"", 0, 0, NULL, NULL, M_VARIABLE_LIST_NULL},
	};

	m_variable_list_register(&head, vars);

	op1 = m_operation_init("equal(var1:2)", &head);
	op2 = m_operation_init("null(var1)", &head);
	//op3 = m_operation_init("equal(var1:)", &head);
    
	if(op1 && op2) {
		a = 0;
		printf("%d \n", m_operation_value(op1));
		printf("%d \n", m_operation_value(op2));
		a = 1;
		printf("%d \n", m_operation_value(op1));
		printf("%d \n", m_operation_value(op2));
		a = 2;
		printf("%d \n", m_operation_value(op1));
		printf("%d \n", m_operation_value(op2));

		m_operation_clean(op1);
		m_operation_clean(op2);
	}

	return 0;
}
