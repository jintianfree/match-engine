#include <stdio.h>
#include "m_variable.h"
#include "../m_operator_null.c"

int main()
{
	uint32_t a  = 0;
	char str[12];
	size_t str_len = 0;
	char *p = NULL;
	size_t p_len = 0;

	struct m_operation op1;
	struct m_operation op2;
    struct m_operation op3;
    
	struct m_variable vars[] = {
		{"var1", MST_ADDRESS, MRT_UINT32, &a, NULL, M_VARIABLE_LIST_NULL},
   		{"var2", MST_ADDRESS, MRT_STRING, str, &str_len, M_VARIABLE_LIST_NULL},
   		{"var3", MST_POINTER_ADDRESS, MRT_STRING, &p, &p_len, M_VARIABLE_LIST_NULL},
        {"", 0, 0, NULL, NULL, M_VARIABLE_LIST_NULL},
	};

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
