/*
 * Clone from github
 * URL:
 * https://github.com/nowl/iniparser
 * https://github.com/nowl/iniparser.git
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "m_log.h"
#include "m_ini_parser.h"

struct config
{
	char *key;
	char *value;
};

struct section
{
	char *name;
	int num_configs;
	struct config **configs;
};


enum States
{
	START,
	NEW_SECTION,	
	IN_SECTION,
	END_OF_FILE
};

struct read_ini
{
	FILE *fin;
	char *tmp;
	int tmp_cap;
	int current_line;
	int state;
};

/* Trims whitespace from the passed in string between begin_ind and
 * end_ind characters in the string. It returns the begin index and
 * the end index of the first and last valid (non-whitespace)
 * characters in the string. */
static void
trim(char *str, int *begin_ind, int *end_ind)
{
	int b = *begin_ind;
	int e = *end_ind;
	int i;

	/* find first index of non-whitespace character */
	for(i=b; i<e; i++)
	{
		char c = str[i];
		if(c != ' ' && c != '\t' && c != '\n' && c != '\r')
			break;
	}
	*begin_ind = i;
			
	/* find last index of non-whitespace character */
	for(i=e-1; i>=b; i--)
	{
		char c = str[i];
		if(c != ' ' && c != '\t' && c != '\n' && c != '\r')
			break;
	}
	*end_ind = i;
}

static int
read_line(struct read_ini *read_ini)
{
	int i = 0;
	int b;
	int new = 0;

	for(;;)
	{
		char c;

		/* resize buffer if necessary */
		if(i >= read_ini->tmp_cap)
		{
			read_ini->tmp_cap *= 2;
			char *p = realloc(read_ini->tmp, sizeof(*read_ini->tmp) * read_ini->tmp_cap);
			if(p) {
				read_ini->tmp = p;
			} else {
				ERROR("malloc error @%s:%d \n", __func__, __LINE__);
				free(read_ini->tmp);
				return -1;
			}
		}

		/* read next byte from file */
		b = getc(read_ini->fin);

		/* if end of file then end */
		if(b == EOF)
		{
			read_ini->state = END_OF_FILE;
			break;
		}

		/* test for end of line */
		c = b;
		if(c == '\r' || c == '\n')
        {
            read_ini->current_line++;
			break;
        }

		/* place char in buffer */
		read_ini->tmp[i++] = c;		
		new = 1;
	}
	
	/* place end-of-string marker */
	if(new)
		read_ini->tmp[i] = '\0';

	return new;
}

static struct section *
parse_section(struct read_ini *read_ini)
{
	int new = 0;
	struct section *section = NULL;
    
    if((section = malloc(sizeof(*section))) == NULL) {
		goto err;
	}
    
	section->num_configs = 0;
	section->configs = NULL;
	
	for(;;)
	{
		new = 0;

		switch(read_ini->state)
		{
		case START:
		case IN_SECTION:
			new = read_line(read_ini);
			break;
		case NEW_SECTION:
			new = 1;
			break;
		case END_OF_FILE:
			return section;
		default:
			printf("invalid state %d in parse_section\n", read_ini->state);
			break;
		}

		if(new > 0)
		{
			int i, x, y, b=0, e=strlen(read_ini->tmp);
			struct config *cfg;
			trim(read_ini->tmp, &b, &e);

			/* check for comments */
			if(read_ini->tmp[b] == '#' || read_ini->tmp[b] == ';')
				continue;

			/* check for new section */
			if(read_ini->tmp[b] == '[')
			{
				if(read_ini->state == IN_SECTION)
				{
					read_ini->state = NEW_SECTION;
					break;
				}
				else
				{
					/* fill section name */
					if(read_ini->tmp[e] == ']') {
						read_ini->tmp[e] = '\0';
					} else {
						read_ini->tmp[e + 1] = '\0';
					}
					if((section->name = strdup(&read_ini->tmp[b+1])) == NULL) {
						goto err;
					}
					
					read_ini->state = IN_SECTION;
					continue;
				}
			}

			if((cfg = malloc(sizeof(*cfg))) == NULL) {
				goto err;
			}
			
			/* read key */

			for(i=b; i<=e; i++)		
			{
				char c = read_ini->tmp[i];
				if(c == ':' || c == '=')
					break;
			}
			x = b; y = i;
			trim(read_ini->tmp, &x, &y);
			read_ini->tmp[y+1] = '\0';
			if((cfg->key = strdup(&read_ini->tmp[x])) == NULL) {
				goto err;
			}

			/* read value */
            x = i+1;
            y = e+1;
               
			if(x > e) {
                /* only have a key */
                cfg->value = strdup("");
            } else {
                trim(read_ini->tmp, &x, &y);
                read_ini->tmp[y+1] = '\0';
                cfg->value = strdup(&read_ini->tmp[x]);
			}
            
			if(cfg->value == NULL) {
				goto err;
			}

			/* add the config to the section */
			
			if(section->configs)
			{
				section->num_configs++;
				section->configs = realloc(section->configs, sizeof(*section->configs) * section->num_configs);
			}
			else
			{
				section->configs = malloc(sizeof(*section->configs));
				section->num_configs = 1;
			}

			if(section->configs == NULL) {
				goto err;
			}

			section->configs[section->num_configs-1] = cfg;
		} else if (new < 0){
			goto err;
		}
	}

	return section;
err:
	ERROR("malloc error @ %s:%d \n", __func__, __LINE__);
	return NULL;
}

