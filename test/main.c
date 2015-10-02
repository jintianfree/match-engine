#include <stdio.h>
#include "../expression.c"

void printf_node(struct sub_expression_tree_node *node)
{
	printf("=%s= ", node->expression);
}

int main(int argc, char *argv[]) 
{
	const char *s = "(a(x)x(x((()))))";
	int i = 0;

	printf("DEBUG: %d \n", first_op_pos("!laksd\\!jflsadf)"));
	printf("DEBUG: %d \n", first_op_pos("xlaksd\\!jflsadf)"));
	printf("DEBUG: %d \n", first_op_pos("laksd\\!jflsad\\)"));
	printf("DEBUG: %d \n", first_op_pos("laksd\\!jflsadjf)"));

	s = "((x&1 | (x&2)) \\(";
	char *exps[1024];
	int n = split_expression(s, exps, 1024);
	printf("%s \n", s);
	for(i = 0; i < n; i++) {
		printf("%s  \n", exps[i]);
	}

	s="equal[x,\\ x]&equal[s,s] | xxxx (a\\ bc & 1)|(abc &2)";
	n = split_expression(s, exps, 1024);
	printf("%s \n", s);
	for(i = 0; i < n; i++) {
		printf("%s  \n", exps[i]);
	}

	assert(split_expression(s, exps, 2) < 0);


	char *xx[1024];
	s="(a&b|!c)&((A&B)|(!C))";
	n = split_expression(s, exps, 1024);
	int m = infix_to_suffix(exps, xx, n);

	for(i = 0; i < n; i++) {
		printf("%s ", exps[i]);
	}
	printf("\n");
	for(i = 0; i < m; i++) {
		printf("%s ", xx[i]);
	}
	printf("\n");

	//s = "!((1|0) & (1&0)) & 1 & 1 & 0 | 1";
	s = "!1 & 1 & 1";
	n = split_expression(s, exps, 1024);
	m = infix_to_suffix(exps, xx, n);
	for(i = 0; i < n; i++) {
		printf("%s ", exps[i]);
	}
	printf("\n");
	for(i = 0; i < m; i++) {
		printf("%s ", xx[i]);
	}
	printf("\n");

	struct sub_expression_tree_node *node = NULL;
	s = "!((1|0) & (1&0)) & 1 & 1 & 0 | 1";
	node = create_logical_expression_tree(s);
	assert(match(node) == 1);

	s = "1&1|(0&1&!((1&1&1&!1)&!0))&(1&0&!1)";
	node = create_logical_expression_tree(s);
	assert(match(node) == 0);

	printf("%s\n", s);
	m_binary_tree_traversal_LDR(node, printf_node);
	printf("\n");
	printf("%d \n", match(node));
	return 0;
}
