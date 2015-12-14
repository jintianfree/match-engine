#include <string.h>
#include "m_variable.h"

int main()
{
	int a = 0;
    char array[10];
    size_t array_len = 0;
	char *p = NULL;
	size_t p_len = 0;

    struct m_variable_list head = M_VARIABLE_LIST_NULL;
    
	struct m_variable vars[] = {
		{"var1", MST_ADDRESS, MRT_UINT32, &a, NULL, M_VARIABLE_LIST_NULL},
		{"var2", MST_ADDRESS, MRT_STRING, array, &array_len, M_VARIABLE_LIST_NULL},
		{"var3", MST_POINTER_ADDRESS, MRT_STRING, &p, &p_len, M_VARIABLE_LIST_NULL},
        {"", 0, 0, NULL, NULL, M_VARIABLE_LIST_NULL},
	};

	m_variable_list_register(&head, vars);
	m_variable_list_print(&head);

	a = 123;
	strcpy(array, "abc");
	array_len = strlen(array);

	p = array;
	p_len = 2;

	m_variable_list_print(&head);

	m_variable_list_unregister(&head, vars);
	m_variable_list_print(&head);

	return 0;
}
