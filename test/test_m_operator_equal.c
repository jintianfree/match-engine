#include <stdio.h>
#include <assert.h>
#include "m_variable_table.h"
#include "../m_operator_equal.c"

struct test_struct {
	int8_t int8;
	uint32_t uint32;
	char string[1024];
	size_t string_len;
	char *sptr;
	size_t sptr_len;
	char bytes[1024];
	size_t bytes_len;
	char *bptr;
	size_t bptr_len;
};

void int8_equal_test(struct m_variable_table_manager *mgr, size_t base, struct m_variable_table *table)
{
	struct m_variable_descr *descr = NULL;
	struct m_operation op1, op2, op3, op4, op5;

	VAR_TABLE_2_STRUCT(struct test_struct, p, table, base);
	descr = m_variable_descr_find_by_name(mgr, "int8");

	assert(m_operator_equal_init(descr, NULL, "65535", &op1) != 0);	/* equal(int8:65535) */
	assert(m_operator_equal_init(descr, NULL, "255",   &op1) != 0);	/* equal(int8:255) */
	assert(m_operator_equal_init(descr, NULL, "a25",   &op1) != 0);	/* equal(int8:255) */
	assert(m_operator_equal_init(descr, NULL, "25a",   &op1) != 0);	/* equal(int8:255) */

	assert(m_operator_equal_init(descr, NULL, "-128",  &op1) == 0);	/* equal(int8:-128) */
	assert(m_operator_equal_init(descr, NULL, " 127",  &op2) == 0);	/* equal(int8:127) */
	assert(m_operator_equal_init(descr, NULL, "0",     &op3) == 0);	/* equal(int8:0) */
	assert(m_operator_equal_init(descr, NULL, "-55",   &op4) == 0);	/* equal(int8:-55) */
	assert(m_operator_equal_init(descr, NULL, "55 ",   &op5) == 0);	/* equal(int8:55) */

	p->int8 = -128;
	assert(m_operator_equal_value(&op1, table) == 1);
	assert(m_operator_equal_value(&op2, table) == 0);
	assert(m_operator_equal_value(&op3, table) == 0);
	assert(m_operator_equal_value(&op4, table) == 0);
	assert(m_operator_equal_value(&op5, table) == 0);

	p->int8 = 127;
	assert(m_operator_equal_value(&op1, table) == 0);
	assert(m_operator_equal_value(&op2, table) == 1);
	assert(m_operator_equal_value(&op3, table) == 0);
	assert(m_operator_equal_value(&op4, table) == 0);
	assert(m_operator_equal_value(&op5, table) == 0);

	p->int8 = 0;
	assert(m_operator_equal_value(&op1, table) == 0);
	assert(m_operator_equal_value(&op2, table) == 0);
	assert(m_operator_equal_value(&op3, table) == 1);
	assert(m_operator_equal_value(&op4, table) == 0);
	assert(m_operator_equal_value(&op5, table) == 0);

	p->int8 = -55;
	assert(m_operator_equal_value(&op1, table) == 0);
	assert(m_operator_equal_value(&op2, table) == 0);
	assert(m_operator_equal_value(&op3, table) == 0);
	assert(m_operator_equal_value(&op4, table) == 1);
	assert(m_operator_equal_value(&op5, table) == 0);

	p->int8= 55;
	assert(m_operator_equal_value(&op1, table) == 0);
	assert(m_operator_equal_value(&op2, table) == 0);
	assert(m_operator_equal_value(&op3, table) == 0);
	assert(m_operator_equal_value(&op4, table) == 0);
	assert(m_operator_equal_value(&op5, table) == 1);

	m_operator_equal_clean(&op1);
	m_operator_equal_clean(&op2);
	m_operator_equal_clean(&op3);
	m_operator_equal_clean(&op4);
	m_operator_equal_clean(&op5);

	return;
}

