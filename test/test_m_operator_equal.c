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

    
    return;
}

int main()
{
    int8_equal_test();
    uint32_equal_test();
    string_equal_test();
    
	return 0;
}
