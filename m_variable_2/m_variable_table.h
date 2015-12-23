#ifndef _M_VARIABLE_TABLE_H_
#define _M_VARIABLE_TABLE_H_

#include <stddef.h>

enum m_var_store_type {
	MST_MEMORY = 0,
	MST_ADDRESS,
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

struct m_variable_descr {
	char var_name[16];
	enum m_var_store_type store_type;
	enum m_var_real_type real_type;
	size_t var_offset;
	size_t var_len_offset;
};

#define M_VARIABLE_DESCR_LIST_NULL    {NULL, NULL}

struct m_variable_descr_list {
	struct m_variable_descr_list *prev;
	struct m_variable_descr_list *next;
};


struct m_variable_table_manager {
	size_t size;
	struct m_variable_descr_list head;
};

struct m_variable_table {
	struct m_variable_table_manager *manager;
	void *base;
};

int m_variable_table_manager_init(struct m_variable_table_manager *manager);
void m_variable_table_manager_clean(struct m_variable_table_manager *manager);

size_t m_variable_descr_register(struct m_variable_table_manager *manager, struct m_variable_descr vars[], size_t size);
void m_variable_descr_unregister(struct m_variable_table_manager *manager, struct m_variable_descr vars[]);

struct m_variable_table * m_variable_table_new(struct m_variable_table_manager *manager);
void m_variable_table_free(struct m_variable_table *table);

void m_variable_table_print(struct m_variable_table *table);
const char *m_variable_real_type_2_str(enum m_var_real_type type);
struct m_variable_descr *m_variable_descr_find_by_name(struct m_variable_table_manager *manager, const char *name);

#endif
