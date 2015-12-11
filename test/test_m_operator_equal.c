#include <stdio.h>
#include "../m_operator_equal.c"

int main()
{
	int a;

	struct m_variable vars[] = {
		{"var1", MT_UINT32, &a, NULL},
	};

	m_variable_list_register(vars, 1);

	struct m_operation op;

	m_operator_equal_init("equal(var1:2)", &op, head);

	a = 2;
	printf("%d \n", m_operator_equal_value(&op));

	a = 3;
	printf("%d \n", m_operator_equal_value(&op));


	m_operator_equal_init("equal(var2:2)", &op, head);

	return 0;
}
