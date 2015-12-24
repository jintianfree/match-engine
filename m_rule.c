#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "m_common.h"
#include "m_log.h"
#include "m_ini_config.h"
#include "m_variable_table.h"
#include "m_logic_operation.h"

#define MAX_NAME_LEN 128
#define MAX_MATCH_LEN 1024
#define MAX_ACTION_LEN 1024

struct m_rule_list {
	struct m_rule_list *prev;
	struct m_rule_list *next;
};

struct m_rule {
	int id;
	char name[MAX_NAME_LEN];
	struct m_logic_operation *op;
	struct m_rule_list node;
};

/*
 * [RULE n]
 * id = 
 * name = 
 * match = 
 * action = action-name {var1}  {var2}  str1 str2 | action-name2 {var1} str1
 */

struct m_rule_list *m_rule_init(const char *file, struct m_variable_table_manager *manager)
{
	int eno = 0;
	int ret = 0;
 	int id = 0;
	char name[MAX_NAME_LEN];
	char match[MAX_MATCH_LEN];
	char action[MAX_ACTION_LEN];

	struct m_rule_list *head = NULL;
	struct m_ini_config *ini = NULL;

 	struct m_ini_config_descr descr[] = {
		{"", "id", &id, 0, ICVT_INT, 0, "1"},
		{"", "name", name, MAX_NAME_LEN, ICVT_STRING, 0, "{default name}"},
		{"", "match", match, MAX_MATCH_LEN, ICVT_STRING, 1, ""},
		{"", "action", action, MAX_ACTION_LEN, ICVT_STRING, 0, ""},
		{"", "", NULL, 0, 0, 0, ""},
	};


	if((ini = m_ini_config_init(file)) == NULL) {
		eno = -1;
		goto err;
	}

	while((ret = m_ini_config_next(ini, descr)) != 0) {
		if(ret == -1) {
			ERROR("x");
			continue;
		}

		struct m_rule *rule = NULL;
		struct m_logic_operation *op = NULL; 

		if((op = m_logic_operation_init(match, manager)) == NULL) {
			eno = -2;
			goto err;
		}

		if((rule = malloc(sizeof(struct m_rule))) == NULL) {
			eno = -3;
			goto err;
		}

		rule->id = id;
		strcpy(rule->name, name);
		rule->op = op;

		if(head != NULL) {
			head->prev = &rule->node;
			rule->node.next = head;
			rule->node.prev = NULL;
		}

		head = &rule->node;
	}

	m_ini_config_clean(ini);

	return head;
err:
	return NULL;
}

int m_rule_match(struct m_rule_list *head, struct m_variable_table *table)
{
	int n = 0;
	struct m_rule *rule = NULL;
	struct m_rule_list *node = head;

	while(node != NULL) {
		rule = container_of(node, struct m_rule, node);

		if(m_logic_operation_value(rule->op, table)) {
			n++;
		}

		node = node->next;
	}

	return n;
}

void m_rule_clean(struct m_rule_list *head)
{
	(void)head;
}
