#ifndef _M_VARIABLE_H_
#define _M_VARIABLE_H_

enum m_var_type {
	MT_UINT8 = 0,
	MT_UINT16,
	MT_UINT32,
	MT_UINT64,
	MT_INT8,
	MT_INT16,
	MT_INT32,
	MT_INT64,
	MT_FLOAT,
	MT_DOUBLE,
	MT_ABSOLUTE_TIME,
	MT_RELATIVE_TIME,
	MT_STRING,
	MT_BYTES,
	MT_IPv4,
	MT_IPv6,
	MT_NUM_TYPES /* last item number plus one */
};

struct m_variable {
	enum m_var_type type;
	char var_name[16];
	void *var_addr;
	size_t *var_len;
};

#endif
