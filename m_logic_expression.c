#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "m_common.h"
#include "m_log.h"
#include "m_stack.h"
#include "m_binary_tree.h"
#include "m_logic_expression.h"

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

#define LOGICAL_OPERATOR	"!&|{}"
#define ESCAPE_CHAR			'\\'
#define SPECIAL_CHAR		" \t"

struct m_logic_sentence {
	struct m_binary_tree_node node;	/* must first, so &node == &m_logic_sentence */
	char *sentence;
#define SENTENCE_PARSE_ERROR	-1
#define SENTENCE_NULL			0
#define SENTENCE_UNPARSE		1
#define SENTENCE_PARSED			2
	int state;
	union sentence_info info;
};

struct m_logic_sentence *create_sentence_tree(const char *expression);
static void release_sentence_tree(struct m_logic_sentence *s);

static int first_op_pos(const char *expression)
{
	char *op = LOGICAL_OPERATOR;
	char escape = ESCAPE_CHAR;

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

static int is_logical_operator(char c)
{
	if(strchr(LOGICAL_OPERATOR, c) != NULL) {
		return 1;
	} else {
		return 0;
	}
}

static int expression_to_infix_sentence(const char *exp, char *s[], int size)
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
	buf = malloc(end + 1); /* +1 for \0 */
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

		
	free(buf);

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
		ERROR("could not contain so many sentence! \n");
		break;
	}

	if(buf) {
		free(buf);
		buf = NULL;
	}

	return eno;
}

static void free_sentences(char *s[], int n)
{
	int i = 0;

	for(i = 0; i < n; i++) {
		assert(s[i]);
		free(s[i]);
		s[i] = NULL;
	}

	return;
}

static void print_sentences(char *s[], int n) 
{
	int i = 0;

	for(i = 0; i < n; i++) {
		printf("%s ", s[i]);
	}
	printf("\n");
}

static int infix_sentence_to_suffix(char *infix[], char *suffix[], int size)
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

		switch(infix[i][0]) {	/* ! > & > | */
		case '|':
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
		case '&':
			while((p = (char *)m_stack_top(&stack)) != NULL) {
				if(p[0] == '&' || p[0] == '!') {
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
		case '{':
			m_stack_push(&stack, (intptr_t)infix[i]);
			break;
		case '}':
			while(1) {
				p = (char *)m_stack_pop(&stack) ;
				if(p == NULL) {
					eno = -1;
					goto err;
				}

				if(p[0] == '{') {
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
		print_sentences(infix, size);
		break;
	}

	return -1;
}

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
	s->state = SENTENCE_NULL;
	s->info.data = NULL;
	s->info.value = 0;
	s->info.op = 0;

	return s;
err:
	switch(eno) {
	case -1:
		break;
	}
	return NULL;
}

static void free_logic_sentence(struct m_logic_sentence *s)
{
	free(s);
}

#define MAX_SENTENCE_NUM		1024
struct m_logic_sentence *create_sentence_tree(const char *expression)
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
		if((node = new_logic_sentence()) == NULL) {
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

		/* TODO: check NULL */
		if((node->sentence = strdup(suffix[i])) == NULL) {
			eno = -5;
			goto err;
		}
		node->state = SENTENCE_UNPARSE;
		m_stack_push(&s, (intptr_t)node);
	}
	
	node = (struct m_logic_sentence *)m_stack_pop(&s);
	if(m_stack_top(&s) != 0) {
		eno = -6;
		goto err;
	}

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
		ERROR("illegal expression, too many logic operator :%s\n", expression);
		release_sentence_tree(node);
		break;
	case -5:
		ERROR("malloc fail \n");
		release_sentence_tree(node);
		break;
	case -6:
		ERROR("illegal expression, short of logic operator :%s\n", expression);
		release_sentence_tree(node);
		break;
	}

	free_sentences(infix, ni);
	free_sentences(suffix, ns);
	while((node = (struct m_logic_sentence *)m_stack_pop(&s)) != NULL) {
		release_sentence_tree(node);
	}
	m_stack_clean(&s);

	return NULL;
}

static int release_sentence_tree_node(struct m_binary_tree_node *node, void *arg)
{
	(void)arg;
	struct m_logic_sentence *n = (struct m_logic_sentence *)node;
	if(n->sentence) {
		free(n->sentence);
		n->sentence = NULL;
	}
	free_logic_sentence(n);

	return 0;
}

static void release_sentence_tree(struct m_logic_sentence *s)
{
	m_binary_tree_traver_LRD(&s->node, release_sentence_tree_node, NULL);
}