void uint32_equal_test(struct m_variable_table_manager *mgr, size_t base, struct m_variable_table *table)
{
	struct m_variable_descr *descr = NULL;
	struct m_operation op1, op2, op3;

	VAR_TABLE_2_STRUCT(struct test_struct, p, table, base);
	descr = m_variable_descr_find_by_name(mgr, "uint32");

	assert(m_operator_equal_init(descr, NULL, "4294967296",           &op1) != 0);
	assert(m_operator_equal_init(descr, NULL, "18446744073709551615", &op1) != 0);
	assert(m_operator_equal_init(descr, NULL, "18446744073709551616", &op1) != 0);

	assert(m_operator_equal_init(descr, NULL, "4294967295", &op1) == 0);
	assert(m_operator_equal_init(descr, NULL, "0",          &op2) == 0);	
	assert(m_operator_equal_init(descr, NULL, "55",         &op3) == 0);

	p->uint32 = 4294967295U;
	assert(m_operator_equal_value(&op1, table) == 1);
	assert(m_operator_equal_value(&op2, table) == 0);
	assert(m_operator_equal_value(&op3, table) == 0);

	p->uint32 = 0;
	assert(m_operator_equal_value(&op1, table) == 0);
	assert(m_operator_equal_value(&op2, table) == 1);
	assert(m_operator_equal_value(&op3, table) == 0);    

	p->uint32 = 55;
	assert(m_operator_equal_value(&op1, table) == 0);
	assert(m_operator_equal_value(&op2, table) == 0);
	assert(m_operator_equal_value(&op3, table) == 1);


	m_operator_equal_clean(&op1);
	m_operator_equal_clean(&op2);
	m_operator_equal_clean(&op3);

	return;
}

