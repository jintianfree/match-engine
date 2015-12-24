
#include "../m_rule.c"

struct test_struct {
	uint32_t var1;
	char var2[1024];
	size_t var2_len;
	char *var3;
	size_t var3_len;
};

int main()
{
	size_t base = 0;
	struct m_variable_table_manager *mgr = NULL;
	struct m_variable_table *table = NULL;
	struct m_variable_descr descrs[] = {
		{"var1", MST_MEMORY, MRT_UINT32, offsetof(struct test_struct, var1), 0},
		{"var2", MST_MEMORY, MRT_STRING, offsetof(struct test_struct, var2), offsetof(struct test_struct, var2_len)},
		{"var3", MST_ADDRESS, MRT_STRING, offsetof(struct test_struct, var3), offsetof(struct test_struct, var3_len)},
		{"", 0, 0, 0, 0},
	};

	mgr = m_variable_table_manager_init();
	base = m_variable_descr_register(mgr, descrs, sizeof(struct test_struct));

	table = m_variable_table_new(mgr);
	m_variable_table_zero(table);

	VAR_TABLE_2_STRUCT(struct test_struct, p, table, base);

	struct m_rule_list *head = m_rule_init("rule.ini", mgr);

	int i = 0;

	while(i++ < 10) {
		p->var1 = i;
		printf("==== %d \n", m_rule_match(head, table));
	}


	return 0;
}