static int parse_sentence(struct m_binary_tree_node *node, void *arg)
{
	int eno = 0;
	struct m_logic_sentence *n = (struct m_logic_sentence *)node;

	if(is_logical_operator(n->sentence[0])) {
		n->info.op = n->sentence[0];

		return 0;
	}

	int i = 0;
	int j = 0;
	int len = strlen(n->sentence);
	char *s = malloc(len + 1);	/* +1 for \0 */
	if(s == NULL) {
		eno = -1;
		goto err;
	} 

	for(i = 0; i < len; i++) {
		if(n->sentence[i] == ESCAPE_CHAR) {
			continue;
		}
	
		s[j++] = n->sentence[i];
	}
	if(j == 0) {
		eno = -2;
		goto err;
	}

	s[j] = 0;

	struct {struct m_sentence_handle *handle; void *arg;} *args = arg;
	struct m_sentence_handle *handle = args->handle;

	if(handle->parse(s, &n->info, args->arg) != 0) {
		eno = -2;
		goto err;
	}

	n->state = SENTENCE_PARSED;

	if(s) {
		free(s);
		s = NULL;
	}

	return 0;

err:
	switch(eno) {
	case -1:
		ERROR("couldn't malloc enough memory to contain: %s \n", n->sentence);
		break;
		ERROR("illegal sentence: %s \n", n->sentence);
	case -2:
		break;
	case -3:
		break;
	}

	if(s) {
		free(s);
		s = NULL;
	}

	n->state = SENTENCE_PARSE_ERROR;

	return -1;
}

static int clean_sentence(struct m_binary_tree_node *node, void *arg)
{
	struct m_logic_sentence *n = (struct m_logic_sentence *)node;

	if(!is_logical_operator(n->sentence[0])) {
		if(arg && n->state == SENTENCE_PARSED) {
			sentence_clean func = (sentence_clean)arg;
			func(&n->info);
		}
	}

	return 0;
}

static int m_logic_sentence_evaluate(struct m_binary_tree_node *node, 
		sentence_value func, void *arg)
{
	assert(node);

	int value = 0;
	int left_value = 0;
	int right_value = 0;
	struct m_logic_sentence *s = (struct m_logic_sentence *)node;

	if(m_binary_tree_is_leaf_node(&s->node)) {
		value = func(&s->info, arg);
		DEBUG("%s = %d \n", s->sentence, value);
		goto end;
	}

	if(s->info.op == '!') {
		assert(s->node.left == NULL);
		right_value = m_logic_sentence_evaluate(s->node.right, func, arg);

		value = !right_value;
		DEBUG("! %d = %d\n", right_value, value);
		goto end;
	}

	left_value = m_logic_sentence_evaluate(s->node.left, func, arg);

	if(s->info.op == '&' && left_value == 0) {
		value = 0;
		DEBUG("%d & ? = %d \n", left_value, value);
		goto end;
	} else if(s->info.op == '|' && left_value == 1) {
		value = 1;
		DEBUG("%d | ? = %d \n", left_value, value);
		goto end;
	}

	assert(s->node.right);

	right_value = m_logic_sentence_evaluate(s->node.right, func, arg);

	if(s->info.op == '&') {
		value = left_value & right_value;
		DEBUG("%d & %d = %d \n", left_value, right_value, value);
		goto end;
	} else if(s->info.op == '|') {
		value = left_value | right_value;
		DEBUG("%d | %d = %d \n", left_value, right_value, value);
		goto end;
	} else {
		assert(1);
	}

end:
	return value;
}

int  m_logic_expression_init(struct m_logic_expression *lexp, 
		struct m_sentence_handle *handle, const char *exp, void *arg)
{
	assert(lexp && handle && exp);

	int eno = 0;
	struct m_logic_sentence *root = NULL;

	if((root = create_sentence_tree(exp)) == NULL) {
		eno = -1;
		goto err;
	}

	struct {struct m_sentence_handle *handle; void *arg;} args = {handle, arg};

	if(m_binary_tree_traver_LDR(&root->node, parse_sentence, &args) != 0) {
		eno = -2;
		goto err;
	}

	lexp->root = root;
	lexp->handle = handle;

	return 0;
err:
	switch(eno) {
	case -1:
		break;
	case -2:
		/* some of sentence in exp has parsed, so must clean that */
		m_binary_tree_traver_LDR(&root->node, clean_sentence, handle->clean);

		release_sentence_tree(root);
		break;
	}
	ERROR("m_logic_expression (%s)  init error (%d) \n", exp, eno);

	return -1;
}

void m_logic_expression_clean(struct m_logic_expression *lexp)
{
	assert(lexp && lexp->root && lexp->handle);

	m_binary_tree_traver_LDR(&lexp->root->node, clean_sentence, lexp->handle->clean);

	release_sentence_tree(lexp->root);

	lexp->root = NULL;
	lexp->handle = NULL;

	return;
}

int m_logic_expression_evaluate(struct m_logic_expression *lexp, void *arg)
{
	assert(lexp && lexp->root && lexp->handle);

	int value = 0;

	value = m_logic_sentence_evaluate(&lexp->root->node, lexp->handle->value, arg);

	return value;
}