void string_equal_test(struct m_variable_table_manager *mgr, size_t base, struct m_variable_table *table)
{
	struct m_variable_descr *descr = NULL;

	VAR_TABLE_2_STRUCT(struct test_struct, p, table, base);
	descr = m_variable_descr_find_by_name(mgr, "string");

	/*
	 * MST_MEMORY
	 */
	struct m_operation op1, op2;

	assert(m_operator_equal_init(descr, NULL, "",           &op1) == 0);
	assert(m_operator_equal_init(descr, NULL, "abc \t \n",  &op2) == 0);

	char *s = "";
	strcpy(p->string, s);
	p->string_len = strlen(s);    
	assert(m_operator_equal_value(&op1, table) == 1);
	assert(m_operator_equal_value(&op2, table) == 0);

	s = "abc \t \n";
	strcpy(p->string, s);
	p->string_len = strlen(s);
	assert(m_operator_equal_value(&op1, table) == 0);
	assert(m_operator_equal_value(&op2, table) == 1);

	p->string_len = 3;
	assert(m_operator_equal_value(&op1, table) == 0);
	assert(m_operator_equal_value(&op2, table) == 0);    

	m_operator_equal_clean(&op1);
	m_operator_equal_clean(&op2);

	/*
	 * MST_ADDRESS
	 */
	descr = m_variable_descr_find_by_name(mgr, "sptr");

	assert(m_operator_equal_init(descr, NULL, "",           &op1) == 0);
	assert(m_operator_equal_init(descr, NULL, "abc \t \n",  &op2) == 0);

	p->sptr = NULL;
	p->sptr_len = 0;
	assert(m_operator_equal_value(&op1, table) == 0);
	assert(m_operator_equal_value(&op2, table) == 0);


	s = "";
	p->sptr = s;
	p->sptr_len = strlen(s);
	assert(m_operator_equal_value(&op1, table) == 1);
	assert(m_operator_equal_value(&op2, table) == 0);

	s = "abc \t \n";
	p->sptr = s;
	p->sptr_len = strlen(s);
	assert(m_operator_equal_value(&op1, table) == 0);
	assert(m_operator_equal_value(&op2, table) == 1);

	p->sptr_len = 3;
	assert(m_operator_equal_value(&op1, table) == 0);
	assert(m_operator_equal_value(&op2, table) == 0);    

	s = "abc \t \n++++++++";
	p->sptr = s;
	p->sptr_len = strlen("abc \t \n");
	assert(m_operator_equal_value(&op1, table) == 0);
	assert(m_operator_equal_value(&op2, table) == 1); 

	m_operator_equal_clean(&op1);
	m_operator_equal_clean(&op2);

	/*
	 * option
	 */

	descr = m_variable_descr_find_by_name(mgr, "string");

	struct m_operation op0, op3, op4, op5, op6, op7, op8;

	assert(m_operator_equal_init(descr, "[[]",     "abcde", &op1) != 0);
	assert(m_operator_equal_init(descr, "]",       "abcde", &op1) != 0);
	assert(m_operator_equal_init(descr, "[",       "abcde", &op1) != 0);
	assert(m_operator_equal_init(descr, ",[]",     "abcde", &op1) != 0);
	assert(m_operator_equal_init(descr, "[2,,3]",  "abcde", &op1) != 0);
	assert(m_operator_equal_init(descr, "[2,,]",   "abcde", &op1) != 0);
	assert(m_operator_equal_init(descr, "[,,]",    "abcde", &op1) != 0);
	assert(m_operator_equal_init(descr, "[,,3]",   "abcde", &op1) != 0);

	assert(m_operator_equal_init(descr, ""    ,    "abcde", &op0) == 0);
	assert(m_operator_equal_init(descr, "[][]",    "abcde", &op1) == 0);
	assert(m_operator_equal_init(descr, "[,][]",   "abcde", &op2) == 0);
	assert(m_operator_equal_init(descr, "[3][]",   "abcde", &op3) == 0);
	assert(m_operator_equal_init(descr, "[,5][]",  "abcde", &op4) == 0);
	assert(m_operator_equal_init(descr, "[3,][]",  "abcde", &op5) == 0);
	assert(m_operator_equal_init(descr, "[1,6][]", "abcde", &op6) == 0);
	assert(m_operator_equal_init(descr, "[][I]",   "abcde", &op7) == 0);
	assert(m_operator_equal_init(descr, "[1,6][I]","abcde", &op8) == 0);

	s = "abcde";
	strcpy(p->string, s);
	p->string_len = strlen(s);    
	assert(m_operator_equal_value(&op0, table) == 1);
	assert(m_operator_equal_value(&op1, table) == 1);
	assert(m_operator_equal_value(&op2, table) == 1);
	assert(m_operator_equal_value(&op3, table) == 0);
	assert(m_operator_equal_value(&op4, table) == 1);
	assert(m_operator_equal_value(&op5, table) == 0);
	assert(m_operator_equal_value(&op6, table) == 0);
	assert(m_operator_equal_value(&op7, table) == 1);
	assert(m_operator_equal_value(&op8, table) == 0);

	s = "012abcde";
	strcpy(p->string, s);
	p->string_len = strlen(s);    
	assert(m_operator_equal_value(&op0, table) == 0);
	assert(m_operator_equal_value(&op1, table) == 0);
	assert(m_operator_equal_value(&op2, table) == 0);
	assert(m_operator_equal_value(&op3, table) == 1);
	assert(m_operator_equal_value(&op4, table) == 0);
	assert(m_operator_equal_value(&op5, table) == 1);
	assert(m_operator_equal_value(&op6, table) == 0);
	assert(m_operator_equal_value(&op7, table) == 0);
	assert(m_operator_equal_value(&op8, table) == 0);

	s = "0abcde678";
	strcpy(p->string, s);
	p->string_len = strlen(s);    
	assert(m_operator_equal_value(&op0, table) == 0);
	assert(m_operator_equal_value(&op1, table) == 0);
	assert(m_operator_equal_value(&op2, table) == 0);
	assert(m_operator_equal_value(&op3, table) == 0);
	assert(m_operator_equal_value(&op4, table) == 0);
	assert(m_operator_equal_value(&op5, table) == 0);
	assert(m_operator_equal_value(&op6, table) == 1);
	assert(m_operator_equal_value(&op7, table) == 0);
	assert(m_operator_equal_value(&op8, table) == 1);

	s = "0abCDe678";
	strcpy(p->string, s);
	p->string_len = strlen(s);    
	assert(m_operator_equal_value(&op0, table) == 0);
	assert(m_operator_equal_value(&op1, table) == 0);
	assert(m_operator_equal_value(&op2, table) == 0);
	assert(m_operator_equal_value(&op3, table) == 0);
	assert(m_operator_equal_value(&op4, table) == 0);
	assert(m_operator_equal_value(&op5, table) == 0);
	assert(m_operator_equal_value(&op6, table) == 0);
	assert(m_operator_equal_value(&op7, table) == 0);
	assert(m_operator_equal_value(&op8, table) == 1);

	s = "abCDe";
	strcpy(p->string, s);
	p->string_len = strlen(s);    
	assert(m_operator_equal_value(&op0, table) == 0);
	assert(m_operator_equal_value(&op1, table) == 0);
	assert(m_operator_equal_value(&op2, table) == 0);
	assert(m_operator_equal_value(&op3, table) == 0);
	assert(m_operator_equal_value(&op4, table) == 0);
	assert(m_operator_equal_value(&op5, table) == 0);
	assert(m_operator_equal_value(&op6, table) == 0);
	assert(m_operator_equal_value(&op7, table) == 1);
	assert(m_operator_equal_value(&op8, table) == 0);

	m_operator_equal_clean(&op0);
	m_operator_equal_clean(&op1);
	m_operator_equal_clean(&op2);
	m_operator_equal_clean(&op3);
	m_operator_equal_clean(&op4);
	m_operator_equal_clean(&op5);
	m_operator_equal_clean(&op6);
	m_operator_equal_clean(&op7);
	m_operator_equal_clean(&op8);

	/*
	 * option  B
	 */

	descr = m_variable_descr_find_by_name(mgr, "string");

	/* e8 a7 84 e5 88 99  -- guize in chinese - utf-8
	 * b9 e6 d4 f2 -- guize in chinese - ansi 
	 * 61 0a -- a utf-8
	 * 41 0a -- A utf-8
	 * 42 0a -- B utf-8
	 */
	char a1[] = {0xe8, 0xa7, 0x84, 0xe5, 0x88, 0x99, 0x00};	/* guize */
	char a2[] = {0x61, 0x0a, 0xe8, 0xa7, 0x84, 0xe5, 0x88, 0x99, 0x00};	/* a + guize */
	char b1[] = {0xb9, 0xe6, 0xd4, 0xf2, 0x00};

	assert(m_operator_equal_init(descr, "[][IB]", "abcde", &op2) != 0);

	assert(m_operator_equal_init(descr, "[][B]",  "e8 a7 84 e5 88 99 ", &op1) == 0);
	assert(m_operator_equal_init(descr, "[][B]",  "61 0a e8 a7 84 e5 88 99 ", &op2) == 0);
	assert(m_operator_equal_init(descr, "[][B]",  "b9 e6 d4 f2 ", &op3) == 0);
	assert(m_operator_equal_init(descr, "[2,8][B]",  "e8 a7 84 e5 88 99 ", &op4) == 0);

	strcpy(p->string, a1);
	p->string_len = strlen(a1);
	assert(m_operator_equal_value(&op1, table) == 1);
	assert(m_operator_equal_value(&op2, table) == 0);
	assert(m_operator_equal_value(&op3, table) == 0);
	assert(m_operator_equal_value(&op4, table) == 0);

	strcpy(p->string, a2);
	p->string_len = strlen(a2);
	assert(m_operator_equal_value(&op1, table) == 0);
	assert(m_operator_equal_value(&op2, table) == 1);
	assert(m_operator_equal_value(&op3, table) == 0);
	assert(m_operator_equal_value(&op4, table) == 1);

	strcpy(p->string, b1);
	p->string_len = strlen(b1);
	assert(m_operator_equal_value(&op1, table) == 0);
	assert(m_operator_equal_value(&op2, table) == 0);
	assert(m_operator_equal_value(&op3, table) == 1);
	assert(m_operator_equal_value(&op4, table) == 0);

	m_operator_equal_clean(&op1);
	m_operator_equal_clean(&op2);
	m_operator_equal_clean(&op3);
	m_operator_equal_clean(&op4);
	
	return;
}

