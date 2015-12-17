#include <stdio.h>
#include <assert.h>
#include "m_variable.h"
#include "../m_operator_equal.c"
void int8_equal_test()
{
	int8_t int8 = 0;
    struct m_operation op1, op2, op3, op4, op5;
    struct m_variable var = {"int8", MST_ADDRESS, MRT_INT8, &int8, NULL, M_VARIABLE_LIST_NULL};
    
	assert(m_operator_equal_init(&var, NULL, "65535", &op1) != 0);	/* equal(int8:65535) */
	assert(m_operator_equal_init(&var, NULL, "255",   &op1) != 0);	/* equal(int8:255) */

	assert(m_operator_equal_init(&var, NULL, "-128", &op1) == 0);	/* equal(int8:-128) */
	assert(m_operator_equal_init(&var, NULL, "127",  &op2) == 0);	/* equal(int8:127) */
	assert(m_operator_equal_init(&var, NULL, "0",    &op3) == 0);	/* equal(int8:0) */
	assert(m_operator_equal_init(&var, NULL, "-55",  &op4) == 0);	/* equal(int8:-55) */
	assert(m_operator_equal_init(&var, NULL, "55",   &op5) == 0);	/* equal(int8:55) */

	int8 = -128;
	assert(m_operator_equal_value(&op1) == 1);
	assert(m_operator_equal_value(&op2) == 0);
	assert(m_operator_equal_value(&op3) == 0);
	assert(m_operator_equal_value(&op4) == 0);
	assert(m_operator_equal_value(&op5) == 0);

	int8 = 127;
	assert(m_operator_equal_value(&op1) == 0);
	assert(m_operator_equal_value(&op2) == 1);
	assert(m_operator_equal_value(&op3) == 0);
	assert(m_operator_equal_value(&op4) == 0);
	assert(m_operator_equal_value(&op5) == 0);

	int8 = 0;
	assert(m_operator_equal_value(&op1) == 0);
	assert(m_operator_equal_value(&op2) == 0);
	assert(m_operator_equal_value(&op3) == 1);
	assert(m_operator_equal_value(&op4) == 0);
	assert(m_operator_equal_value(&op5) == 0);

	int8 = -55;
	assert(m_operator_equal_value(&op1) == 0);
	assert(m_operator_equal_value(&op2) == 0);
	assert(m_operator_equal_value(&op3) == 0);
	assert(m_operator_equal_value(&op4) == 1);
	assert(m_operator_equal_value(&op5) == 0);

	int8= 55;
	assert(m_operator_equal_value(&op1) == 0);
	assert(m_operator_equal_value(&op2) == 0);
	assert(m_operator_equal_value(&op3) == 0);
	assert(m_operator_equal_value(&op4) == 0);
	assert(m_operator_equal_value(&op5) == 1);

	m_operator_equal_clean(&op1);
	m_operator_equal_clean(&op2);
	m_operator_equal_clean(&op3);
	m_operator_equal_clean(&op4);
	m_operator_equal_clean(&op5);
    
    return;
}

void uint32_equal_test()
{
	uint32_t uint32 = 0;	
    struct m_operation op1, op2, op3;
    struct m_variable var = {"uint32", MST_ADDRESS, MRT_UINT32, &uint32, NULL, M_VARIABLE_LIST_NULL};
    
	assert(m_operator_equal_init(&var, NULL, "4294967296",           &op1) != 0);
    assert(m_operator_equal_init(&var, NULL, "18446744073709551615", &op1) != 0);
    assert(m_operator_equal_init(&var, NULL, "18446744073709551616", &op1) != 0);

	assert(m_operator_equal_init(&var, NULL, "4294967295", &op1) == 0);
	assert(m_operator_equal_init(&var, NULL, "0",          &op2) == 0);	
	assert(m_operator_equal_init(&var, NULL, "55",         &op3) == 0);

	uint32 = 4294967295U;
 	assert(m_operator_equal_value(&op1) == 1);
	assert(m_operator_equal_value(&op2) == 0);
	assert(m_operator_equal_value(&op3) == 0);
    
    uint32 = 0;
	assert(m_operator_equal_value(&op1) == 0);
	assert(m_operator_equal_value(&op2) == 1);
	assert(m_operator_equal_value(&op3) == 0);    
    
    uint32 = 55;
	assert(m_operator_equal_value(&op1) == 0);
	assert(m_operator_equal_value(&op2) == 0);
	assert(m_operator_equal_value(&op3) == 1);
	

	m_operator_equal_clean(&op1);
	m_operator_equal_clean(&op2);
	m_operator_equal_clean(&op3);
    
    return;
}

