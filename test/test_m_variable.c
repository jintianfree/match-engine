
#include "m_variable.h"

int main()
{
	int a;
	size_t len;

	void **p = NULL;

	*p = 0x010;

	struct m_variable vars[] = {
		{"var1", MT_UINT8, (void *)&a, &len},
		{"var2", MT_UINT8, (void *)&a, &len},
		{"var3", MT_UINT8, (void *)&a, &len},
		{"var4", MT_UINT8, (void *)&a, &len},
		{"var5", MT_UINT8, (void *)&a, &len},
	};

	m_variable_list_register(vars, 5);
	m_variable_list_register(vars, 5);
	m_variable_list_print();
	m_variable_list_unregister(vars, 5);
	m_variable_list_unregister(vars, 5);
	m_variable_list_print();

	return 0;
}
