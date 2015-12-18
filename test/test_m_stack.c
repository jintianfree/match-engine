#include <stdio.h>
#include <assert.h>
#include "m_stack.h"

int main()
{
	struct m_stack stack;
	int i = 0;
	int j = 0;

	assert(m_stack_init(&stack) == 0);

	for(i = 1; m_stack_push(&stack, i) == 0; i++) {
	}

	assert(i > 1);
	assert(m_stack_empty(&stack) == 0);

	printf("stack full %d \n", i);

	i -= 1;

	while((j = m_stack_pop(&stack)) != 0) {
		assert(j == i);
		i--;
		printf("j %d \n", j);
	}

	assert(i == 0);
	assert(m_stack_empty(&stack) == 1);

	m_stack_clean(&stack);

	return 0;
}

