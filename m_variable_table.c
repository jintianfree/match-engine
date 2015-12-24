#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "m_log.h"
#include "m_common.h"
#include "m_variable_table.h"

struct m_variable_descr_list {
	struct m_variable_descr_list *prev;
	struct m_variable_descr_list *next;
};

struct m_variable_table_manager {
	size_t size;
	struct m_variable_descr_list head;
	/* 0 -- user can register vars; 
	 * 1 -- user can not register vars,
	 *      if registerd, table have malloced will can not use rightly. */
	int state;
};

struct _m_variable_descr {
	struct m_variable_descr descr;
	struct m_variable_descr_list node;
};

static void m_variable_descr_list_insert(struct m_variable_descr_list *head, struct m_variable_descr_list *node);
static void m_variable_descr_list_remove(struct m_variable_descr_list *head, struct m_variable_descr_list *node);

struct m_variable_table * m_variable_table_new(struct m_variable_table_manager *manager)
{
	struct m_variable_table *table = NULL;

	if((table = malloc(sizeof(struct m_variable_table) + manager->size)) == NULL) {
		goto err;
	}
	
	/* we have alloc tables, so can not continue register vars, 
	 * use state to ensure it */
	manager->state = 1;
	table->manager = manager;
	table->base = table + 1;

	return table;
err:
	ERROR("malloc error @%s:%d \n", __func__, __LINE__);
	return NULL;
}

void m_variable_table_free(struct m_variable_table *table)
{
	assert(table);

	free(table);
}

void m_variable_table_zero(struct m_variable_table *table)
{
	struct m_variable_table_manager *manager = NULL;
	struct m_variable_descr_list *node = NULL;
	struct _m_variable_descr *_descr = NULL;
	struct m_variable_descr *descr = NULL;

	manager =  table->manager;

	assert(manager && manager->head.prev == NULL);

	node = manager->head.next;
	while(node != NULL) {
		_descr = container_of(node, struct _m_variable_descr, node);
		descr = &(_descr->descr);

		void *p = NULL;
		size_t *plen = 0;

		switch(descr->store_type) {
		case MST_MEMORY:
			p = table->base + descr->var_offset;
			plen = table->base + descr->var_len_offset;
	
			switch(descr->real_type) {
			case MRT_UINT8:
				*(uint8_t *)p = 0;
				break;
			case MRT_UINT16:
				*(uint16_t *)p = 0;
				break;
			case MRT_UINT32:
				*(uint32_t *)p = 0;
				break;
			case MRT_INT8:
				*(int8_t *)p = 0;
				break;
			case MRT_INT16:
				*(int16_t *)p = 0;
				break;
			case MRT_INT32:
				*(int32_t *)p = 0;
				break;
			case MRT_STRING:
				*plen = 0;
				break;
			default:
				/* if support new type, must add zero code here */
				assert(0);
				break;
			}
			break;
		case MST_ADDRESS:
			*(void **)(table->base + descr->var_offset) = NULL;
			break;
		}

		node = node->next;
	}

	return;

}

struct m_variable_table_manager *m_variable_table_manager_init()
{
	struct m_variable_table_manager *manager = NULL;

	if((manager = malloc(sizeof(struct m_variable_table_manager))) == NULL) {
		goto err;
	}
	manager->size = 0;
	manager->head.prev = NULL;
	manager->head.next = NULL;
	manager->state = 0;

	return manager;
err:
	ERROR("malloc fail @%s:%d \n", __func__, __LINE__);
	return NULL;
}

void m_variable_table_manager_clean(struct m_variable_table_manager *manager)
{
	assert(manager);

	free(manager);

	return;
}

size_t m_variable_descr_register(struct m_variable_table_manager *manager, struct m_variable_descr vars[], size_t size)
{
	int i = 0; 
	int eno = 0;
	size_t base = 0;
	struct m_variable_descr *descr = NULL;
	struct _m_variable_descr *_descr = NULL;

	assert(manager->head.prev == NULL);

	/* if state==1, some table have alloced by m_variable_table_new() 
	 * if continue register, these tables have alloced will wrongly use. */
	assert(manager->state == 0);

	/* check name, we don't allow two variable have same name. */
	for(i = 0; vars[i].var_name[0] != 0; i++) {
		descr = m_variable_descr_find_by_name(manager, vars[i].var_name);
		if(descr) {
			eno = -1;
			goto err;
		}
	}
	
	if(manager->size == 0) {
		manager->size += 4;
	}

	/* TODO: memory alignment */
	base = manager->size;

	for(i = 0; vars[i].var_name[0] != 0; i++) {
		if((_descr = malloc(sizeof(struct _m_variable_descr))) == NULL) {
			eno = -2;
			goto err;
		}

		memcpy(&(_descr->descr), &vars[i], sizeof(struct m_variable_descr));
		_descr->descr.var_offset += base;
		_descr->descr.var_len_offset += base;	/* TODO: var_len_offset == 0 */

		m_variable_descr_list_insert(&manager->head, &(_descr->node));
	}

	manager->size += size;

	return base;
err:
	switch(eno) {
	case -1:
		ERROR("variable name %s has registered, try to use a different name \n", descr->var_name);
		break;
	case -2:
		/* TODO: free malloced */
		ERROR("malloc fail @%s:%d \n", __func__, __LINE__);
		break;
	}

	return 0;

}

