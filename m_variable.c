#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "m_common.h"
#include "m_log.h"
#include "m_variable.h"

void m_variable_list_insert(struct m_variable_list *head, struct m_variable_list *node);
void m_variable_list_remove(struct m_variable_list *head, struct m_variable_list *node);

int m_variable_list_register(struct m_variable_list *head, struct m_variable vars[])
{
	int i = 0; 
	int eno = 0;
	struct m_variable_list *node = NULL;
	struct m_variable *var = NULL;

	assert(head->prev == NULL);

	/* check name, we don't allow two variable have same name. */
	node = head->next;
	while(node != NULL) {
		var = container_of(node, struct m_variable, node);

		for(i = 0; vars[i].var_addr != NULL; i++) {
			if(strcmp(vars[i].var_name, var->var_name) == 0) {
				eno = -1;
				goto err;
			}
		}

		node = node->next;
	}

	for(i = 0; vars[i].var_addr != NULL; i++) {
		m_variable_list_insert(head, &vars[i].node);
	}

	return 0;
err:
	switch(eno) {
	case -1:
		ERROR("variable name %s has registered, try to use a different name \n", vars->var_name);
		break;
	}

	return -1;
}

void m_variable_list_unregister(struct m_variable_list *head, struct m_variable vars[])
{
    int i = 0;
    
	for(i = 0; vars[i].var_addr != NULL; i++) {
		m_variable_list_remove(head, &vars[i].node);
	}

	return;
}

void m_variable_print(struct m_variable *var)
{
	printf("name: %s type: %d ", var->var_name, var->real_type);
	
	void *p = NULL;

	switch(var->store_type) {
	case MST_ADDRESS:
		p = var->var_addr;
		break;
	case MST_POINTER_ADDRESS:
		p = *(void **)var->var_addr;
		break;
	}

	switch(var->real_type) {
    case MRT_UINT8:
    case MRT_UINT16:
	case MRT_UINT32:
		printf("%u ", *(uint32_t *)p);
		break;
	case MRT_STRING:
        nprintf((char *)p, *var->var_len);
		break;
    default:
        break;
	}
    
    printf("\n");
    
    return;
}

void m_variable_list_print(struct m_variable_list *head)
{
    struct m_variable *var = NULL;
	struct m_variable_list *node = NULL;

	assert(head->prev == NULL);

	node = head->next;
    
	while(node != NULL) {
		var = container_of(node, struct m_variable, node);
        m_variable_print(var);
        node = node->next;
    }
    
    return;
}

struct m_variable *name_2_var(struct m_variable_list *head, const char *name)
{
    struct m_variable *var = NULL;
	struct m_variable_list *node = NULL;

	assert(head->prev == NULL);

	node = head->next;
    
	while(node != NULL) {
		var = container_of(node, struct m_variable, node);
		if(strcmp(name, var->var_name) == 0) {
			return var;
		}
        node = node->next;
    }
    
    return NULL;
}

void m_variable_list_insert(struct m_variable_list *head, struct m_variable_list *node)
{
	assert(head->prev == NULL);

	if(head->next) {
		head->next->prev = node;
	}
	
	node->prev = head;
    node->next = head->next;
    head->next = node;

	return;
}

void m_variable_list_remove(struct m_variable_list *head, struct m_variable_list *node)
{
	assert(head->prev == NULL && head->next != NULL);
    assert(node->prev != NULL);

#if 1
	/* check if the node belongs to this list */
	struct m_variable_list *next = NULL; 

	next = head->next;
	while(next != NULL) {
		if(next == node) {
			break;
		}
		next = next->next;
	}

	if(node != next) {
        goto err;
    }
#endif 

	if(node->next) {
		node->next->prev = node->prev;
	}

	node->prev->next = node->next;

    node->prev = NULL;
    node->next = NULL;
    
	return;
err:
    ERROR("you are removeing a node not belongs to a list \n");
    return;
}
