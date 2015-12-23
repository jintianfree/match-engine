#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <m_common.h>
#include "m_variable_table.h"

struct test_struct {
	uint32_t uint32;
	char array32[32];
	size_t array32_len;
	char *pointer;
	size_t pointer_len;
};

struct test_struct2 {
	uint8_t uint8;
};

struct m_variable_descr vars[] = {
	{"uint32", MST_MEMORY, MRT_UINT32, offsetof(struct test_struct, uint32), 0},
	{"array32", MST_MEMORY, MRT_STRING, offsetof(struct test_struct, array32), offsetof(struct test_struct, array32_len)},
	{"pointer", MST_ADDRESS, MRT_STRING, offsetof(struct test_struct, pointer), offsetof(struct test_struct, pointer_len)},
	{"", 0, 0, 0, 0},
};

struct m_variable_descr vars2[] = {
	{"uint8", MST_MEMORY, MRT_UINT8, offsetof(struct test_struct2, uint8), 0},
};

int main()
{

	struct m_variable_table_manager mgr ;
	m_variable_table_manager_init(&mgr);

	size_t base = m_variable_descr_register(&mgr, vars, sizeof(struct test_struct));
	size_t base2 = m_variable_descr_register(&mgr, vars2, sizeof(struct test_struct2));


	struct m_variable_table *table = m_variable_table_new(&mgr);


	struct test_struct *p = table->base + base;
	p->uint32 = 32;
	strcpy(p->array32, "abc"); 
	p->pointer = p->array32;

	struct test_struct2 *p2 = table->base + base2;
	p2->uint8 = 1245;

	m_variable_table_print(&mgr, table);

	m_variable_descr_unregister(&mgr, vars);
	m_variable_table_manager_clean(&mgr);
	return 0;
}

