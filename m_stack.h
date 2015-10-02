#ifndef _M_STACK_H_
#define _M_STACK_H_

#include <unistd.h>

struct m_stack {
	union m_stack_element *elements;
	int capacity;
	int top;
};

int m_stack_init(struct m_stack *s);
void m_stack_clean(struct m_stack *s);
int m_stack_push(struct m_stack *s, intptr_t e);
intptr_t m_stack_pop(struct m_stack *s);
intptr_t m_stack_top(struct m_stack *s);
int m_stack_empty(struct m_stack *s);

#endif