static struct ini* parse_ini(struct read_ini *read_ini)
{
	struct ini *ini = NULL;
	char finished = 0;
    
	if((ini = malloc(sizeof(*ini))) == NULL) {
		goto err;
	}
    
	ini->num_sections = 0;
	ini->sections = NULL;

	while(!finished)
	{
		switch(read_ini->state)
		{
		case START:
		case NEW_SECTION:
		{
			/* read a section */
			struct section * section = parse_section(read_ini);

			if(!section)
				break;

			/* add it to the sections structure */
			if(ini->sections)
			{
				ini->num_sections++;
				void *p = realloc(ini->sections, sizeof(*ini->sections) * ini->num_sections);
				if(p == NULL) {
					goto err;
				}
				ini->sections = p;
			}
			else
			{
				if((ini->sections = malloc(sizeof(*ini->sections))) == NULL) {
					goto err;
				}
				ini->num_sections = 1;
			}
			ini->sections[ini->num_sections-1] = section;
			break;
		}
		case END_OF_FILE:
			finished = 1;
			break;
		default:
			ERROR("error parsing file at line %d\n", read_ini->current_line);
			goto err;
			finished = 1;
			break;
		
        }
	}

	return ini;
err:
	if(ini && ini->sections) {
		/* TODO: free sections && sections->configs */
	}

	if(ini) {
		free(ini);
		ini = NULL;
	}

	return NULL;
}

struct ini* read_ini(struct read_ini **read_inip, const char *filename)
{
	int eno = 0;
	struct ini *ini = NULL; 
    struct read_ini *read_ini = *read_inip;
    
	assert(read_ini == NULL);

    if((read_ini = malloc(sizeof(*read_ini))) == NULL) {
		eno = -1;
		goto err;
	}

	*read_inip = read_ini;
        
	read_ini->current_line = 0;
	read_ini->state = START;
	if((read_ini->tmp = malloc(sizeof(*read_ini->tmp) * 4)) == NULL) {
		eno = -1;
		goto err;
	}
	read_ini->tmp_cap = 4;
	if((read_ini->fin = fopen(filename, "r")) == NULL) {
		eno = -2;
		goto err;
	}

	ini = parse_ini(read_ini);

	fclose(read_ini->fin);

	return ini;
err:
	switch(eno) {
	case -1:
		ERROR("malloc error @%s:%d \n", __func__, __LINE__);
		break;
	case -2:
		ERROR("fail to open file:%s \n", filename);
		break;
	}

	if(read_ini && read_ini->tmp) {
		free(read_ini->tmp);
		read_ini->tmp = NULL;
	}

	if(read_ini && read_ini->fin) {
		fclose(read_ini->fin);
	}

	if(read_ini) {
		free(read_ini);
		read_ini = NULL;
	}

	*read_inip = NULL;

	return NULL;
}

char *ini_get_value_in_section(struct section *section, const char *key)
{
    int c = 0;
    
    for(c = 0; c < section->num_configs; c++) {
        if(strcmp(key, section->configs[c]->key) == 0) {
            return section->configs[c]->value;
        }
    }
    
    return NULL;
}

/* This does a linear search through the keys in the parsed ini file
 * so should happen in O(n) time. This could be made better through
 * using hashtables but I don't think this will generally be the
 * bottleneck (how large can ini files get?) */
char *
ini_get_value(struct ini* ini,
              const char *section,
              const char *key)
{
    int s = 0;

	for(s = 0; s < ini->num_sections; s++)
	{
        if( strcmp(section, ini->sections[s]->name) == 0 )
            return ini_get_value_in_section(ini->sections[s], key);  
	}

    return NULL;
}

/* pretty print the structure */
void
ini_pp(struct ini* ini)
{
	int s, c;
	printf("num sections: %d\n", ini->num_sections);
	for(s=0; s<ini->num_sections; s++)
	{
		printf("section: \"%s\" ", ini->sections[s]->name);
		printf("(num configs: %d)\n", ini->sections[s]->num_configs);
		for(c=0; c<ini->sections[s]->num_configs; c++)
			printf("  key: \"%s\", value: \"%s\"\n",
				   ini->sections[s]->configs[c]->key,
				   ini->sections[s]->configs[c]->value);
	}
}

void
destroy_ini(struct ini* ini)
{
	int s, c;
	for(s=0; s<ini->num_sections; s++)
	{
		for(c=0; c<ini->sections[s]->num_configs; c++)
		{
			free(ini->sections[s]->configs[c]->key);
			free(ini->sections[s]->configs[c]->value);
		}
		free(ini->sections[s]->name);
		free(ini->sections[s]->configs);
	}

	free(ini->sections);
	free(ini);
}

void
cleanup_readini(struct read_ini* read_ini)
{
    free(read_ini->tmp);
    free(read_ini);
}
