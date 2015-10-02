#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <alloca.h>
#include <assert.h>
#include "m_log.h"
#include "m_stack.h"
#include "m_binary_tree.h"
#include "m_logic_expression.h"

#define MAX_EXPRESSION_LEN		2048
#define MIN(a, b)   (a) > (b) ? (b) : (a)

#define LOGICAL_OPERATOR	"!&|()"
#define ESCAPE_CHAR			'\\'
#define SPECIAL_CHAR		" \t"

union sentence_info {
	int value;
	void *data;
	char operator;
};

struct m_logic_sentence {
	struct m_binary_tree_node node;
	char *sentence;
	union sentence_info info;
};


int first_op_pos(const char *expression)
{
	char *op = "!&|()";
	char escape = '\\';

	int i = 0;
	int op_len = strlen(op);
	int ex_len = strlen(expression);

	const char *pos = expression + ex_len;

	while(i < op_len) {
		int n = 0;
		const char *p = NULL;

		while(n < ex_len && (p = strchr(expression + n, op[i])) != NULL) {
			if(p > expression && *(p - 1) == escape) {
				n = p - expression + 1;
				p = NULL;
				continue;
			}

			break;
		}

		if(p && pos > p) {
			pos = p;
		}

		i++;
	}

	if(pos - expression < ex_len) {
		return pos - expression;
	} else {
		return -1;
	}
}

int is_logical_operator(char c)
{
	if(strchr(LOGICAL_OPERATOR, c) != NULL) {
		return 1;
	} else {
		return 0;
	}
}

int expression_to_infix_sentence(const char *exp, char *s[], int size)
{
	int i = 0;
	int j = 0;
	int n = 0;
	int cur = 0;
	int last = 0;
	int pos = 0;
	int end = 0;
	int eno = 0;
	char *buf = NULL;

	end = strlen(exp);
	buf = alloca(end);	/* alloc buf in stack, neendn't free */
	if(buf == NULL) {
		eno = -1;
		goto err;
	}

	for(i = 0; i < end; i++) {
		if(exp[i] == ' ' || exp[i] == '\t') {
			if(i > 0 && exp[i - 1] != '\\') {
				continue;
			}
		}

		buf[j++] = exp[i];
	}
	buf[j] = 0;

	end = strlen(buf);

	while(cur < end && (pos = first_op_pos(buf + cur)) != -1) {
		cur += pos;

		if(cur != last) {
			if(n >= size) {
				eno = -2;
				goto err;
			}
			s[n++] = strndup(buf + last, cur - last);
		}

		if(n >= size) {
			eno = -2;
			goto err;
		}
		s[n++] = strndup(buf + cur, 1);

		cur += 1;
		last = cur;
	}

	if(cur < end && n < size) {
		s[n++] = strdup(buf + cur);
	}

	return n;
err:
	switch(eno) {
	case -1:
		ERROR("alloca failed \n");
		break;
	case -2:
		for(i = 0;i < n; i++) {
			assert(s[i]);
			free(s[i]);
			s[i] = NULL;
		}
		ERROR("could not contain so many sub expression! \n");
		break;
	}

	return eno;
}


int infix_sentence_to_suffix(char *infix[], char *suffix[], int size)
{
	int i = 0;
	int j = 0;
	int eno = 0;
	char *p = NULL;
	struct m_stack stack;

	m_stack_init(&stack);
	
	for(i = 0; i < size; i++) {
		if(!is_logical_operator(infix[i][0])) {
			suffix[j++] = strdup(infix[i]);
			continue;
		}

		if(m_stack_empty(&stack)) {
			m_stack_push(&stack, (intptr_t)infix[i]);
			continue;
		}

		switch(infix[i][0]) {
		case '|':
		case '&':
			while((p = (char *)m_stack_top(&stack)) != NULL) {
				if(p[0] == '|' || p[0] == '&' || p[0] == '!') {
					suffix[j++] = strdup(p);
					m_stack_pop(&stack);
					continue;
				}
				break;
			}

			m_stack_push(&stack, (intptr_t)infix[i]);
			break;
		case '!':
				while((p = (char *)m_stack_top(&stack)) != NULL) {
				if(p[0] == '!') {
					suffix[j++] = strdup(p);
					m_stack_pop(&stack);
					continue;
				}

				m_stack_push(&stack, (intptr_t)infix[i]);
				break;
			}
			break;
		case '(':
			m_stack_push(&stack, (intptr_t)infix[i]);
			break;
		case ')':
			while(1) {
				p = (char *)m_stack_pop(&stack) ;
				if(p == NULL) {
					eno = -1;
					goto err;
				}

				if(p[0] == '(') {
					break;
				}

				suffix[j++] = strdup(p);
			}
			break;
		}
	}

	while((p = (char *)m_stack_pop(&stack)) != NULL) {
		suffix[j++] = strdup(p);
	}

	m_stack_clean(&stack);
	
	return j;
err:
	switch(eno) {
	case -1:
		for(i = 0; i < j; i++) {
			assert(suffix[i]);
			free(suffix[i]);
			suffix[i] = NULL;
		}

		ERROR("Brackets do not match\n");
		break;
	}

	return -1;
}

void free_sentences(char *s[], int n)
{
	int i = 0;

	for(i = 0; i < n; i++) {
		assert(s[i]);
		free(s[i]);
		s[i] = NULL;
	}

	return;
}