void m_variable_descr_unregister(struct m_variable_table_manager *manager, struct m_variable_descr vars[])
{
	int i = 0;
	struct _m_variable_descr *_descr = NULL;
	struct m_variable_descr *descr = NULL;

	for(i = 0; vars[i].var_name[0] != 0; i++) {
		descr = m_variable_descr_find_by_name(manager, vars[i].var_name);
		if(descr) {
			_descr = container_of(descr, struct _m_variable_descr, descr);
			m_variable_descr_list_remove(&manager->head, &_descr->node);
			free(_descr);
			_descr = NULL;
		}
	}

	return;
}

void m_variable_table_print(struct m_variable_table *table)
{
	struct m_variable_table_manager *manager = NULL;
	struct m_variable_descr_list *node = NULL;
	struct _m_variable_descr *_descr = NULL;
	struct m_variable_descr *descr = NULL;

	manager =  table->manager;

	assert(manager && manager->head.prev == NULL);

	node = manager->head.next;
	while(node != NULL) {
		_descr = container_of(node, struct _m_variable_descr, node);
		descr = &(_descr->descr);

		void *p = NULL;
		size_t *plen = 0;

		printf("name: %s type: %s value:", descr->var_name, 
				m_variable_real_type_2_str(descr->real_type));

		switch(descr->store_type) {
		case MST_MEMORY:
			p = table->base + descr->var_offset;
			break;
		case MST_ADDRESS:
			p = *(void **)(table->base + descr->var_offset);
			break;
		}

		plen = table->base + descr->var_len_offset;

		switch(descr->real_type) {
		case MRT_UINT8:
			printf("%u ", *(uint8_t *)p);
			break;
		case MRT_UINT16:
			printf("%u ", *(uint16_t *)p);
			break;
		case MRT_UINT32:
			printf("%u ", *(uint32_t *)p);
			break;
		case MRT_INT8:
			printf("%d ", *(int8_t *)p);
			break;
		case MRT_INT16:
			printf("%d ", *(int16_t *)p);
			break;
		case MRT_INT32:
			printf("%d ", *(int32_t *)p);
			break;
		case MRT_STRING:
			nprintf(p, *plen);
			break;
		default:
			break;
		}

		printf("\n");

		node = node->next;
	}

	return;
}

const char *m_variable_real_type_2_str(enum m_var_real_type type)
{
	static char name[12];

	switch(type) {
	case MRT_UINT8:
		strcpy(name, "UINT8");
		break;
	case MRT_UINT16:
		strcpy(name, "UINT16");
		break;
	case MRT_UINT32:
		strcpy(name, "UINT32");
		break;
	case MRT_UINT64:
		strcpy(name, "UINT64");
		break;
	case MRT_INT8:
		strcpy(name, "INT8");
		break;
	case MRT_INT16:
		strcpy(name, "INT16");
		break;
	case MRT_INT32:
		strcpy(name, "INT32");
		break;
	case MRT_INT64:
		strcpy(name, "INT64");
		break;
	case MRT_FLOAT:
		strcpy(name, "FLOAT");
		break;
	case MRT_DOUBLE:
		strcpy(name, "DOUBULE");
		break;
	case MRT_ABSOLUTE_TIME:
		strcpy(name, "ABSOLUTE_TIME");
		break;
	case MRT_RELATIVE_TIME:
		strcpy(name, "RELATIVE_TIME");
		break;
	case MRT_STRING:
		strcpy(name, "STRING");
		break;
	case MRT_BYTES:
		strcpy(name, "BYTES");
		break;
	case MRT_IPv4:
		strcpy(name, "IPV4");
		break;
	case MRT_IPv6:
		strcpy(name, "IPV6");
		break;
	default:
		strcpy(name, "unknown type");
		break;
	}

	return name;

}

struct m_variable_descr *m_variable_descr_find_by_name(struct m_variable_table_manager *manager, const char *name)
{
	struct m_variable_descr_list *node = NULL;
	struct _m_variable_descr *_descr = NULL;
	struct m_variable_descr *descr = NULL;

	node = manager->head.next;

	while(node != NULL) {
		_descr = container_of(node, struct _m_variable_descr, node);
		descr = &_descr->descr;

		if(strcmp(descr->var_name, name) == 0) {
			return descr;
		}

		node = node->next;
	}

	return NULL;
}

static void m_variable_descr_list_insert(struct m_variable_descr_list *head, struct m_variable_descr_list *node)
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

static void m_variable_descr_list_remove(struct m_variable_descr_list *head, struct m_variable_descr_list *node)
{
	assert(head->prev == NULL && head->next != NULL);
	assert(node->prev != NULL);

#if 1
	/* check if the node belongs to this list */
	struct m_variable_descr_list *next = NULL; 

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

