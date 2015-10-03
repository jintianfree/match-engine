#include <stdio.h>
#include <assert.h>
#include "m_stack.h"

int main(int argc, char *argv[])
{
	struct m_stack stack;
	int i = 0;
	int j = 0;

	m_stack_init(&stack);

	for(i = 1; m_stack_push(&stack, i) == 0; i++) {
	}
	printf("stack full %d \n", i);

	i -= 1;

	while((j = m_stack_pop(&stack)) != 0) {
		assert(j == i);
		i--;
		printf("j %d \n", j);
	}

	m_stack_clean(&stack);

	return 0;
}

