#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "m_log.h"
#include "m_stack.h"

#define DEFAULT_STACK_SIZE		16
#define MAX_STACK_SIZE			10240

union m_stack_element {
	intptr_t e;
};

int m_stack_init(struct m_stack *s)
{
	assert(s != NULL);

	s->elements = malloc(DEFAULT_STACK_SIZE * sizeof(union m_stack_element));
	if(s->elements == NULL) {
		goto err;
	}
	s->capacity = DEFAULT_STACK_SIZE;
	s->top = 0;

	return 0;
err:
	ERROR("maloc failed! \n");
	return -1;
}

void m_stack_clean(struct m_stack *s)
{
	assert(s != NULL);

	s->top = 0;
	s->capacity = 0;
	if(s->elements) {
		free(s->elements);
		s->elements = NULL;
	}

	return;
}

int m_stack_push(struct m_stack *s, intptr_t e)
{
	int eno = 0;

	assert(s && s->capacity);
	assert(e != 0);
	
	if(s->top + 1 > s->capacity) {
		int new_capacity = 2 * s->capacity;
		union m_stack_element *p = NULL;

		if(new_capacity > MAX_STACK_SIZE) {
			if(s->capacity == MAX_STACK_SIZE) {
				eno = -1;
				goto err;
			} else {
				new_capacity = MAX_STACK_SIZE;
			}
		}

		p = realloc(s->elements, new_capacity * sizeof(union m_stack_element));
		if(p == NULL) {
			eno = -2;
			goto err;
		}

		s->elements = p;
		s->capacity = new_capacity;
	}

	s->elements[s->top].e = e;
	s->top++;

	return 0;
err:
	switch(eno) {
	case -1:
		ERROR("stack full! \n");
		break;
	case -2:
		ERROR("malloc failed! \n");
		break;
	}

	return -1;
}

intptr_t m_stack_top(struct m_stack *s)
{
	if(s->top == 0) {
		return 0;
	}

	return s->elements[s->top - 1].e;
}

intptr_t m_stack_pop(struct m_stack *s)
{
	if(s->top == 0) {
		return 0;
	}

	s->top--;

	return s->elements[s->top].e;
}

int m_stack_empty(struct m_stack *s)
{
	return (s->top == 0);
}