void bytes_equal_test(struct m_variable_table_manager *mgr, size_t base, struct m_variable_table *table)
{
	struct m_variable_descr *descr = NULL;

	VAR_TABLE_2_STRUCT(struct test_struct, p, table, base);
	descr = m_variable_descr_find_by_name(mgr, "bytes");

	struct m_operation op1, op2, op3;

	assert(m_operator_equal_init(descr, "[]", "abcde", &op1) != 0);
	assert(m_operator_equal_init(descr, "[]", "abcd", &op1) != 0);
	assert(m_operator_equal_init(descr, "[]", "abc", &op1) != 0);
	assert(m_operator_equal_init(descr, "[]", "ab gi", &op1) != 0);
	assert(m_operator_equal_init(descr, "[]", "ab ig", &op1) != 0);
	assert(m_operator_equal_init(descr, "[]", "ab ig ab", &op1) != 0);
	assert(m_operator_equal_init(descr, "[]", "ab,", &op1) != 0);
	assert(m_operator_equal_init(descr, "[]", "ab,cd", &op1) != 0);
	assert(m_operator_equal_init(descr, "[][IB]", "ab", &op1) != 0);

	assert(m_operator_equal_init(descr, "", "ab cd ef", &op1) == 0);
	assert(m_operator_equal_init(descr, "", "0xab 0xcd 0xef", &op2) == 0);
	assert(m_operator_equal_init(descr, "[5,8]", "0xab 0xcd 0xef", &op3) == 0);

	p->bytes_len = 0;
	assert(m_operator_equal_value(&op1, table) == 0);
	assert(m_operator_equal_value(&op2, table) == 0);
	assert(m_operator_equal_value(&op3, table) == 0);

	p->bytes[0] = 0xab;
	p->bytes[1] = 0xcd;
	p->bytes[2] = 0xef;
	assert(m_operator_equal_value(&op1, table) == 0);
	assert(m_operator_equal_value(&op2, table) == 0);
	assert(m_operator_equal_value(&op3, table) == 0);

	p->bytes_len = 3;
	assert(m_operator_equal_value(&op1, table) == 1);
	assert(m_operator_equal_value(&op2, table) == 1);
	assert(m_operator_equal_value(&op3, table) == 0);

	p->bytes_len = 2;
	assert(m_operator_equal_value(&op1, table) == 0);
	assert(m_operator_equal_value(&op2, table) == 0);
	assert(m_operator_equal_value(&op3, table) == 0);

	p->bytes_len = 4;
	assert(m_operator_equal_value(&op1, table) == 0);
	assert(m_operator_equal_value(&op2, table) == 0);
	assert(m_operator_equal_value(&op3, table) == 0);

	p->bytes[5] = 0xab;
	p->bytes[6] = 0xcd;
	p->bytes[7] = 0xef;

	p->bytes_len = 7;
	assert(m_operator_equal_value(&op1, table) == 0);
	assert(m_operator_equal_value(&op2, table) == 0);
	assert(m_operator_equal_value(&op3, table) == 0);

	p->bytes_len = 8;
	assert(m_operator_equal_value(&op1, table) == 0);
	assert(m_operator_equal_value(&op2, table) == 0);
	assert(m_operator_equal_value(&op3, table) == 1);

	p->bytes_len = 9;
	assert(m_operator_equal_value(&op1, table) == 0);
	assert(m_operator_equal_value(&op2, table) == 0);
	assert(m_operator_equal_value(&op3, table) == 1);

	return;
}

