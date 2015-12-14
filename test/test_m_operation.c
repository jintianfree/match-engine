#include <stdio.h>
#include <string.h>
#include "m_variable.h"
#include "m_operation.h"

int main()
{
	int a  = 0;
    struct m_variable_list head = M_VARIABLE_LIST_NULL;
    struct m_operation *operation = NULL;
     
	struct m_variable vars[] = {
		{"var1", MST_ADDRESS, MRT_UINT32, &a, NULL, M_VARIABLE_LIST_NULL},
        {"", 0, 0, NULL, NULL, M_VARIABLE_LIST_NULL},
	};

	m_variable_list_register(&head, vars);

    operation = m_operation_init("equal(var1:2)", &head);
    if(operation) {
		a = 2;
        printf("%d \n", m_operation_value(operation));
		a = 1;
        printf("%d \n", m_operation_value(operation));

        m_operation_clean(operation);
    }

    return 0;
}
