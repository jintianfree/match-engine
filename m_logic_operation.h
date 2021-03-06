#ifndef _M_LOGIC_OPERATION_H_
#define _M_LOGIC_OPERATION_H_

#include "m_variable.h"
#include "m_logic_expression.h"

struct m_logic_operation {
	struct m_logic_expression exp;
};

int m_logic_operation_init(struct m_logic_operation *mlo, const char *exp, struct m_variable_list *head);
int m_logic_operation_value(struct m_logic_operation *mlo);
void m_logic_operation_clean(struct m_logic_operation *mlo);

#endif
