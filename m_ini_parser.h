#ifndef __INI_PARSER_H__
#define __INI_PARSER_H__



/* private */
struct read_ini;
struct config;
struct section;

/* public data structures */
struct ini
{
	int num_sections;
	struct section **sections;	
};


/* public API */

/* This will parse a given ini file contained in filename and return
 * the result as an allocated ini structure. The read_ini structure
 * should be declared and then set to NULL beforehand (this is to make
 * the reader thread-safe. */
struct ini* read_ini(struct read_ini **read_inip, const char *filename);

/* Retrieves a value from a section/key combination and returns that
 * as a string. Note the returned value should not be freed. NULL is
 * returned if the section/key combination can not be found. */
char *ini_get_value(struct ini* ini, const char *section, const char *key);

char *ini_get_value_in_section(struct section *section, const char *key);

/* Pretty print a given ini structure. */
void ini_pp(struct ini* ini);

/* Free memory associated with an ini structure. */
void destroy_ini(struct ini* ini);

/* Free memory associated with a read_ini structure. */
void cleanup_readini(struct read_ini* read_ini);

#endif  /* __INI_PARSER_H__ */
