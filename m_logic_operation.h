#ifndef _M_LOGIC_OPERATION_H_
#define _M_LOGIC_OPERATION_H_

#include "m_variable_table.h"

struct m_logic_operation;

struct m_logic_operation *m_logic_operation_init(const char *exp, struct m_variable_table_manager *manager);
int m_logic_operation_value(struct m_logic_operation *mlo, struct m_variable_table *table);
void m_logic_operation_clean(struct m_logic_operation *mlo);

#endif
