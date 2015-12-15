#ifndef _M_OPERATION_H_
#define _M_OPERATION_H_

#include <stdint.h>
#include "m_variable.h"

struct m_operation;

typedef int (*operator_init)(struct m_variable *var, const char *option, const char *value, struct m_operation *operation);
typedef void (*operator_clean)(struct m_operation *operation);
typedef int (*operator_value)(struct m_operation *operation);

struct m_operator {
	char name[16];
	operator_init init;
	operator_clean clean;
	operator_value value;
};

struct m_operation {
	struct m_variable *var;
	struct m_operator *op;
	union {
		struct {
			void *value_p;
			size_t value_len;
		};
		uint64_t value_i;
	};
	void *operator_option;
};

struct m_operation *m_operation_init(const char *s, struct m_variable_list *head);
int m_operation_value(struct m_operation *operation);
void m_operation_clean(struct m_operation *operation);

#endif
