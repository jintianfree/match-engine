#ifndef _M_VARIABLE_H_
#define _M_VARIABLE_H_

#include <stddef.h>

enum m_var_store_type {
    MST_ADDRESS,
    MST_POINTER_ADDRESS,
};

enum m_var_real_type {
	MRT_UINT8 = 0,
	MRT_UINT16,
	MRT_UINT32,
	MRT_UINT64,
	MRT_INT8,
	MRT_INT16,
	MRT_INT32,
	MRT_INT64,
	MRT_FLOAT,
	MRT_DOUBLE,
	MRT_ABSOLUTE_TIME,
	MRT_RELATIVE_TIME,
	MRT_STRING,
	MRT_BYTES,
	MRT_IPv4,
	MRT_IPv6,
	MRT_NUM_TYPES /* last item number plus one */
};

#define M_VARIABLE_LIST_NULL    {NULL, NULL}

struct m_variable_list {
	struct m_variable_list *prev;
	struct m_variable_list *next;
};

struct m_variable {
	char var_name[16];
    enum m_var_store_type store_type;
	enum m_var_real_type real_type;
    void *var_addr;
    size_t *var_len;
	struct m_variable_list node;
};

int m_variable_list_register(struct m_variable_list *head, struct m_variable vars[]);
void m_variable_list_unregister(struct m_variable_list *head, struct m_variable vars[]);
void m_variable_list_print(struct m_variable_list *head);

void m_variable_print(struct m_variable *var);
struct m_variable *name_2_var(struct m_variable_list *head, const char *name);

#endif
