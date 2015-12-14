#include <stdio.h>
#include "m_variable.h"
#include "../m_operator_equal.c"

int main()
{
	int a  = 0;
	struct m_operation op;
    
	struct m_variable vars[] = {
		{"var1", MST_ADDRESS, MRT_UINT32, &a, NULL, M_VARIABLE_LIST_NULL},
   		{"var2", MST_ADDRESS, MRT_INT32, &a, NULL, M_VARIABLE_LIST_NULL},
        {"", 0, 0, NULL, NULL, M_VARIABLE_LIST_NULL},
	};

	m_operator_equal_init(&vars[0], NULL, "2", &op);

	a = 2;
	printf("%d \n", m_operator_equal_value(&op));

	a = 3;
	printf("%d \n", m_operator_equal_value(&op));

	m_operator_equal_init(&vars[1], NULL, "5", &op);

	return 0;
}