/*
 * Term:
 *
 * >>Expression:
 *  eg: 0 & 1
 *      0 & 1 & (1 & 1)
 *      equal(a:0) & equal(b:1)
 *
 *  >>Sentence:
 *  expression consist one or more sentence
 *   eg: 0 is a sentence
 *       & is a sentence
 *       equal(a:0) is a sentence
 *
 *  >>Operator:
 *  operator is special sentence  
 *  they are '!'  '|'  '&'
 *
 *  >>Value:
 *  in 0 & 1
 *  sentence 0's value is 0
 *  sentence 1's value is 1
 *  so expression 0 & 1's value is 0
 *
 *  in equal(a:0) & equal(b:1)
 *  if a == 0 , b ==1
 *  sentence equal(a:0)'value is 1
 *  sentence equal(b:1)'value is 1
 *  so expression equal(a:0) & equal(b:1)'s value is 1
 */

static struct m_logic_sentence *new_logic_sentence()
{
	int eno = 0;
	struct m_logic_sentence *s = NULL;

	s = malloc(sizeof(struct m_logic_sentence));
	if(s == NULL) {
		eno = -1;
		goto err;
	}

	m_binary_tree_zero_node(&s->node);
	s->sentence = NULL;
	s->info.data = NULL;
	s->info.value = 0;
	s->info.operator = 0;

	return s;
err:
	return NULL;
}

static void free_logic_sentence(struct m_logic_sentence *s)
{
	free(s);
}

#define MAX_SENTENCE_NUM		1024
struct m_logic_sentence *expression_to_sentence_tree(const char *expression)
{
	int i = 0;
	int eno = 0;
	int ni = 0;
	int ns = 0;
	char *infix[MAX_SENTENCE_NUM];
	char *suffix[MAX_SENTENCE_NUM];
	struct m_stack s;
	struct m_logic_sentence *node = NULL;
	struct m_logic_sentence *child = NULL;

	if(m_stack_init(&s) < 0) {
		eno = -1;
		goto err;
	}

	ni = expression_to_infix_sentence(expression, infix, MAX_SENTENCE_NUM);
	ns = infix_sentence_to_suffix(infix, suffix, ni);
	
	if(ns <= 0) {
		eno = -2;
		goto err;
	}

	for(i = 0; i < ns; i++) {
		node = new_logic_sentence();
		if(node == NULL) {
			eno = -3;
			goto err;
		}

		if(is_logical_operator(suffix[i][0])) {
			child = (struct m_logic_sentence *)m_stack_pop(&s);
			if(child == NULL) {
				eno = -4;
				goto err;
			}
			m_binary_tree_insert_right(&node->node, &child->node);

			if(suffix[i][0] != '!') {
				child = (struct m_logic_sentence *)m_stack_pop(&s);
				if(child == NULL) {
					eno = -4;
					goto err;
				}
				m_binary_tree_insert_left(&node->node, &child->node);
			}
		}

		node->sentence = strdup(suffix[i]);
		m_stack_push(&s, (intptr_t)node);
	}
	
	node = (struct m_logic_sentence *)m_stack_pop(&s);

	free_sentences(infix, ni);
	free_sentences(suffix, ns);
	m_stack_clean(&s);

	return node;
err:
	switch(eno) {
	case -1:
		break;
	case -2:
		break;
	case -3:
		ERROR("malloc fail \n");
		break;
	case -4:
		ERROR("illegal expression \n");
		free_logic_sentence(node);
		break;
	}

	free_sentences(infix, ni);
	free_sentences(suffix, ns);
	while((node = (struct m_logic_sentence *)m_stack_pop(&s)) != NULL) {
		free_logic_sentence(node);
	}
	m_stack_clean(&s);

	return NULL;
}


void parse_sentence(struct m_binary_tree_node *node, void *arg)
{
	struct m_logic_sentence *n = (struct m_logic_sentence *)node;

	if(is_logical_operator(n->sentence[0])) {
		n->info.operator = n->sentence[0];
	} else {
		if(arg) {
			parse_sentence_func_p func = (parse_sentence_func_p)arg;
			func(n->sentence, &n->info);
		}
	}
}

int m_logic_expression_init(struct m_logic_expression *lexp, const char *exp, parse_sentence_func_p func)
{
	struct m_logic_sentence *s = expression_to_sentence_tree(exp);
	m_binary_tree_traver_LDR(&s->node, parse_sentence, func);
	lexp->root = s;
	return 0;
}

void m_logic_expression_clean(struct m_logic_expression *lexp)
{
}

int m_logic_expression_evaluate(struct m_logic_expression *lexp, sentence_value_func_p func)
{
}

int m_logic_sentence_evaluate(struct m_logic_sentence *s, sentence_value_func_p func)
{
	int value = 0;
	int left_value = 0;
	int right_value = 0;

	if(m_binary_tree_is_leaf_node(&s->node)) {
		value = func(&s->info);
		DEBUG("%s = %d \n", s->sentence, value);
		return value;
	}

	if(s->info.operator == '!') {
		assert(s->node.left == NULL);
		right_value = m_logic_sentence_evluate(s->node.right);
		DEBUG("! %d = %d\n", right_value, !right_value);
		return !right_value;
	}

	left_value = m_logic_sentence_evluate(s->node.left);

	if(s->info.operator == '&' && left_value == 0) {
		DEBUG("%d & ? = %d \n", left_value, 0);
		return 0;
	} else if(s->info.operator == '|' && left_value == 1) {
		DEBUG("%d | ? = %d \n", left_value, 1);
		return 1;
	}

	assert(s->node.right);
	right_value = m_logic_sentence_evluate(s->node.right);

	if(s->info.operator == '&') {
		DEBUG("%d & %d = %d \n", left_value, right_value, left_value & right_value);
		return left_value & right_value;
	} else if(s->info.operator == '|') {
		DEBUG("%d | %d = %d \n", left_value, right_value, left_value | right_value);
		return left_value | right_value;
	} else {
		assert(1);
	}

	return 0;
}

