#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "m_log.h"
#include "m_variable.h"

struct m_variable_list {
	struct m_variable *var;
	int num;
	struct m_variable_list *prev;
	struct m_variable_list *next;
};

struct m_variable_list *head = NULL;

int m_variable_list_register(struct m_variable vars[], int num)
{
	int i = 0; 
	int j = 0;
	int eno = 0;
	struct m_variable *var = NULL;
	struct m_variable_list *list = NULL;
	

	/* check name, we don't allow two variable have same name. */
	list = head;
	while(list != NULL) {
		for(i = 0; i < list->num; i++) {
			var = &list->var[i];

			for(j = 0; j < num; j++) {
				if(strcmp(vars[j].var_name, var->var_name) == 0) {
					eno = -1;
					goto err;
				}
			}
		}
		
		list = list->next;
	}

	list = malloc(sizeof(struct m_variable_list));
	if(list == NULL) {
		eno = -2;
		goto err;
	}

	list->var = vars;
	list->num = num;
	list->next = head;
	list->prev = NULL;

	if(head) {
		head->prev = list;
	}
	head = list;

	return 0;
err:
	switch(eno) {
	case -1:
		ERROR("variable name %s has registered, try to use a different name \n", vars[j].var_name);
		break;
	case -2:
		ERROR("fail to malloc @%s:%d \n", __func__, __LINE__);
		break;
	}

	return -1;
}

void m_variable_list_unregister(struct m_variable vars[], int num)
{
	struct m_variable_list *list = NULL;

	list = head;
	while(list != NULL) {
		if(list->num == num && list->var == vars) {
			if(list == head) {
				head = list->next;
			}


			if(list->prev) {
				list->prev->next = list->next;
			}

			if(list->next) {
				list->next->prev = list->prev;
			}

			free(list);

			break;
		}

		list = list->next;
	}

	return;
}

void m_variable_print(struct m_variable *var)
{
	printf("name: %s type: %d addr: %x len_addr: %x \n",
			var->var_name, var->type, var->var_addr, var->var_len);
}

void m_variable_list_print()
{
	int i = 0;
	struct m_variable_list *list = NULL;

	list = head;

	while(list != NULL) {
		for(i = 0; i < list->num; i++) {
			m_variable_print(&list->var[i]);
		}

		list = list->next;
	}
}

struct m_variable *name_2_var(struct m_variable_list *head, const char *name)
{
	int i = 0; 
	struct m_variable *var = NULL;
	struct m_variable_list *list = NULL;
	
	list = head;
	while(list != NULL) {
		for(i = 0; i < list->num; i++) {
			var = &list->var[i];

			if(strcmp(name, var->var_name) == 0) {
				return var;
			}
		}
		
		list = list->next;
	}

	return NULL;
}

