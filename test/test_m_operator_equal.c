#include <stdio.h>
#include "m_variable.h"
#include "../m_operator_equal.c"

int main()
{
	int8_t int8 = 0;
	int16_t int16 = 0;
	int32_t int32 = 0;
	int64_t int64 = 0;
	uint8_t uint8 = 0;
	uint16_t uint16 = 0;
	uint32_t uint32 = 0;
	uint64_t uint64 = 0;

    
	struct m_variable vars[] = {
		{"int8",   MST_ADDRESS, MRT_INT8,   &int8,   NULL, M_VARIABLE_LIST_NULL},
		{"int16",  MST_ADDRESS, MRT_INT16,  &int16,  NULL, M_VARIABLE_LIST_NULL},
		{"int32",  MST_ADDRESS, MRT_INT32,  &int32,  NULL, M_VARIABLE_LIST_NULL},
		{"int64",  MST_ADDRESS, MRT_INT64,  &int64,  NULL, M_VARIABLE_LIST_NULL},
		{"uint8",  MST_ADDRESS, MRT_UINT8,  &uint8,  NULL, M_VARIABLE_LIST_NULL},
		{"uint16", MST_ADDRESS, MRT_UINT16, &uint16, NULL, M_VARIABLE_LIST_NULL},
		{"uint32", MST_ADDRESS, MRT_UINT32, &uint32, NULL, M_VARIABLE_LIST_NULL},
		{"uint64", MST_ADDRESS, MRT_UINT64, &uint64, NULL, M_VARIABLE_LIST_NULL},
        	{"", 0, 0, NULL, NULL, M_VARIABLE_LIST_NULL},
	};

	struct m_operation op1, op2, op3, op4;
	m_operator_equal_init(&vars[0], NULL, "-1", &op1);
	m_operator_equal_init(&vars[0], NULL, "65535", &op2);
	m_operator_equal_init(&vars[0], NULL, "255", &op3);
	m_operator_equal_init(&vars[0], NULL, "55", &op4);

	int8 = -1;
	printf("int8 %d -- equal(int8:-1)=%d \n",    int8, m_operator_equal_value(&op1));
	printf("int8 %d -- equal(int8:65535)=%d \n", int8, m_operator_equal_value(&op2));
	printf("int8 %d -- equal(int8:255)=%d \n",   int8, m_operator_equal_value(&op3));
	printf("int8 %d -- equal(int8:55)=%d \n",    int8, m_operator_equal_value(&op4));

	int8 = 65535;
    printf("int8 %d -- equal(int8:-1)=%d \n",    int8, m_operator_equal_value(&op1));
	printf("int8 %d -- equal(int8:65535)=%d \n", int8, m_operator_equal_value(&op2));
	printf("int8 %d -- equal(int8:255)=%d \n",   int8, m_operator_equal_value(&op3));
	printf("int8 %d -- equal(int8:55)=%d \n",    int8, m_operator_equal_value(&op4));
    
	int8 = 255;
	printf("int8 %d -- equal(int8:-1)=%d \n",    int8, m_operator_equal_value(&op1));
	printf("int8 %d -- equal(int8:65535)=%d \n", int8, m_operator_equal_value(&op2));
	printf("int8 %d -- equal(int8:255)=%d \n",   int8, m_operator_equal_value(&op3));
	printf("int8 %d -- equal(int8:55)=%d \n",    int8, m_operator_equal_value(&op4));
    
    int8 = 55;
	printf("int8 %d -- equal(int8:-1)=%d \n",    int8, m_operator_equal_value(&op1));
	printf("int8 %d -- equal(int8:65535)=%d \n", int8, m_operator_equal_value(&op2));
	printf("int8 %d -- equal(int8:255)=%d \n",   int8, m_operator_equal_value(&op3));
	printf("int8 %d -- equal(int8:55)=%d \n",    int8, m_operator_equal_value(&op4));
    
	printf("%d \n", m_operator_equal_value(&op1));

	printf("%d \n", m_operator_equal_value(&op1));

	m_operator_equal_init(&vars[1], NULL, "5", &op1);

	return 0;
}
