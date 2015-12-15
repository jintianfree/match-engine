#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "m_log.h"
#include "m_variable.h"
#include "m_operation.h"
#include "m_operator_equal.h"

/* [n,m][I,B]*/
struct equal_operator_option {
	int start;          /* [start, end) */
	int end;
	int ignore_case;    /* I -- ignoring the case of the characters */
    int binary_string;  /* B -- binary string  AC EF 0A */
};

struct equal_operator_option *parse_m_operator_equal_option(const char *option)
{
    struct equal_operator_option *op = NULL;
    
    if(option == NULL) {
        goto end;
    }
    
    char *p1 = strchr(option, '[');
    char *p2 = strchr(option, ']');
    
    if(p2 - p1 > 1) {
        
        char *p3 = strchr(option, ',');
        if(p3 == NULL || p3 > p2) {   /* [ ] or [2] */
        }
        
        /* [,] */
        
        /* [2,] */
        /* [,2] */
        /* [2,3] */
    }
    
    if(strchr(option, 'I') != NULL) {
        op->ignore_case = 1;
    }
    
    if(strchr(option, 'B') != NULL) {
        op->binary_string = 1;
    }
    
end:
    return op;
}

int m_operator_equal_int(struct m_variable *var, const char *value, struct m_operation *operation)
{
	int eno = 0;
	long long int lli = 0;
	unsigned long long  int ulli = 0;

	errno = 0;

	switch(var->real_type) {
	case MRT_UINT8:
	case MRT_UINT16:
	case MRT_UINT32:
	case MRT_UINT64:
		ulli = strtoull(value, NULL, 10);
		operation->value_i = ulli;
		operation->operator_option = NULL;
		break;
	case MRT_INT8:
	case MRT_INT16:
	case MRT_INT32:
	case MRT_INT64:
		lli = strtoll(value, NULL, 10);
		operation->value_i = lli;
		operation->operator_option = NULL;
		break;
	default:
		break;
	}


	if(errno != 0) {
		eno = -1;
		goto err;
	}

	switch(var->real_type) {
	case MRT_UINT8:
		if(ulli > UINT8_MAX) {
			eno = -2;
			goto err;
		}
		break;
	case MRT_UINT16:
		if(ulli > UINT16_MAX) {
			eno = -2;
			goto err;
		}
		break;
	case MRT_UINT32:
		if(ulli > UINT32_MAX) {
			eno = -2;
			goto err;
		}
		break;
	case MRT_UINT64:
		break;
	case MRT_INT8:
		if(lli > INT8_MAX || lli < INT8_MIN) {
			eno = -2;
			goto err;
		}
		break;
	case MRT_INT16:
		if(lli > INT16_MAX || lli < INT16_MIN) {
			eno = -2;
			goto err;
		}
		break;
	case MRT_INT32:
		if(lli > INT32_MAX || lli < INT32_MIN) {
			eno = -2;
			goto err;
		}
		break;
	case MRT_INT64:
		break;
	default:
		break;
	}
	
	return 0;
err:
	switch(eno) {
	case -1:
		ERROR("parse value (%s) error \n", value);
		break;
	case -2:
		ERROR("value (%s) overflow  type (%s) \n", value, m_real_type_2_str(var->real_type));
		break;
	}

	return -1;
}

/* TODO: do not support option now! equal(var_name[][]: value) */
int m_operator_equal_init(struct m_variable *var, const char *option, const char *value, struct m_operation *operation)
{
	(void)option;

	int eno = 0;

	if(value == NULL) {
		eno = -1;
		goto err;
	}

	operation->op = &operator_equal;
	operation->var = var;

	switch(var->real_type) {
	case MRT_UINT8:
	case MRT_INT8:
	case MRT_UINT16:
	case MRT_INT16:
	case MRT_UINT32:
	case MRT_INT32:
	case MRT_UINT64:
	case MRT_INT64:
		if(m_operator_equal_int(var, value, operation) != 0) {
			eno = -2;
			goto err;
		}
		break;
	case MRT_STRING:
		if((operation->value_p = strdup(value)) == NULL) {
			eno = -3;
			goto err;
		}
		operation->value_len = strlen(value);
		operation->operator_option = NULL;
        
        break;
	default:
		eno = -4;
		goto err;
		break;
	}


	return 0;
err:
	switch(eno) {
	case -1:
		ERROR("null value \n");
	        break;
	case -2:
		break;
	case -3:
		ERROR("malloc error @%s:%d \n", __func__, __LINE__);
		break;
	case -4:
		ERROR("equal do not support type %s now", m_real_type_2_str(var->real_type));
		break;
	}

	return -1;
}

int m_operator_equal_value(struct m_operation *operation)
{
	void *p = NULL;
	size_t plen = 0;

	switch(operation->var->store_type) {
	case MST_ADDRESS:
		p = operation->var->var_addr;
		break;
	case MST_POINTER_ADDRESS:
		p = *(void **)(operation->var->var_addr);
		break;
	}
    
    if(p == NULL) {
        return 0;
    }
    
    if(operation->var->var_len) {
        plen = *(operation->var->var_len);
    }   

	switch(operation->var->real_type) {
	case MRT_UINT8:
	case MRT_INT8:
		return *((uint8_t *)(p)) == (uint8_t)(operation->value_i);
		break;
	case MRT_UINT16:
	case MRT_INT16:
		return *((uint16_t *)(p)) == (uint16_t)(operation->value_i);
		break;
	case MRT_UINT32:
	case MRT_INT32:
		return *((uint32_t *)(p)) == (uint32_t)(operation->value_i);
		break;
	case MRT_UINT64:
	case MRT_INT64:
		return *((uint64_t *)(p)) == (uint64_t)(operation->value_i);
		break;
	case MRT_STRING:	
		if(plen != operation->value_len) {
			return 0;
		}
		return (strncmp((char *)p, (char *)operation->value_p, plen) == 0);
        
		break;
	default:
		break;
	}

	return 0;
}

void m_operator_equal_clean(struct m_operation *operation)
{
	switch(operation->var->real_type) {
	case MRT_STRING:
		if(operation->value_p) {
			free(operation->value_p);
			operation->value_p = NULL;
		}
		break;
	default:
		break;
	}

	operation->var = NULL;
	operation->op = NULL;
	operation->value_i = 0;
	operation->operator_option = NULL;
}

struct m_operator operator_equal = {
	.name = "equal",
	.init = m_operator_equal_init,
	.clean = m_operator_equal_clean,
	.value = m_operator_equal_value,
};

