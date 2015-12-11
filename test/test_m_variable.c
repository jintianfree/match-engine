
#include "m_variable.h"

int main()
{
	int a;
	char *p = NULL;
    char array[] = {'a', 'b', 'c', '\0'};
    p = array;
    size_t len;

    struct m_variable_list head = M_VARIABLE_LIST_NULL;
    
    len = 3;

	struct m_variable vars[] = {
		{"var1", MST_ADDRESS, MRT_UINT32, (void *)&a, &len, M_VARIABLE_LIST_NULL},
		{"var2", MST_ADDRESS, MRT_STRING, (void *)array, &len, M_VARIABLE_LIST_NULL},
		{"var3", MST_POINTER_ADDRESS, MRT_STRING, (void *)&p, &len, M_VARIABLE_LIST_NULL},
		{"var4", MST_ADDRESS, MRT_UINT8, (void *)&a, &len, M_VARIABLE_LIST_NULL},
		{"var5", MST_ADDRESS, MRT_UINT8, (void *)&a, &len, M_VARIABLE_LIST_NULL},
        {"", 0, 0, NULL, NULL, M_VARIABLE_LIST_NULL},
	};

	m_variable_list_register(&head, vars);
	m_variable_list_register(&head, vars);
	m_variable_list_print(&head);
	m_variable_list_unregister(&head, vars);
	/* m_variable_list_unregister(&head, vars); */
	m_variable_list_print(&head);

	return 0;
}
