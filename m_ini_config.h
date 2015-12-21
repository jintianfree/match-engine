#ifndef _M_INI_CONFIG_H_
#define _M_INI_CONFIG_H_

enum m_ini_config_value_type {
	ICVT_STRING = 0,
	ICVT_INT,
	ICVT_UINT,
	ICVT_LONG,
	ICVT_ULONG,
	ICVT_BOOL,
};

#define MAX_INI_CONFIG_SECTION_LEN			32
#define MAX_INI_CONFIG_KEY_LEN				32
#define MAX_INI_CONFIG_DEFAULT_VALUE_LEN		512
#define M_INI_CONFIG_DESCR_NULL				{"", "", NULL, 0, 0, ""}
struct m_ini_config_descr {
	char section[MAX_INI_CONFIG_SECTION_LEN];
	char key[MAX_INI_CONFIG_KEY_LEN];
	void *addr;
	int max_len;
	enum m_ini_config_value_type type;
	char default_value[MAX_INI_CONFIG_DEFAULT_VALUE_LEN];
};

void m_ini_config_print_descr(struct m_ini_config_descr descr[]);

/*
 * MODE1:
 *
 * [abc]
 * a = value1
 * b = value2
 *
 * [lmn]
 * n = value1
 * m = value2
 */
int m_ini_config_read(const char *filename, struct m_ini_config_descr descr[]);


/*
 * MODE2:
 *
 * [pair1]
 * id = 1
 * name = name1
 * 
 * [pair2]
 * id=2
 * name = name2
 * 
 * [pair3]
 * id = 3
 * name = name3
 *
 * ...
 *
 */

struct m_ini_config {
	struct ini *ini;
	struct read_ini *rini;
	int next_section;
};

struct m_ini_config *m_ini_config_init(const char *filename);
/* rtn: 0 - OK,  1 - end, -1 - error. */
int m_ini_config_next(struct m_ini_config *config, struct m_ini_config_descr descr[]);
void m_ini_config_clean(struct m_ini_config *config);


#endif