void string_equal_test()
{
	char string[1024];
	char *sptr = NULL;
	char *p = NULL;
	size_t slen = 0;        
	struct m_operation op1, op2;

	/*
	 * MST_ADDRESS
	 */
	struct m_variable var1 = {"string1", MST_ADDRESS, MRT_STRING, string, &slen, M_VARIABLE_LIST_NULL};

	assert(m_operator_equal_init(&var1, NULL, "",           &op1) == 0);
	assert(m_operator_equal_init(&var1, NULL, "abc \t \n",  &op2) == 0);

	p = "";
	strcpy(string, p);
	slen = strlen(p);    
	assert(m_operator_equal_value(&op1) == 1);
	assert(m_operator_equal_value(&op2) == 0);

	p = "abc \t \n";
	strcpy(string, p);
	slen = strlen(p);
	assert(m_operator_equal_value(&op1) == 0);
	assert(m_operator_equal_value(&op2) == 1);

	slen = 3;
	assert(m_operator_equal_value(&op1) == 0);
	assert(m_operator_equal_value(&op2) == 0);    

	m_operator_equal_clean(&op1);
	m_operator_equal_clean(&op2);

	/*
	 * ERROR:  MST_POINTER_ADDRESS + &string  is a wrong way.
	 */
	struct m_variable var2 = {"string2", MST_POINTER_ADDRESS, MRT_STRING, &string, &slen, M_VARIABLE_LIST_NULL};
	(void)var2;

	/*
	 * MST_POINTER_ADDRESS
	 */
	struct m_variable var3 = {"sptr",    MST_POINTER_ADDRESS, MRT_STRING, &sptr,   &slen, M_VARIABLE_LIST_NULL};

	assert(m_operator_equal_init(&var3, NULL, "",           &op1) == 0);
	assert(m_operator_equal_init(&var3, NULL, "abc \t \n",  &op2) == 0);

	sptr = NULL;
	slen = 0;
	assert(m_operator_equal_value(&op1) == 0);
	assert(m_operator_equal_value(&op2) == 0);

	sptr = string;

	p = "";
	strcpy(string, p);
	slen = strlen(p);    
	assert(m_operator_equal_value(&op1) == 1);
	assert(m_operator_equal_value(&op2) == 0);

	p = "abc \t \n";
	strcpy(string, p);
	slen = strlen(p);
	assert(m_operator_equal_value(&op1) == 0);
	assert(m_operator_equal_value(&op2) == 1);

	slen = 3;
	assert(m_operator_equal_value(&op1) == 0);
	assert(m_operator_equal_value(&op2) == 0);    

	p = "abc \t \n++++++++";
	strcpy(string, p);
	slen = strlen("abc \t \n");
	assert(m_operator_equal_value(&op1) == 0);
	assert(m_operator_equal_value(&op2) == 1); 

	m_operator_equal_clean(&op1);
	m_operator_equal_clean(&op2);

	
	/*
	 * option
	 */
	struct m_operation op0, op3, op4, op5, op6, op7, op8;
	struct m_variable var4 = {"string1", MST_ADDRESS, MRT_STRING, string, &slen, M_VARIABLE_LIST_NULL};

	assert(m_operator_equal_init(&var4, "[[]",     "abcde", &op1) != 0);
	assert(m_operator_equal_init(&var4, "]",       "abcde", &op1) != 0);
	assert(m_operator_equal_init(&var4, "[",       "abcde", &op1) != 0);
	assert(m_operator_equal_init(&var4, ",[]",     "abcde", &op1) != 0);
	assert(m_operator_equal_init(&var4, "[2,,3]",  "abcde", &op1) != 0);
	assert(m_operator_equal_init(&var4, "[2,,]",   "abcde", &op1) != 0);
	assert(m_operator_equal_init(&var4, "[,,]",    "abcde", &op1) != 0);
	assert(m_operator_equal_init(&var4, "[,,3]",   "abcde", &op1) != 0);

	assert(m_operator_equal_init(&var4, ""    ,    "abcde", &op0) == 0);
	assert(m_operator_equal_init(&var4, "[][]",    "abcde", &op1) == 0);
	assert(m_operator_equal_init(&var4, "[,][]",   "abcde", &op2) == 0);
	assert(m_operator_equal_init(&var4, "[3][]",   "abcde", &op3) == 0);
	assert(m_operator_equal_init(&var4, "[,5][]",  "abcde", &op4) == 0);
	assert(m_operator_equal_init(&var4, "[3,][]",  "abcde", &op5) == 0);
	assert(m_operator_equal_init(&var4, "[1,6][]", "abcde", &op6) == 0);
	assert(m_operator_equal_init(&var4, "[][I]",   "abcde", &op7) == 0);
	assert(m_operator_equal_init(&var4, "[1,6][I]","abcde", &op8) == 0);

	p = "abcde";
	strcpy(string, p);
	slen = strlen(p);    
	assert(m_operator_equal_value(&op0) == 1);
	assert(m_operator_equal_value(&op1) == 1);
	assert(m_operator_equal_value(&op2) == 1);
	assert(m_operator_equal_value(&op3) == 0);
	assert(m_operator_equal_value(&op4) == 1);
	assert(m_operator_equal_value(&op5) == 0);
	assert(m_operator_equal_value(&op6) == 0);
	assert(m_operator_equal_value(&op7) == 1);
	assert(m_operator_equal_value(&op8) == 0);

	p = "012abcde";
	strcpy(string, p);
	slen = strlen(p);    
	assert(m_operator_equal_value(&op0) == 0);
	assert(m_operator_equal_value(&op1) == 0);
	assert(m_operator_equal_value(&op2) == 0);
	assert(m_operator_equal_value(&op3) == 1);
	assert(m_operator_equal_value(&op4) == 0);
	assert(m_operator_equal_value(&op5) == 1);
	assert(m_operator_equal_value(&op6) == 0);
	assert(m_operator_equal_value(&op7) == 0);
	assert(m_operator_equal_value(&op8) == 0);

	p = "0abcde678";
	strcpy(string, p);
	slen = strlen(p);    
	assert(m_operator_equal_value(&op0) == 0);
	assert(m_operator_equal_value(&op1) == 0);
	assert(m_operator_equal_value(&op2) == 0);
	assert(m_operator_equal_value(&op3) == 0);
	assert(m_operator_equal_value(&op4) == 0);
	assert(m_operator_equal_value(&op5) == 0);
	assert(m_operator_equal_value(&op6) == 1);
	assert(m_operator_equal_value(&op7) == 0);
	assert(m_operator_equal_value(&op8) == 1);

	p = "0abCDe678";
	strcpy(string, p);
	slen = strlen(p);    
	assert(m_operator_equal_value(&op0) == 0);
	assert(m_operator_equal_value(&op1) == 0);
	assert(m_operator_equal_value(&op2) == 0);
	assert(m_operator_equal_value(&op3) == 0);
	assert(m_operator_equal_value(&op4) == 0);
	assert(m_operator_equal_value(&op5) == 0);
	assert(m_operator_equal_value(&op6) == 0);
	assert(m_operator_equal_value(&op7) == 0);
	assert(m_operator_equal_value(&op8) == 1);

	p = "abCDe";
	strcpy(string, p);
	slen = strlen(p);    
	assert(m_operator_equal_value(&op0) == 0);
	assert(m_operator_equal_value(&op1) == 0);
	assert(m_operator_equal_value(&op2) == 0);
	assert(m_operator_equal_value(&op3) == 0);
	assert(m_operator_equal_value(&op4) == 0);
	assert(m_operator_equal_value(&op5) == 0);
	assert(m_operator_equal_value(&op6) == 0);
	assert(m_operator_equal_value(&op7) == 1);
	assert(m_operator_equal_value(&op8) == 0);

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

	/* e8 a7 84 e5 88 99  -- guize in chinese - utf-8
	 * b9 e6 d4 f2 -- guize in chinese - ansi 
	 * 61 0a -- a utf-8
	 * 41 0a -- A utf-8
	 * 42 0a -- B utf-8
	 */
	char a1[] = {0xe8, 0xa7, 0x84, 0xe5, 0x88, 0x99, 0x00};	/* guize */
	char a2[] = {0x61, 0x0a, 0xe8, 0xa7, 0x84, 0xe5, 0x88, 0x99, 0x00};	/* a + guize */
	char b1[] = {0xb9, 0xe6, 0xd4, 0xf2, 0x00};

	struct m_variable var5 = {"string1", MST_POINTER_ADDRESS, MRT_STRING, &p, &slen, M_VARIABLE_LIST_NULL};

	assert(m_operator_equal_init(&var5, "[][IB]", "abcde", &op2) != 0);

	assert(m_operator_equal_init(&var5, "[][B]",  "e8 a7 84 e5 88 99 ", &op1) == 0);
	assert(m_operator_equal_init(&var5, "[][B]",  "61 0a e8 a7 84 e5 88 99 ", &op2) == 0);
	assert(m_operator_equal_init(&var5, "[][B]",  "b9 e6 d4 f2 ", &op3) == 0);
	assert(m_operator_equal_init(&var5, "[2,8][B]",  "e8 a7 84 e5 88 99 ", &op4) == 0);

	p = a1;
	slen = strlen(a1);
	assert(m_operator_equal_value(&op1) == 1);
	assert(m_operator_equal_value(&op2) == 0);
	assert(m_operator_equal_value(&op3) == 0);
	assert(m_operator_equal_value(&op4) == 0);

	p = a2;
	slen = strlen(a2);
	assert(m_operator_equal_value(&op1) == 0);
	assert(m_operator_equal_value(&op2) == 1);
	assert(m_operator_equal_value(&op3) == 0);
	assert(m_operator_equal_value(&op4) == 1);

	p = b1;
	slen = strlen(b1);
	assert(m_operator_equal_value(&op1) == 0);
	assert(m_operator_equal_value(&op2) == 0);
	assert(m_operator_equal_value(&op3) == 1);
	assert(m_operator_equal_value(&op4) == 0);
	
	return;
}

int main()
{
    int8_equal_test();
    uint32_equal_test();
    string_equal_test();
    
	return 0;
}