int main()
{
	size_t base = 0;
	struct m_variable_table_manager *mgr = NULL;
	struct m_variable_table *table = NULL;
	struct m_variable_descr descrs[] = {
		{"int8", MST_MEMORY, MRT_INT8, offsetof(struct test_struct, int8), 0},
		{"uint32", MST_MEMORY, MRT_UINT32, offsetof(struct test_struct, uint32), 0},
		{"string", MST_MEMORY, MRT_STRING, offsetof(struct test_struct, string), offsetof(struct test_struct, string_len)},
		{"sptr", MST_ADDRESS, MRT_STRING, offsetof(struct test_struct, sptr), offsetof(struct test_struct, sptr_len)},
		{"bytes", MST_MEMORY, MRT_BYTES, offsetof(struct test_struct, bytes), offsetof(struct test_struct, bytes_len)},
		{"bptr", MST_ADDRESS, MRT_STRING, offsetof(struct test_struct, bptr), offsetof(struct test_struct, bptr_len)},
		{"", 0, 0, 0, 0},
	};

	mgr = m_variable_table_manager_init();
	base = m_variable_descr_register(mgr, descrs, sizeof(struct test_struct));
	table = m_variable_table_new(mgr);

	int8_equal_test(mgr, base, table);
	uint32_equal_test(mgr, base, table);
	string_equal_test(mgr, base, table);
	bytes_equal_test(mgr, base, table);

	m_variable_descr_unregister(mgr, descrs);
	m_variable_table_manager_clean(mgr);

	return 0;
}

